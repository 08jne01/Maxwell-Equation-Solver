# Maxwell-Equation-Solver

This is a Finite Difference Frequency Domain sovler for maxwells equations in an optical fiber/waveguide implemented in C++. This is an implementation of the paper: https://doi.org/10.1364/OE.10.000853.

Some example modes in different fibers https://imgur.com/a/CVyIGNq

**Results are not confirmed to be accurate yet, this is a WIP project!!**

Dependancies:
- SFML for rendering
- Eigen3 for sparse and dense matrices
- Spectra for sparse eigen solving

**Install and Compile(linux)**

*Root Access*

```git clone https://github.com/08jne01/Maxwell-Equation-Solver.git```

```sudo apt-get install libsfml-dev```

```git clone https://github.com/eigenteam/eigen-git-mirror.git```

```git clone https://github.com/yixuan/spectra.git```

```cd spectra/include/```

```sudo mv Spectra /usr/include/```

```cd ../..```

```cd eigen-git-mirror ```

```sudo mv Eigen /usr/include/```

```cd ..```

```cd Maxwell-Equation-Solver/Maxwell-Equation-Solver```

```g++ -Ofast -o max.out main.cpp Program.cpp Field.cpp FileHandler.cpp Sweep.cpp MaxwellSolver.cpp Clock.cpp FieldViewer.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread -fopenmp```

To then run the program just use: ```max.out``` followed by any of the commands listed in the commands section.

*Non-root Access*

```git clone https://github.com/08jne01/Maxwell-Equation-Solver.git```

```wget https://en.sfml-dev.org/files/SFML-2.5.1-linux-gcc-64-bit.tar.gz```

```tar -xvf SFML-2.5.1-linux-gcc-64-bit.tar.gz``` < check that it is the same file as the one you downloaded version may change.

You can get the latest SFML from https://en.sfml-dev.org/download.php

```cd Maxwell-Equation-Solver/Maxwell-Equation-Solver```

```rm -r lib```

```cd include```

```rm -r SFML```

```cd ../../..```

```cd SFML-2.5.1``` or whatever version you have

```mv lib <Your_Directory_YouDownloaded_Maxwell_Solver_To>/Maxwell-Equation-Solver/Maxwell-Equation-Solver/```

```cd include```

```mv SFML <Your_Directory_YouDownloaded_Maxwell_Solver_To>/Maxwell-Equation-Solver/Maxwell-Equation-Solver/include/```

```cd ..```

```cd Maxwell-Equation-Solver/Maxwell-Equation-Solver```

```g++ -Ofast -o max.out -I./include -L./lib -std=c++11 main.cpp Program.cpp Field.cpp FileHandler.cpp Sweep.cpp MaxwellSolver.cpp Clock.cpp FieldViewer.cpp -lsfml-graphics -lsfml-window -lsfml-system -lpthread -fopenmp```

To then run the program just use: ```export LD_LIBRARY_PATH=./lib && ./max.out``` followed by any of the commands listed in the commands section.

After the first run you can then use the ```./max.out``` alone to run it.

**Install (windows)**

To install just extract binary zip into a folder, be sure to keep the folders with the executable.

**Usage (windows)**

Fiber parameters can be edited in the Resources/config.ini file. Any lines with a proceeding # will be ignored by the parser so you can comment in these files. You can have multiple config files by just creating several config files of different names. You can use these by entering their filename when starting the program.

To run the program just execute the exe, press enter for default config file. The program will execute for the predetermined settings.

Once the program has converged a window will open with your modes.
- Escape will exit the program
- Up and Down will increase and decrease the current selected mode.
- Left and Right will change the current selected field.
- Space will display the imported fiber geometry.
- Enter will export the current mode and all fields to a file in Output_Data/Field_Components_Mode_[CURRENT SELECTED MODE].dat
- Escape will quit or choose fibre mode if sweeping.
- Backspace will end sweep early.

Sweeping is now in you can find all the sweep options in the config files. To sweep just run with the command -sweep. The sweep finds the next mode based on an overlap integral, sometimes the new modes don't contain the previous mode or are too different for the program to be confident it is the same mode. In this case the program will ask you which mode is best to select with a list of the confidence levels for each mode and the field viewer will open so you can see each mode and select the closest one to the one you are tracing.

Commands:
- ```-sweep``` sweep the fibre according to the config.
- ```-neff``` change the maximum refractive index for this run.
- ```-config``` change the config from the default one.

Geometry is imported by bitmap (More accurate methods will be added in future):
- Make sure your bitmap dimensions are divisible by 4.
- ~Make sure your number of points matches the dimensions of the image (160x160 image will require 160 points)~ You can now use rectangular windows and the grid size is chosen based on the image size.
- In your image black will be 1.0 refractive index and white will be the max refractive index set in the config.
