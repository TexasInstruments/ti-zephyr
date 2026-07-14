#!/usr/bin/env python3
"""
Run lp_mspm0g3519 twister tests using the SDK's prebuilt TI OpenOCD fork.

Usage:
    python3 run.py [tests ...]
    # or via top-level dispatcher:
    python3 ti-zephyr/tests/run.py --board lp_mspm0g3519 [tests ...]

If no tests given, defaults to this SDK's own samples/ and tests/ dirs
(not upstream Zephyr's entire test suite for the platform).

OpenOCD is expected already built at toolchains/openocd (via
scripts/setup_openocd.py during install.py) — this script does not fetch or
build it. Board-level reset-wrapper fixes live in
zephyr/boards/ti/lp_mspm0g3519/{board.cmake,support/openocd.cfg}.
"""

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path

SCRIPT_DIR = Path(__file__).parent.resolve()
# Script lives at ti-zephyr/tests/boards/lp_mspm0g3519/ — 4 levels up = workspace root
WEST_DIR = SCRIPT_DIR.parents[3]
ZEPHYR_DIR = WEST_DIR / "zephyr"
TI_ZEPHYR_DIR = WEST_DIR / "ti-zephyr"
OPENOCD_DIR = WEST_DIR / "toolchains" / "openocd"
TWISTER_OUT = ZEPHYR_DIR / "twister-out"
REPORT_FILE = WEST_DIR / "g3519_report.html"

# twister's own default test_roots (no -T given) is hardcoded to
# ZEPHYR_BASE/{tests,samples} only — it does not read a module's `samples:`/
# `tests:` module.yml keys (checked: no consumer anywhere in
# scripts/pylib/twister). So default to this SDK's own curated content
# explicitly instead of silently falling through to upstream Zephyr's entire
# test suite for this platform.
DEFAULT_TEST_ROOTS = [TI_ZEPHYR_DIR / "samples", TI_ZEPHYR_DIR / "tests"]


def run(cmd, cwd=None):
    subprocess.run(cmd, cwd=cwd, check=True)


def check_openocd():
    binary = OPENOCD_DIR / "src" / "openocd"
    if not binary.exists():
        print(f"ERROR: openocd binary not found at {binary}")
        print("Run scripts/setup_openocd.py (or install.py) first.")
        sys.exit(1)


def run_tests(tests):
    jobs = max(1, os.cpu_count() // 2)
    args = ["west", "twister",
            "-p", "lp_mspm0g3519",
            "--hardware-map", str(SCRIPT_DIR / "map.yml"),
            "--device-testing", "-v",
            "-j", str(jobs),
            "--retry-failed", "3"]
    roots = tests or [str(p) for p in DEFAULT_TEST_ROOTS if p.is_dir()]
    for t in roots:
        args += ["-T", t]
    run(args, cwd=ZEPHYR_DIR)


def generate_report():
    data = json.loads((TWISTER_OUT / "twister.json").read_text())
    suites = data.get("testsuites", [])

    passed  = sum(1 for s in suites if s.get("status") == "passed")
    failed  = sum(1 for s in suites if s.get("status") == "failed")
    filtered = sum(1 for s in suites if s.get("status") == "filtered")
    total   = len(suites)

    rows = []
    for ts in suites:
        status  = ts.get("status", "unknown")
        name    = ts.get("name", "")
        elapsed = float(ts.get("execution_time") or 0)
        reason  = ts.get("reason", "")

        log_html = ""
        if status in ("failed", "passed"):
            log_path = TWISTER_OUT / ts["path"] / "handler.log"
            if log_path.exists():
                log = log_path.read_text(errors="replace")
                escaped = log.replace("&", "&amp;").replace("<", "&lt;").replace(">", "&gt;")
                log_html = (f'<details><summary>log</summary>'
                            f'<pre style="background:#1a1a2e;color:#eee;padding:10px;'
                            f'font-size:11px;max-height:300px;overflow:auto">{escaped}</pre>'
                            f'</details>')

        color = {"passed": "#2ecc71", "failed": "#e74c3c",
                 "filtered": "#95a5a6"}.get(status, "#bdc3c7")
        rows.append(
            f'<tr>'
            f'<td><span style="background:{color};color:#fff;padding:2px 8px;'
            f'border-radius:3px;font-size:12px">{status}</span></td>'
            f'<td style="font-family:monospace;font-size:13px">{name}</td>'
            f'<td>{elapsed:.1f}s</td>'
            f'<td>{reason}</td>'
            f'<td>{log_html}</td>'
            f'</tr>'
        )

    html = f"""<!DOCTYPE html>
<html>
<head>
<meta charset="utf-8">
<title>g3519 Twister Report</title>
<style>
  body {{ font-family: sans-serif; background: #f5f5f5; margin: 20px; }}
  h1 {{ color: #2c3e50; }}
  .summary {{ display: flex; gap: 20px; margin-bottom: 20px; }}
  .card {{ background: #fff; border-radius: 8px; padding: 16px 24px; box-shadow: 0 1px 4px rgba(0,0,0,.1); }}
  .card .num {{ font-size: 32px; font-weight: bold; }}
  .pass {{ color: #2ecc71; }} .fail {{ color: #e74c3c; }}
  .filt {{ color: #95a5a6; }} .total {{ color: #3498db; }}
  table {{ width: 100%; border-collapse: collapse; background: #fff;
           border-radius: 8px; overflow: hidden; box-shadow: 0 1px 4px rgba(0,0,0,.1); }}
  th {{ background: #2c3e50; color: #fff; padding: 10px 12px; text-align: left; }}
  td {{ padding: 8px 12px; border-bottom: 1px solid #eee; vertical-align: top; }}
  tr:hover {{ background: #f9f9f9; }}
  details summary {{ cursor: pointer; color: #3498db; }}
</style>
</head>
<body>
<h1>lp_mspm0g3519 Twister Report</h1>
<div class="summary">
  <div class="card"><div class="num total">{total}</div>Total</div>
  <div class="card"><div class="num pass">{passed}</div>Passed</div>
  <div class="card"><div class="num fail">{failed}</div>Failed</div>
  <div class="card"><div class="num filt">{filtered}</div>Filtered</div>
</div>
<table>
<thead><tr><th>Status</th><th>Test</th><th>Time</th><th>Reason</th><th>Log</th></tr></thead>
<tbody>
{"".join(rows)}
</tbody>
</table>
</body>
</html>"""

    REPORT_FILE.write_text(html)
    print(f"\n[g3519] open report: file://{REPORT_FILE}")


parser = argparse.ArgumentParser(description="Run lp_mspm0g3519 twister tests")
parser.add_argument("tests", nargs="*",
                    help="test dirs to pass as -T (default: all tests for platform)")
args = parser.parse_args()

check_openocd()
run_tests(args.tests)
generate_report()
