from __future__ import annotations

import argparse
import csv
import re
from pathlib import Path


REPO_DIR = Path(__file__).resolve().parents[1]
DEFAULT_BINS_DIR = (
    REPO_DIR.parent
    / "i-want-to-start-a-quick"
    / "plots"
    / "angles_2d"
)
DEFAULT_TEMPLATE = REPO_DIR / "Configs" / "Gen2" / "config_xyz_spline_anglebin_template.cfg"
DEFAULT_OUTDIR = REPO_DIR / "Configs" / "Gen2" / "AngleBins"


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(
        description="Generate one Gen2 WireMod config per folded 2D angular bin."
    )
    parser.add_argument(
        "--bins-dir",
        type=Path,
        default=DEFAULT_BINS_DIR,
        help="Directory containing hTrack*_angles_2d_counts_*_wiremod_bins.csv files.",
    )
    parser.add_argument(
        "--pattern",
        default="hTrack*_angles_2d_counts_folded_abs_priority60_wiremod_bins.csv",
        help="Glob pattern for the angular bin CSVs.",
    )
    parser.add_argument(
        "--template",
        type=Path,
        default=DEFAULT_TEMPLATE,
        help="Base config template to copy and override.",
    )
    parser.add_argument(
        "--outdir",
        type=Path,
        default=DEFAULT_OUTDIR,
        help="Output directory for generated configs.",
    )
    return parser.parse_args()


def hist_plane_from_name(path: Path) -> int:
    match = re.search(r"hTrack(\d+)", path.name)
    if not match:
        raise ValueError(f"Could not determine hTrack index from {path}")
    return int(match.group(1))


def format_float(value: str) -> str:
    return f"{float(value):g}"


def config_text(template_text: str, plane: int, row: dict[str, str]) -> str:
    lines = template_text.splitlines()
    overrides = {
        "PlaneIndex": str(plane),
        "UseAbsAngles": "true",
        "ThetaXWMin": format_float(row["theta_xw_min_deg"]),
        "ThetaXWMax": format_float(row["theta_xw_max_deg"]),
        "ThetaYZMin": format_float(row["theta_yz_min_deg"]),
        "ThetaYZMax": format_float(row["theta_yz_max_deg"]),
        "AngleBinRegion": row["region"],
        "AngleBinTracks": format_float(row["tracks"]),
    }

    seen = set()
    rendered = []
    for line in lines:
        stripped = line.strip()
        key = stripped.split(":", 1)[0] if ":" in stripped else None
        if key in overrides:
            rendered.append(f"{key}: {overrides[key]}")
            seen.add(key)
        else:
            rendered.append(line)

    for key, value in overrides.items():
        if key not in seen:
            rendered.append(f"{key}: {value}")

    rendered.append("")
    rendered.append(f"# Source angular bin CSV region {row['region']}")
    rendered.append(
        "# theta_xw: "
        f"{format_float(row['theta_xw_min_deg'])} to {format_float(row['theta_xw_max_deg'])} deg"
    )
    rendered.append(
        "# theta_yz: "
        f"{format_float(row['theta_yz_min_deg'])} to {format_float(row['theta_yz_max_deg'])} deg"
    )
    return "\n".join(rendered) + "\n"


def main() -> None:
    args = parse_args()
    template_text = args.template.read_text()
    args.outdir.mkdir(parents=True, exist_ok=True)

    csv_paths = sorted(args.bins_dir.glob(args.pattern))
    if not csv_paths:
        raise FileNotFoundError(f"No angular bin CSVs found in {args.bins_dir} matching {args.pattern}")

    total = 0
    for csv_path in csv_paths:
        plane = hist_plane_from_name(csv_path)
        plane_dir = args.outdir / f"Plane{plane}"
        plane_dir.mkdir(parents=True, exist_ok=True)

        with csv_path.open() as stream:
            for row in csv.DictReader(stream):
                region = int(row["region"])
                out_path = plane_dir / f"config_xyz_spline_anglebin_plane{plane}_region{region:03d}.cfg"
                out_path.write_text(config_text(template_text, plane, row))
                total += 1

    print(f"Wrote {total} angle-bin configs to {args.outdir}")


if __name__ == "__main__":
    main()
