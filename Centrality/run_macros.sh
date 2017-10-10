#!/bin/bash

#
FOREST_XeXe="/export/d00/scratch/jwang/HLTXeXe/HiForestAOD_MERGED.root"
FOREST_PbPb="/mnt/hadoop/cms/store/user/tatar/official/Hydjet_Quenched_Cymbal5Ev8_PbPbMinBias_5020GeV/HINPbPbWinter16DR-NoPU_75X_mcRun2_HeavyIon_v14-v1-FOREST/0.root"

#
FOLDERS=("rootfiles" "plots")
for i in ${FOLDERS[@]}
do
    if [[ ! -d $i ]]
    then
        mkdir -p $i
    fi
done

#
g++ calibration.C $(root-config --cflags --libs) -g -o calibration.exe
g++ nTrkvshiBin_savehist.C $(root-config --cflags --libs) -g -o nTrkvshiBin_savehist.exe
g++ nTrkvshiBin_plothist.C $(root-config --cflags --libs) -g -o nTrkvshiBin_plothist.exe

#
./calibration.exe $FOREST_XeXe rootfiles/calibration
./nTrkvshiBin_savehist.exe $FOREST_XeXe rootfiles/output XeXe
./nTrkvshiBin_savehist.exe $FOREST_PbPb rootfiles/output PbPb
./nTrkvshiBin_plothist.exe rootfiles/output PbPb XeXe


