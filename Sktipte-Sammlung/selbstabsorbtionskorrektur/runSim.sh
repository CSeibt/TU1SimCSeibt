cd point
./TU1 -m runIsotopes-Discrete.mac

cd ../volume
./TU1 -m runIsotopes-Volume.mac

cd ..
root -l -q Korr.C
