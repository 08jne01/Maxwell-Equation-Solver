#!/bin/bash
SFML_VERSION=SFML-2.5.1

echo "Downloading..."
wget https://en.sfml-dev.org/files/$SFML_VERSION-linux-gcc-64-bit.tar.gz
tar -xvf $SFML_VERSION-linux-gcc-64-bit.tar.gz
rm -r Maxwell-Equation-Solver/lib
rm -r Maxwell-Equation-Solver/include/SFML
mv $SFML_VERSION/lib Maxwell-Equation-Solver/
mv $SFML_VERSION/include/SFML Maxwell-Equation-Solver/include/
rm -r $SFML_VERSION
rm $SFML_VERSION-linux-gcc-64-bit.tar.gz
cd Maxwell-Equation-Solver
echo "Compiling..."
g++ -Ofast -o max.out -I./include -L./lib -std=c++11 main.cpp Program.cpp Field.cpp FileHandler.cpp Sweep.cpp MaxwellSolver.cpp Clock.cpp FieldViewer.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread -fopenmp
echo "Done!"
echo "Use run.sh to run the program!"