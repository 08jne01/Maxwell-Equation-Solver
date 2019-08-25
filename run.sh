#!/bin/bash
cd Maxwell-Equation-Solver/Maxwell-Equation-Solver/
export LD_LIBRARY_PATH=./lib && ./max.out "$@"