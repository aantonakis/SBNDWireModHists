# SBNDWireModHists

This repository is used to create multidimensional histograms for WireMod splines. It also has functionality to project along various axes of the histograms to produce lower dimensional histograms. 


## Setup

First, you need to clone a calibration repository from Sungbin here: [github link](https://github.com/sungbinoh/SBND_calib_recom/blob/sungbino_etau_grid/src/run_recom_loop_emb.C)

Then, you will need to provide the path to this repository in the setup script. There are two main setup scripts which correspond to Gen1 and Gen2 productions. For each setup script, the path needs to be modified at the top of the file. For my personal use, the environment variable is: `export CALIB_WORKING_DIR="/exp/sbnd/app/users/aantonak/SBND_calib_recom"`.

Once the setup script `CALIB_WORKING_DIR` environment variable is modified, you need to source the setup script from within an SL7 container. There are two setup scripts corresponding to Gen1 and Gen2 productions.

* For Gen1 production: `source setup.sh`

* For Gen2 production: `source setup_gen2.sh`


Each one of these has the same `CALIB_WORKING_DIR` variable that needs to be set. This allows WireMod to apply calibration corrections before making splines. Two different setuo scripts are needed due to the diffent calibrations in Gen1 vs Gen2 productions. This is all that is required to start running the code and there are 4 main modules that can be run over the Fermilab grid using the python scripts in the `grid` directory. We will cover these in the next section.







