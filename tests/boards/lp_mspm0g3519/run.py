#!/usr/bin/env python3
"""
Setup openocd vivian_m0g3519 and run lp_mspm0g3519 twister tests.

Usage:
    python3 run.py [--openocd-remote REMOTE] [tests ...]
    # or via top-level dispatcher:
    python3 ti-zephyr/tests/run.py --board lp_mspm0g3519 [tests ...]

Assumes: already on zephyr vivian_m0g3519 branch.
If no tests given, runs all tests for the platform.

openocd branch vivian_m0g3519 contains:
  - MSPMZEP-53 reset reconfiguration (TCL base required for m0 support)
  Board-level fixes live in boards/ti/lp_mspm0g3519/support/openocd.cfg.

Set OPENOCD_REMOTE to the remote where vivian_m0g3519 has been pushed.
"""

import argparse
import json
import os
import subprocess
import sys
from pathlib import Path

BRANCH = "vivian_m0g3519"
# TODO: set this to the remote where vivian_m0g3519 is pushed
OPENOCD_REMOTE_DEFAULT = ""

SCRIPT_DIR = Path(__file__).parent.resolve()
# Script lives at ti-zephyr/tests/boards/lp_mspm0g3519/ — 4 levels up = workspace root
WEST_DIR = SCRIPT_DIR.parents[3]
ZEPHYR_DIR = WEST_DIR / "zephyr"
OPENOCD_DIR = WEST_DIR / "openocd"
TWISTER_OUT = ZEPHYR_DIR / "twister-out"
REPORT_FILE = WEST_DIR / "g3519_report.html"


def run(cmd, cwd=None):
    subprocess.run(cmd, cwd=cwd, check=True)


def setup_openocd(remote):
    if not OPENOCD_DIR.is_dir():
        print(f"ERROR: openocd not found at {OPENOCD_DIR}")
        sys.exit(1)
    if remote:
        run(["git", "fetch", remote], cwd=OPENOCD_DIR)
        run(["git", "checkout", BRANCH], cwd=OPENOCD_DIR)
        run(["git", "reset", "--hard", f"{remote}/{BRANCH}"], cwd=OPENOCD_DIR)
    else:
        run(["git", "checkout", BRANCH], cwd=OPENOCD_DIR)
    result = subprocess.run(["git", "log", "--oneline", "-1"],
                            cwd=OPENOCD_DIR, capture_output=True, text=True)
    print(f"[g3519] openocd: {result.stdout.strip()}")
    binary = OPENOCD_DIR / "src" / "openocd"
    if not binary.exists():
        print("[g3519] Building openocd...")
        run(["./bootstrap"], cwd=OPENOCD_DIR)
        run(["./configure", "--disable-werror"], cwd=OPENOCD_DIR)
        run(["make", f"-j{max(1, os.cpu_count() // 2)}"], cwd=OPENOCD_DIR)


def run_tests(tests):
    jobs = max(1, os.cpu_count() // 2)
    args = ["west", "twister",
            "-p", "lp_mspm0g3519",
            "--hardware-map", str(SCRIPT_DIR / "map.yml"),
            "--device-testing", "-v",
            "-j", str(jobs),
            "--retry-failed", "3"]
    for t in tests:
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
parser.add_argument("--openocd-remote",
                    default=os.environ.get("OPENOCD_REMOTE", OPENOCD_REMOTE_DEFAULT),
                    help="remote with vivian_m0g3519 branch (e.g. origin)")
parser.add_argument("tests", nargs="*",
                    help="test dirs to pass as -T (default: all tests for platform)")
args = parser.parse_args()

setup_openocd(args.openocd_remote or None)
run_tests(args.tests)
generate_report()
