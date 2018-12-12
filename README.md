# FreeRTOSCCTV

This project contains the various source files used to create the FreeRTOS CCTV. The 'hdl' folder contains hardware-related files that
will be used in the main Vivado environment. The 'src' folder contains the C code for the project, which will be imported into the
Xilinx SDK along with FreeRTOS. The below sections will walk you through downloading this project repository, explore the contents
of the hdl and src folders, and guide you through merging this source code with the code from FreeRTOS.

## Download Instructions

To download, you may either download directly from the project's page on Github, or you may clone this repository with Git.
To download from the webpage, simply click the green "Clone or download" button in the top-right corner of the project window, and
download as a ZIP file. You can extract this zip file anywhere you want on your computer.

If you want to clone this project with Git, simply navigate to the folder you want to clone the project into, and type 
'git clone git@github.com:gunit367/FreeRTOSCCTV.git' into your Terminal of choice.

## HDL Folder

The HDL folder contains all of the hardware-related files for the project. Each subfolder of the hdl folder contains VHDL code to describe a new module to
the Vivado environment. We will be importing each of these folders later, and then adding these modules to our project's block diagram. In addition, the hdl
folder contains a file called 'design_1_wrapper.v', which is a Verilog file that describes the top-level hardware interfaces of our project. Lastly, the hdl
folder contains a file called 'CCTY_Constraints.xdc', which is a Xilinx constraints file that tells Vivado which physical pins on the board are mapped to which pins
of our block diagram.

## SRC Folder

The src folder contains all of the extra C code that we added to FreeRTOS. To use these in your project, copy the entire contents of the src folder into
the folder containing the main.c file for FreeRTOS (this is the folder we will eventually import into the SDK). While the location of your FreeRTOS
source may vary depending on where you extracted it when downloaded, within the FreeRTOS filetree this folder's location was
"FreeRTOSv10.1.1\FreeRTOS\Demo\CORTEX_A9_Zynq_ZC702\RTOSDemo\src" when we completed this project. When doing this copy, we will replace the original
main.c from FreeRTOS, as well as add a few subfolders for each of the peripherals the project will use.