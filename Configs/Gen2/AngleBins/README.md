# Gen2 WireMod Angle-Bin Configs

These configs select one folded 2D angular WireMod region and one global plane index per job.

The generated angle windows come from:

```text
/Users/alexanderantonakis/Documents/Codex/2026-06-08/i-want-to-start-a-quick/plots/angles_2d/*priority60_wiremod_bins.csv
```

Each config sets:

```text
PlaneIndex: <0..5>
UseAbsAngles: true
ThetaXWMin / ThetaXWMax
ThetaYZMin / ThetaYZMax
```

The macro interprets `PlaneIndex` as the global index:

```text
hit_idx = plane + 3 * tpc
```

Regenerate configs from the CSV bin tables:

```bash
python3 scripts/generate_gen2_anglebin_configs.py
```

Example grid submission for one generated config:

```bash
python3 grid/submit_multi_dim_tracks_gen2_anglebin.py \
  -ngrid 10 \
  -o gen2_wiremod_plane0_region001 \
  -l <input_file_list.txt> \
  -c Configs/Gen2/AngleBins/Plane0/config_xyz_spline_anglebin_plane0_region001.cfg
```
