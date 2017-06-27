# cable_tester

Simple cable tester for checking CAT network cables. It can check wiring issues as well as cable length.

Wiring mismatch testing is based on voltage division.

Cable length measurement is based on signal reflection.

The microcontroller is a PIC18F45k50. This model is used because it has a CTMU unit that enables the 
microcontroller to measure nanosecond short times which is required for cable length measurement. 
The C program was written in MPLAB IDE.

Circuit was made in Eagle CAD circuit designer.

Master circuit schematic:

![Alt text](eagleCAD/master/circuit_schematic_master.png?raw=true "Circuit schematic")

Remote circuit schematic:

![Alt text](eagleCAD/remote/circuit_schematic_remote.png?raw=true "Circuit schematic")

![Alt text](eagleCAD/master/cable_tester_master.jpg?raw=true "Device")