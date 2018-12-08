# Maxwell-Equation-Solver

This is a Finite Difference Frequency Domain sovler for maxwells equations in an optical fiber/waveguide implemented in C++. This is an implementation of the paper: https://doi.org/10.1364/OE.10.000853.

**Results are not confirmed to be accurate yet, this is a WIP project!!**

Dependancies:
- SFML for rendering
- Eigen3 for sparse and dense matrices
- Spectra for sparse eigen solving

**Install (linux)**

Not supported yet.

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

Geometry is imported by bitmap (More accurate methods will be added in future):
- Make sure your bitmap dimensions are divisible by 4.
- Make sure your number of points matches the dimensions of the image (160x160 image will require 160 points)
- In your image black will be 1.0 refractive index and white will be the max refractive index set in the config.
