#!/bin/bash

run=$1

for i in {1..9..1}
do
    cp test/configPhysicsDummySplit.dat test/configPhysicsDummySplit_"$run"_$i.dat
    cp data/physicsRunsDummySplit.dat data/physicsRunsDummySplit_"$run"_$i.dat
    sed -i "s|3990|$run\_1|" test/configPhysicsDummySplit_"$run"_$i.dat
    sed -i "s|3990|$run|" data/physicsRunsDummySplit_"$run"_$i.dat
    sed -i "s|*.root|$i*.root|" data/physicsRunsDummySplit_"$run"_$i.dat
    sed -i "s|physicsRunsDummySplit|physicsRunsDummySplit_"$run"_$i|" test/configPhysicsDummySplit_"$run"_$i.dat
#    python python/Loop.py --input test/configPhysicsDummySplit_"$run"_$i.dat --debug 3
done