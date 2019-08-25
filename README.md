# Maxwell-Equation-Solver

This is a Finite Difference Frequency Domain sovler for maxwells equations in an optical fiber/waveguide implemented in C++. This is an implementation of the paper: https://doi.org/10.1364/OE.10.000853.

Some example modes in different fibers https://imgur.com/a/CVyIGNq

**Results are not confirmed to be accurate yet, this is a WIP project!!**

Dependancies:
- SFML for rendering
- Eigen3 for sparse and dense matrices
- Spectra for sparse eigen solving

**Install and Compile(linux)**

Locate the install shell script ```cd Maxwell-Equation-Solver```

Run the script with ```bash install.sh```

It will download required files and compile the binary for use.

To then run the program just use: ```./run.sh``` followed by any of the commands listed in the commands section.

**Install (windows)**

To install just extract binary zip into a folder, be sure to keep the folders with the executable.

**Usage**

Fiber parameters can be edited in the Config/config.ini file. Any lines with a proceeding # will be ignored by the parser so you can comment in these files. You can have multiple config files by just creating several config files of different names. You can use these by entering their filename when starting the program.

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
