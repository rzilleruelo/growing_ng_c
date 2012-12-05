growing_ng_c
============

This is a C++ implementation of the Growing Neural Gas clustering Algorithm.

Usage examples
./bin/growingng -t ../data/JapaneseVowels/aeNORM.train  2 0.2 0.002 100 0.0001 0.000000 15 10 10 out/ ae
./bin/growingng -t ../data/control_charts/synthetic_controlNorm.signal 2 0.3 0.1 100 0.01 1e-15 100 10 10 out/ control
./bin/growingng -q ../data/control_charts/synthetic_controlNorm.signal out/ control out/controlNormQuantized.txt

References:
http://web.cs.swarthmore.edu/~meeden/DevelopmentalRobotics/fritzke95.pdf
http://wwwold.ini.rub.de/VDM/research/gsn/JavaPaper/node19.html