# Macros
Here, 3 different types of macros are given for different simulation scenarios:
- Discrete: Here, a pointlike source's position is defined, as well as the discrete energies to be simulated. 
- Nuclide: Similar to discrete, but instead define a radionuclide, which's decay is simulated, as well as it's secondary particles
- Volume: randomises postition of source within a defined (cylindrical) volume. Material and density of extended sample can be altered.


# Evaluation Scripts
A collection of skripts and Files needed for common evaluations, such as reading of measured calibration efficiency or evaluation of simulated efficienceis.
- Use corresponding Analyse_XX script for either discrete simulations or simulations using radio niclides. 
- Use FitEfficiency.C to plot evaluated measured efficiencies. Use Data in format of Measured_data.txt which is read out by script and fitted to Function (Ax)^B
- Use PlotVsSim.C to compare simulated and measured efficiencies through a Residual Plot approach. Uses measured data in the form of a cvs-file and simulated data through a root file provided by Analyse.C. Compares Efficiencies of the same Energy, no need to have the exact same Energies.


# Self Attenuation Scripts 
Skripts for determining self attenuation correction factors through simulating discrete energies of point sources corresponding to real measured calibration efficiencies and simulating randomly distributed point sources withing Volume of defined material. Use Korr.C script for comparison of those efficiency curves, through which correction factors are provided. Can be copied of the terminal in tabillar form.
Recommended steps: (using seperate folders for both simulations recommended)
-insert energies for which correction factors should be determined in energies arrey in both Analyze skripts and macros. (macros each with the same formatting)
-Simulate discrete energy point source usinig point-macro in point folder
-Simulate discrete energy in volume using volume-macro in volume folder
-evaluate each by executing Analyse.C
-execute Korr.C
