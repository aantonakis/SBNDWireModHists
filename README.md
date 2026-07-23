# SBNDWireModHists

This repository is used to create multidimensional histograms for WireMod splines. It also has functionality to project along various axes of the histograms to produce lower dimensional histograms. 


## Setup

First, you need to clone a calibration repository from Sungbin here: [github link](https://github.com/sungbinoh/SBND_calib_recom/blob/sungbino_etau_grid/src/run_recom_loop_emb.C)

Then, you will need to provide the path to this repository in the setup script. There are two main setup scripts which correspond to Gen1 and Gen2 productions. For each setup script, the path needs to be modified at the top of the file. For my personal use, the environment variable is, for example: 

```export CALIB_WORKING_DIR="/exp/sbnd/app/users/aantonak/SBND_calib_recom"```

Once the setup script `CALIB_WORKING_DIR` environment variable is modified, you need to source the setup script from within an SL7 container. There are two setup scripts corresponding to Gen1 and Gen2 productions.

* For Gen1 production: `source setup.sh`

* For Gen2 production: `source setup_gen2.sh`


Each one of these has the same `CALIB_WORKING_DIR` variable that needs to be set. This allows WireMod to apply calibration corrections before making splines. Two different setup scripts are needed due to the different calibrations in Gen1 vs Gen2 productions. This is all that is required to start running the code and there are 4 main modules that can be run over the Fermilab grid using the python scripts in the `grid` directory. We will cover these in the next section. Note that the grid setup scripts are not run by the user and do not need to be modified. These are used by the grid submission scripts defined in the next session. 


# Main Scripts


There are 4 main scripts that can be run on the grid: `multi_dim_tracks_grid.C`, `multi_dim_tracks_gen2_grid.C`, `merge_hists_grid.C` and `caf_multi_dim_tracks.C`

* `multi_dim_tracks_grid.C` and `multi_dim_tracks_gen2_grid.C` are for analyzing through-going cosmic muons in the Calibration NTuples for Gen1 and Gen2 productions respectively. 

* `merge_hists_grid.C` is used to take the outputs from the scripts in the previous bullet, project the output histograms along a set of axes and merge histograms over the grid.

* `caf_multi_dim_tracks.C` is the CAF Ntuple analogue of `multi_dim_tracks_gen2_grid.C`. It was primarily introduced for proton selections.

The basic steps to run each module over the grid is provided below:


## Script Useage


First, go into the `grid` top level directory. There you will find the grid submission scripts for each module: `submit_multi_dim_tracks.py`, `submit_multi_dim_tracks_gen2.py`, `submit_merge_hists.py`, and `submit_caf_multi_dim_tracks.py`

* To run `multi_dim_tracks_grid.C` over the grid, do the following:

```python submit_multi_dim_tracks.py -l <file_list> -o <output_tag> -c <config file> -nfile <# of files to process> -ngrid <# of grid jobs to run>```


* similarly, to run `multi_dim_tracks_gen2_grid.C, do the following:

````python submit_multi_dim_tracks_gen2.py -l <file_list> -o <output_tag> -c <config file> -nfile <# of files to process> -ngrid <# of grid jobs to run>```  


These scripts will write the output files to the users scratch directory under: `/path-to-scratch/sbnd_calib/` 
The directory will be created if it hasn't been yet and it will dump log files there too. The file list needs to be xrootd, the output tag defines the naming scheme of the output directory created under `/path-to-scratch/sbnd_calib/`, and the Config files that can be passed to the `-c` argument are included in the `Configs` top-level directory. These configs can be adjusted to change the selections, dimensions to keep when making histograms, as well as the binning of the histograms.

* To run `merge_hists_grid.C`, do the following:

```python submit_merge_hists.py -l <file_list> -o <output_tag> -nfile <# of files to process> -ngrid <# of grid jobs to run>```

In this case, the input file list is from the output of the histogram generation code and the number of grid jobs determines how many histograms are merged together. In other words, the number of grid jobs determines the number of output files, so the projected histograms from each batch of input files are merged together. The axes/dimensions to keep in the output histograms is configured in the script `grid/bin/grid_executable_merge_hists.sh`. The indices of the axes to keep are configured in the line `root -l -b -q "merge_hists_grid.C(\"input_list_${nProcess}.txt\", \"${nProcess}\", {0, 1, 2, 3, 4})" &> log_${nProcess}.log`, where  {0, 1, 2, 3, 4} correspond to the indices of the axes/dimensions to keep. This needs to be updated in the future to have a config file input like the other scripts.








