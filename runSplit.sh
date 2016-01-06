#!/bin/bash

run=$1
destinationdir="srm://t3se01.psi.ch/pnfs/psi.ch/cms/trivcat/store/user/micheli/rawData/"
echo $destinationdir

for i in {1..9..1}
do
    cp test/configPhysicsDummySplit.dat test/configPhysicsDummySplit_"$run"_$i.dat
    cp data/physicsRunsDummySplit.dat data/physicsRunsDummySplit_"$run"_$i.dat
    sed -i "s|3990|$run\_$i|g" test/configPhysicsDummySplit_"$run"_$i.dat
    sed -i "s|3990|$run|" data/physicsRunsDummySplit_"$run"_$i.dat
    sed -i "s|*.root|$i*.root|" data/physicsRunsDummySplit_"$run"_$i.dat
    sed -i "s|physicsRunsDummySplit|physicsRunsDummySplit_"$run"_$i|" test/configPhysicsDummySplit_"$run"_$i.dat
    python python/Loop.py --input test/configPhysicsDummySplit_"$run"_$i.dat --debug 3
done

hadd output_run$run.root output_run"$run"_*.root
rm output_run"$run"_*.root
gfal-copy file:///shome/micheli/H4Analysis/output_run$run.root $destinationdir
rm output_run$run.root