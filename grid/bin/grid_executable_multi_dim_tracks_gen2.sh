# Setup grid submission

echo "@@hostname: ${hostname}"
hostname

outDir=$1
echo "@@ outDir : ${outDir}"
DFPREFIX=$2

nProcess=$PROCESS
echo "@@ nProcess : ${nProcess}"


config=$3
echo "@@ config : ${config}"


echo "@@ pwd"
pwd
echo "@@ ls -alh"
ls -alh

thisOutputCreationDir=`pwd`
filesFromSender=${CONDOR_DIR_INPUT}/bin_dir/

echo "@@ check filesFromSender dir"
ls -alh ${filesFromSender}

echo "@@ copy scripts"
mkdir -p data/sample_list/sbndgpvm/
cp ${filesFromSender}/input_list_${nProcess}.txt ./data/sample_list/sbndgpvm/
ls -alh ./data/sample_list/sbndgpvm/
cp ${filesFromSender}/setup_grid_gen2.sh .
cp -r ${filesFromSender}/bin .
cp -r ${filesFromSender}/include .
cp -r ${filesFromSender}/include_wire .
cp -r ${filesFromSender}/multi_dim_tracks_gen2_grid.C .
cp ${filesFromSender}/${config} .

echo "@@ make output/root"
mkdir -p output/root
#ifdh  mkdir_p ${outDir}
ls -alh

echo "@@ source setup_grid_gen2.sh"
source setup_grid_gen2.sh
export SBND_YZCORR_PATH=${filesFromSender}/

echo "@@ YZ path : ${SBND_YZCORR_PATH}"

spack find ifdhc
#spack load ifdhc@2.8.0 arch=linux-almalinux9-x86_64_v3
#spack load ifdhc@2.7.2
spack load ifdhc@2.6.20

spack find root

spack find sqlite

spack load sqlite

which root
root-config --cflags
spack location -i sqlite

echo "@@ run"

# dim = {x, y, z, txz, tyz, dq/dx, Q, width, goodness, pathological}
#SQLITE_DIR=$(spack location -i sqlite)

#root -l -b -q \
#'gSystem->AddIncludePath("-I'"$SQLITE_DIR"'/include"); gSystem->Load("'"$SQLITE_DIR"'/lib/libsqlite3.so"); .L multi_dim_tracks_gen2_grid.C+'


root -l -b -q "multi_dim_tracks_gen2_grid.C(\"input_list_${nProcess}.txt\", \"${nProcess}\", \"${config}\")" &> log_${nProcess}.log


# //////////////////////////////////////////////////////////

ls -alh

echo "@@ setup IFDH_CP_MAXRETRIES to 2"
export IFDH_CP_MAXRETRIES=2

echo "@@ outDir : "${outDir}
echo "@@ ifdh  mkdir_p "${outDir}
ifdh  mkdir_p ${outDir}

outFILE=${thisOutputCreationDir}/output/root/output_multi_dim_tracks_${nProcess}.root
#outFILE=${thisOutputCreationDir}/${outDir}/output_ndhist_charges_tracks_crossers_${nProcess}.root
if [ -f "$outFILE" ]; then
  echo "ifdh cp ${thisOutputCreationDir}/output/root/output_multi_dim_tracks_${nProcess}.root ${outDir}/${DFPREFIX}_${nProcess}.root"
  ifdh cp ${thisOutputCreationDir}/output/root/output_multi_dim_tracks_${nProcess}.root ${outDir}/${DFPREFIX}_${nProcess}.root
  echo "ifdh cp ${thisOutputCreationDir}/log_${nProcess}.log ${outDir}/log_${nProcess}.log"
  ifdh cp ${thisOutputCreationDir}/log_${nProcess}.log ${outDir}/log_${nProcess}.log
  echo "@@ Done!"
else
  ifdh cp ${thisOutputCreationDir}/log_${nProcess}.log ${outDir}/log_${nProcess}.log
  echo "File not exist"
fi

