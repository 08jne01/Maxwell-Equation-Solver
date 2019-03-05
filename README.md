# Maxwell-Equation-Solver

This is a Finite Difference Frequency Domain sovler for maxwells equations in an optical fiber/waveguide implemented in C++. This is an implementation of the paper: https://doi.org/10.1364/OE.10.000853.

Some example modes in different fibers https://imgur.com/a/CVyIGNq

**Results are not confirmed to be accurate yet, this is a WIP project!!**

Dependancies:
- SFML for rendering
- Eigen3 for sparse and dense matrices
- Spectra for sparse eigen solving

**Install and Compile(linux)**

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

```g++ -o max.out main.cpp Program.cpp Field.cpp FileHandler.cpp Sweep.cpp MaxwellSolver.cpp Clock.cpp -lsfml-graphics -lsfml-window -lsfml-system -pthread```



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

Commands:
- ```-sweep``` sweep the fibre according to the config.
- ```-neff``` change the maximum refractive index for this run.
- ```-config``` change the config from the default one.

Geometry is imported by bitmap (More accurate methods will be added in future):
- Make sure your bitmap dimensions are divisible by 4.
- Make sure your number of points matches the dimensions of the image (160x160 image will require 160 points)
- In your image black will be 1.0 refractive index and white will be the max refractive index set in the config.
