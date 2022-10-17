# Arduino Simulator Environment

This is a Simulator framework for generic CPU, and uses Arduino as a concrete instance

The project is divided as follows:

1. The Arduino main application (.ino file)
   - This should contain the main application code
   - The code should build a library containing loop() and setup() functions which will be called by the Simulator
   - Any other functionality is private to the module
   - User should include here any STD or USER Lib dependency
2. The simulator
   - The logic behind, glueing standard and user libraries, application flow, vizualization modes, real-world interface and analysis hooks
3. STD Libraries
   - common functions implementing digitalWrite(), pinMode(), analogWrite(), ......
4. User Libraries
   - Libraries defined by user, implementing complex modules like LCDs, LED matrix, logical devices, shift regs, large screens, RGB leds, sensors, ......
5. CUI/GUI Bindings
   - Bindings to support a various range of vizualization modes like: CUI, GUI, WEB, LOG, VR, ....
6. Analysis Hooks
   - Minimum support to enable extrnal analysis for soundness, profiling, code-coverage, performance, ....
7. I/O Proxy Layer
   - An electronic interface to the real-world which may include an Arduino device
8. Library Designer
   - A mechanism to design graphical peripheral libraries along with their functionalities
   - These libraries are user-libraries
   - Should be included in other projects


Components of the project:

1. Simulator
   - executes APP using a specific Board/CPU
   - the execution of the code may interract with other simulator libs and feedback providers
2. Board provider
   - Logic and specs of the CPU
   - PIN I/O specs
   - Peripheral specs
4. Visualization mode
   - CUI - full console
   - GUI - full GUI Windows
   - LOG - full LOGs
   - WEB - full Web view
   - VR  - virtual reality for simulation of mechanics
5. Libraries
   - logic and specs of the peripheral modules
6. Analysis layer
   - hooks for profiling and instrumentation
7. I/O Proxy
   - interaction of the simulation with the outside world I/O through a speciall crafted board


Targets:

1. Interaction between the simulator and real-world environments
2. Quick setup for code analysis scenarios in embedded world, without the need of real boards and debuggers
3. Possibilitty to develop custom libraries for any type of module or component and also describe how it should behave 
4. Posibillitty to generate infinite random problems and sollutions for student tests
5. Posibillitty to connect the simulator to a virtual/3D mechanical environment and also describe how mechanical components should interact with simulation

