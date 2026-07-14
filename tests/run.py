#!/usr/bin/env python3
"""
TI Zephyr SDK test dispatcher.

Routes to the per-board test runner under tests/boards/<board>/run.py.

Usage:
    python3 tests/run.py --board lp_mspm0g3519 [runner-args ...]
    python3 tests/run.py --board lp_mspm33c321a [runner-args ...]

Adding a new board:
    mkdir tests/boards/<new_board>
    cp tests/boards/lp_mspm0g3519/run.py tests/boards/<new_board>/run.py
    # edit: update board string, openocd args, etc.
    cp tests/boards/lp_mspm0g3519/map.yml tests/boards/<new_board>/map.yml
    # edit: update platform, serial device, openocd paths
"""

import argparse
import os
import subprocess
import sys

BOARDS_DIR = os.path.join(os.path.dirname(os.path.abspath(__file__)), "boards")


def list_boards():
    if not os.path.isdir(BOARDS_DIR):
        return []
    return sorted(
        d for d in os.listdir(BOARDS_DIR)
        if os.path.isfile(os.path.join(BOARDS_DIR, d, "run.py"))
    )


parser = argparse.ArgumentParser(
    description="TI SDK test dispatcher — routes to per-board runner",
    formatter_class=argparse.RawDescriptionHelpFormatter,
)
parser.add_argument("--board", required=True,
                    help=f"Board target. Available: {', '.join(list_boards()) or 'none yet'}")
parser.add_argument("--list-boards", action="store_true",
                    help="List boards that have a test runner")
args, remaining = parser.parse_known_args()

if args.list_boards:
    boards = list_boards()
    print("Boards with test runners:")
    for b in boards:
        print(f"  {b}")
    sys.exit(0)

runner = os.path.join(BOARDS_DIR, args.board, "run.py")
if not os.path.exists(runner):
    boards = list_boards()
    print(f"[ERROR] No test runner for board: {args.board}")
    print(f"  Expected: {runner}")
    if boards:
        print(f"  Available: {', '.join(boards)}")
    sys.exit(1)

sys.exit(subprocess.call([sys.executable, runner] + remaining))
