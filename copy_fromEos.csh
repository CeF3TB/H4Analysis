#!/bin/tcsh

#gfal-copy root://eoscms.cern.ch///store/group/dpg_ecal/alca_ecalcalib/ECALTB_H4_Fall2015/raw/DataTree/4055/*.root srm://t3se01.psi.ch//pnfs/psi.ch/cms/trivcat/store/t3groups/ethz-ecal/H4TestBeamFall2015/raw/DataTree/4055/

  set dir = root://eoscms.cern.ch///store/group/dpg_ecal/alca_ecalcalib/ECALTB_H4_Fall2015/raw/DataTree/4053/
  set destinationdir = srm://t3se01.psi.ch/pnfs/psi.ch/cms/trivcat/store/t3groups/ethz-ecal/H4TestBeamFall2015/raw/DataTree/4053/
#  mkdir $i 
  foreach f (`gfal-ls $dir` )
     set file = $dir$f
     echo "copying $file"
    gfal-copy $file $destinationdir
  end
#end  
