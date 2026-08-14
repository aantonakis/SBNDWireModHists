from __future__ import annotations

import argparse
from pathlib import Path


PROJECT_DIR = Path(__file__).resolve().parents[1]
DEFAULT_GEN2_CONFIG_DIR = PROJECT_DIR.parent / "SBNDWireModHists" / "Configs" / "Gen2"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Set isData true/false in generated WireMod angle-bin config files."
    )
    target = parser.add_mutually_exclusive_group()
    target.add_argument(
        "config_dir",
        type=Path,
        nargs="?",
        default=None,
        help="Specific directory containing generated .cfg files.",
    )
    target.add_argument(
        "--anglebins-name",
        default=None,
        help="Name of one AngleBins directory under SBNDWireModHists/Configs/Gen2, e.g. AngleBins_q.",
    )
    target.add_argument(
        "--all-anglebins",
        action="store_true",
        help="Update every AngleBins* directory under SBNDWireModHists/Configs/Gen2.",
    )
    parser.add_argument(
        "--value",
        choices=["true", "false"],
        default="false",
        help="Value to set for isData.",
    )
    parser.add_argument(
        "--dry-run",
        action="store_true",
        help="Print files that would be changed without editing them.",
    )
    return parser.parse_args()


def target_dirs(args: argparse.Namespace) -> list[Path]:
    if args.all_anglebins:
        dirs = [
            path
            for path in sorted(DEFAULT_GEN2_CONFIG_DIR.glob("AngleBins*"))
            if path.is_dir()
        ]
        if not dirs:
            raise FileNotFoundError(f"No AngleBins* directories found under {DEFAULT_GEN2_CONFIG_DIR}")
        return dirs

    if args.anglebins_name:
        return [DEFAULT_GEN2_CONFIG_DIR / args.anglebins_name]

    if args.config_dir is not None:
        return [args.config_dir]

    return [DEFAULT_GEN2_CONFIG_DIR / "AngleBins"]


def update_file(path: Path, value: str, dry_run: bool) -> bool:
    lines = path.read_text().splitlines(keepends=True)
    changed = False
    found = False
    updated_lines = []

    for line in lines:
        stripped = line.lstrip()
        prefix = line[: len(line) - len(stripped)]
        if stripped.startswith("isData:"):
            found = True
            newline = "\n" if line.endswith("\n") else ""
            new_line = f"{prefix}isData: {value}{newline}"
            updated_lines.append(new_line)
            changed = changed or new_line != line
        else:
            updated_lines.append(line)

    if not found:
        raise ValueError(f"No isData line found in {path}")

    if changed and not dry_run:
        path.write_text("".join(updated_lines))

    return changed


def main() -> None:
    args = parse_args()

    changed_files = []
    dirs = target_dirs(args)
    for config_dir in dirs:
        if not config_dir.exists():
            raise FileNotFoundError(config_dir)
        for path in sorted(config_dir.rglob("*.cfg")):
            if update_file(path, args.value, args.dry_run):
                changed_files.append(path)

    action = "Would update" if args.dry_run else "Updated"
    print(f"{action} {len(changed_files)} config files")
    for config_dir in dirs:
        print(f"Target: {config_dir}")
    for path in changed_files[:20]:
        print(path)
    if len(changed_files) > 20:
        print(f"... and {len(changed_files) - 20} more")


if __name__ == "__main__":
    main()
