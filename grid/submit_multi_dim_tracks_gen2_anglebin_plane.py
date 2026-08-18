#!/usr/bin/env python3
from __future__ import annotations

import argparse
import subprocess
import sys
from pathlib import Path


REPO_DIR = Path(__file__).resolve().parents[1]
GEN2_CONFIG_DIR = REPO_DIR / "Configs" / "Gen2"
SUBMIT_SCRIPT = REPO_DIR / "grid" / "submit_multi_dim_tracks_gen2_anglebin.py"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description=(
            "Submit multi_dim_tracks_gen2_anglebin jobs for every angle-bin config "
            "in one plane and observable directory."
        )
    )
    parser.add_argument(
        "--plane",
        required=True,
        help="Plane to submit, either as an integer like 0 or a directory name like Plane0.",
    )
    parser.add_argument(
        "--observable",
        choices=["base", "q", "dq", "w"],
        default="q",
        help="AngleBins directory to use: base -> AngleBins, q -> AngleBins_q, etc.",
    )
    parser.add_argument(
        "--anglebins-dir",
        type=Path,
        default=None,
        help="Explicit AngleBins directory. Overrides --observable.",
    )
    parser.add_argument(
        "--sample",
        choices=["data", "mc"],
        required=True,
        help="Sample label inserted in the -o output prefix.",
    )
    parser.add_argument(
        "--tag",
        default="gen2",
        help="Leading output label inserted before data/mc in the -o output prefix.",
    )
    parser.add_argument(
        "-l",
        "--inputfilelist",
        required=True,
        help="Input file list passed through to submit_multi_dim_tracks_gen2_anglebin.py.",
    )
    parser.add_argument(
        "-nfile",
        dest="nfiles",
        default=0,
        type=int,
        help="Number of files to process for each config. Passed through unchanged.",
    )
    parser.add_argument(
        "-ngrid",
        dest="ngrid",
        default=0,
        type=int,
        help="Number of grid jobs for each config. Passed through unchanged.",
    )
    parser.add_argument(
        "--limit",
        type=int,
        default=None,
        help="Submit only the first N configs. Useful for a quick test.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print the commands without executing them.",
    )
    return parser.parse_args()


def plane_dir_name(plane: str) -> str:
    if plane.lower().startswith("plane"):
        suffix = plane[5:]
        if not suffix.isdigit():
            raise ValueError(f"Could not parse plane value: {plane}")
        return f"Plane{int(suffix)}"
    if not plane.isdigit():
        raise ValueError(f"Could not parse plane value: {plane}")
    return f"Plane{int(plane)}"


def anglebins_dir(args: argparse.Namespace) -> Path:
    if args.anglebins_dir is not None:
        return args.anglebins_dir
    if args.observable == "base":
        return GEN2_CONFIG_DIR / "AngleBins"
    return GEN2_CONFIG_DIR / f"AngleBins_{args.observable}"


def output_prefix(tag: str, sample: str, config: Path) -> str:
    return f"{tag}_{sample}_{config.stem}"


def command_for_config(args: argparse.Namespace, config: Path) -> list[str]:
    return [
        sys.executable,
        str(SUBMIT_SCRIPT),
        "-l",
        args.inputfilelist,
        "-nfile",
        str(args.nfiles),
        "-ngrid",
        str(args.ngrid),
        "-o",
        output_prefix(args.tag, args.sample, config),
        "-c",
        str(config),
    ]


def main() -> None:
    args = parse_args()
    config_root = anglebins_dir(args) / plane_dir_name(args.plane)
    if not config_root.exists():
        raise FileNotFoundError(config_root)

    configs = sorted(config_root.glob("*.cfg"))
    if args.limit is not None:
        configs = configs[: args.limit]
    if not configs:
        raise FileNotFoundError(f"No .cfg files found in {config_root}")

    print(f"Found {len(configs)} configs in {config_root}")
    for config in configs:
        cmd = command_for_config(args, config)
        print(" ".join(cmd))
        if not args.dry_run:
            subprocess.run(cmd, cwd=str(REPO_DIR), check=True)


if __name__ == "__main__":
    main()
