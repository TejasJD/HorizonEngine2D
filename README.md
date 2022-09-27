# groupProject

Github repository for the Msc, ASD group project.

## Prerequisites
1. [Visual studio 2022](https://visualstudio.microsoft.com/) (Follow the *Setup for Development -> Windows* section
below. **Visual Studio build tools** and **CMake for Windows** can be installed through the process alongside the IDE
and can be primarily accessed through **Developer Command Prompt for VS 2022 / Developer Powershell for VS 2022**).

## Setup for Development
1. ### Windows
   1. Refer to the link for setting up [Visual Studio 2022 for C++ Development](https://learn.microsoft.com/en-us/cpp/build/vscpp-step-0-installation?view=msvc-170).
   2. Refer to the link for an introduction to [CMake projects in Visual Studio 2022](https://learn.microsoft.com/en-us/cpp/build/cmake-projects-in-visual-studio?view=msvc-170).
## Getting the project
1. ### Windows
   1. Run the following commands in the **command prompt / powershell** to get the project files on your local machine.
```
git clone https://github.com/alex-mulkerrins/groupProject.git
cd groupProject
git checkout API_Design
```
## Building the project
1. ### Windows
    1. #### Visual Studio
        1. Save the [CMakeLists.txt](CMakeLists.txt) file in this directory to configure and generate the project.
        2. Right-click on [CMakeLists.txt](CMakeLists.txt) and select the build option from the
dropdown to build the project.
        3. Select the appropriate startup item (HznApplication.exe) to run the client application.
![Win V S Run Project](README_images/WinVSRunProject.png)
    2. #### Visual Studio developer command prompt / powershell.
        1. Open either of the command line tools and navigate to the project directory.
        2. Run the following commands in **Developer Command Prompt for VS 2022 / Developer Powershell for VS 2022**. Two presets are available in
the [CMakePresets.json](CMakePresets.json) file. *x64-debug* and *x64-release*. The command below would configure and generate the project based on the preset
provided to the `--preset` option. Command: `cmake --preset x64-debug`
        3. Run the following command to build the project: `cd bin\build\x64-debug` and `cmake --build .`
        4. Run the following command to run the client application: `cd HznApplication\Debug` and then `HznApplication.exe` (or the following for powershell `cd HznApplication\Debug` and then `.\HznApplication.exe`).
        5. (Grouped up):
```
cmake --preset x64-debug
cd bin\build\x64-debug
cmake --build .
cd HznApplication\Debug
.\HznApplication.exe
```
