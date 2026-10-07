# SPDX-FileCopyrightText: 2026 Texas Instruments Incorporated
# SPDX-License-Identifier: LicenseRef-TI-TFL

"""Generate a landing index.html for the GitHub Pages docs site.

Scans the given site directory for per-SoC subdirectories (each a Sphinx
build from ``make -C doc html SOC=<soc>``) and writes a top-level
index.html linking to each one. Only the landing page of the published
site; it is not part of the Sphinx build.

Styled to match the per-SoC Sphinx docs (TI red header, #404040 body text
on white) - see doc/_static/custom.css.

Usage: generate_docs_index.py <site_dir> <logo_png>
"""

import base64
import html
import sys
from pathlib import Path

# soc -> (display name, devices) - device text is cosmetic context for the
# card, from the board table in doc/introduction/index.rst. SoCs missing
# here still get a card, with just their upper-cased name.
SOC_INFO = {
    "am13":   ("AM13", "AM13E230"),
    "am62l":  ("AM62L", "AM62L"),
    "mspm0":  ("MSPM0", "MSPM0G3507 / MSPM0G3519 / MSPM0L2228"),
    "mspm33": ("MSPM33", "MSPM33C321"),
}

TITLE = "Zephyr SDK Documentation"


def main():
    site_dir = Path(sys.argv[1])
    logo_path = Path(sys.argv[2])

    socs = sorted(
        d.name for d in site_dir.iterdir()
        if d.is_dir() and (d / "index.html").is_file()
    )
    if not socs:
        sys.exit(f"No SoC builds found in {site_dir}")

    logo_b64 = base64.b64encode(logo_path.read_bytes()).decode()

    cards = []
    for soc in socs:
        name, devices = SOC_INFO.get(soc, (soc.upper(), ""))
        cards.append(f"""      <a class="card" href="{soc}/index.html">
        <h2>{html.escape(name)}</h2>
        <p>{html.escape(devices)}</p>
        <span class="card-link">View documentation &rarr;</span>
      </a>""")
    cards = "\n".join(cards)

    (site_dir / "index.html").write_text(f"""<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="utf-8">
<meta name="viewport" content="width=device-width, initial-scale=1">
<title>{TITLE}</title>
<style>
  body {{
    margin: 0;
    font-family: Lato, "Helvetica Neue", Arial, sans-serif;
    color: #404040;
    background-color: #FCFCFC;
  }}
  .topbar {{
    background-color: #ffffff;
    padding: 20px 40px;
  }}
  .topbar img {{
    height: 32px;
  }}
  .navbar {{
    background-color: #CC0000;
    padding: 16px 40px;
  }}
  .navbar span {{
    color: #ffffff;
    font-size: 15px;
    font-weight: bold;
    letter-spacing: 0.02em;
  }}
  main {{
    max-width: 960px;
    margin: 0 auto;
    padding: 40px 24px;
  }}
  main h1 {{
    font-size: 34px;
    font-weight: bold;
    color: #1a1a1a;
    margin: 0 0 8px 0;
  }}
  main > p {{
    font-size: 17px;
    color: #757575;
    margin-top: 0;
    margin-bottom: 32px;
  }}
  .grid {{
    display: grid;
    grid-template-columns: repeat(auto-fit, minmax(220px, 1fr));
    gap: 20px;
    margin-top: 24px;
  }}
  .card {{
    display: block;
    border: 1px solid #e0e0e0;
    border-radius: 6px;
    padding: 20px;
    text-decoration: none;
    color: #404040;
    background-color: #ffffff;
    transition: border-color 0.15s ease, box-shadow 0.15s ease;
  }}
  .card:hover {{
    border-color: #CC0000;
    box-shadow: 0 2px 8px rgba(0, 0, 0, 0.08);
  }}
  .card h2 {{
    margin: 0 0 4px 0;
    font-size: 18px;
    color: #1a1a1a;
  }}
  .card p {{
    margin: 0;
    font-size: 13px;
    color: #757575;
  }}
  .card-link {{
    display: inline-block;
    margin-top: 12px;
    font-size: 13px;
    font-weight: bold;
    color: #CC0000;
  }}
  footer {{
    text-align: center;
    padding: 24px;
    font-size: 12px;
    color: #9a9a9a;
  }}
</style>
</head>
<body>
  <div class="topbar">
    <img src="data:image/png;base64,{logo_b64}" alt="Texas Instruments">
  </div>
  <div class="navbar">
    <span>{TITLE}</span>
  </div>
  <main>
    <h1>{TITLE}</h1>
    <p>Zephyr RTOS-based software development kit &mdash; select a device below to view its documentation.</p>
    <div class="grid">
{cards}
    </div>
  </main>
  <footer>&copy; 2026 Texas Instruments Incorporated</footer>
</body>
</html>
""")


if __name__ == "__main__":
    main()
