Source: https://howtomechatronics.com/tutorials/g-code-explained-list-of-most-important-g-code-commands/

## moves

### G-Code commands
G20 -> inches (feedrate in/min)
G21 -> mm (feedrate mm/min)
G00 -> move to point (G00 X## Y## (Z##))
G01 -> extrude straight line (G01 X## Y## (Z##) F200)
G02 -> clockwise circular motion (G02 X## Y## I## J##) -> (X, Y -> endlocation & I, J -> center point in relation to start point)
G03 -> counterclockwise circular motion (G03 X## Y## I## J##) -> (X, Y -> endlocation & I, J -> center point in relation to start point)
G17 -> select workingpane XY (alignt with ground) (default)
G18 -> select workingpane XZ
G19 -> select workingpane YZ
G28 -> go to refrence / home point
G28 X## Y## Z## -> go to refrence / home point trough point defined by XYZ to avoid collision
G28.1 X## Y## Z## -> Define homepoint
G90 -> absolute mode (take always absolute point from zero)
G91 -> relative mode (position in relation to last point) also called incremental mode


### M-Code commands
M00 -> Program stop
M02 -> end of program
M03 -> spindle on - clockwise
M04 -> spindle on - counterclockwise
M05 -> spindle stop
M06 -> tool change
M08 -> flood colant ON
M09 -> flood colant OFF
M30 - End of program

#### 3D printer commands
M104 -> Start extruder heating
M109 -> Wait unitl extruder reaches T0
M140 -> Start bed heating
M190 -> Wait until bed reaches T0
M106 -> Set fan speed

## Note
You can use commands like:
```G-code
G01 X5 Y7 F200
X10 Y15
X12 Y20
```
This is because the G01 stays until replaced by other command. it wil go from (0, 0) -> (5, 7) -> (10, 15) -> (12, 20). The feedrate (F##) is set once and stays so until changed. 



Schrijven & uploaden kan met: Universal Gcode Sender.



# list of letters
A: x-rotation in degree's
B: y-rotation in degree's
C: z-rotation -> **not suported**
D: 
E: Extrude amount between points -> **not suported**
F: Feedrate in mm/min
G: -> **reserved for G-code commands**
H: 
I: X space point to maintain constant distance from in mm
J: Y space point to maintain constant distance from in mm
K: Z space point to maintain constant distance from in mm
L:
M: -> **reserved for M-code commands**
N:
O:
P: time to wait in milliseconds.
Q:
R: Radius of arc -> **not suported**
S: Spindle power (RPM)
T:
U:
V:
W:
X: x-axis in mm
Y: y-axis in mm
Z: z-axis in mm




# GCode
G0: G0 (Xn, Yn, Zn, An, Bn, Fn) -> not all parameters must be used. **rapid movement**
G1: G1 (Xn, Yn, Zn, An, Bn, Fn, En, Sn) -> not all parameters must be used. **liniar movement**
G2: G2 (Xn, Yn, Zn, In, Jn, Kn, En, Fn, Sn, Rn) -> not all parameters must be used. **Clockwise Arc**
G3: G3 (Xn, Yn, Zn, In, Jn, Kn, En, Fn, Sn, Rn) -> not all parameters must be used. **Counter-Clockwise Arc**
G4: G4 (Pn) -> pause the machine. **Dwell**
G6: **Not suported**
G10: G10 (Xnnn)