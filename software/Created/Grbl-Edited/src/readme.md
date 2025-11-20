# function sequence;


Receiver -> reader -> 




mainLoop() -> readSerialLine() -> [GCODE?] executeGcodeLine() -> ExecuteMovementLine() -> planLine() -> Planbufferline() 


Wat wilt de stepper weten?
- Snelheid.
- eindpunt (vec3).


snelheid bepalen met encoder: 






Wat wil machine weten?


### Inputline reader
weten?
- input line

wat doen: 
- system commands filteren
- gcode commands maken.



### gcode reader 
weten?
- gcode command.





### Planner
weten?
- begin locatie
- eind locatie
- snelheid
- spindle

wat doen:
- begin & exit speed berekenen


#### Planner block inhoud
- distance
- feedrate/maxspeed
- spindle (aan/uit/snelheid)
- unit vecotor (n componenten)
- entry speed
- exit speed
- start point
- exit point



### Motor driver
weten?
- huidige Locatie (global??)
- eind locatie
- eind targetspeed
- Max snelheid
- max rem (global??)

wat doen:
- check of afremen? (current speed - rem > eind speed)
- steps zetten iedere keer.
- check of eindpunt (current loc +- 2xaccuracy == eindpos);
- spindle aansturen


- buffer 1space: werken aan die tot eindpunt en dan nieuwe pakken uit plannerbuffer.
#### Buffer inhoud
- eind locatie
- eind target speed
- max target speed
- spindle (aan/uit/snelheid).









stepper isr -> flag niewe nodig aan


-> input lezen
-> check flag stepper isr: aan ? nieuwe pakken : continue;


## Vars

### Macro's (exclusive states)
- BAUD 9600: baud rate tx/rx
- MAX_LINE_SIZE 40: lenght of a line;
- EMPTY_CHAR 0xff: empty character code

### Globals
- systemState: state of system (use SYSTEM states)
- stepperState: state of stepper (use STEPPER states)
- sensorState: state of sensor (use SENSOR states)

- stepperBuffer: 1 big, contains all info for 1 segment.

## States

### System
- IDLE 0
- RUNNING 1
- ERROR 2
- INTERNAL_ERROR_RESTART_REQUIRED 3

### STEPPER
- STEPPER_FULL 0
- STEPPER_EMPTY 1
- STEPPER_ERROR 2

### SENSOR
- SENSOR_OK
- SENSOR_ERROR




# GCODE commands

## (Fast) move straigt
- G0 (x,y,z,f)
- G1 (x,y,z,f,e)

## arc -> G17 G18 G19 for selecting plane (i=x, i=y planes) TODO
- G2 (x,y,z,i,j,r) clockwise
- G3 (x,y,z,i,j,r) counter clockwise

## absolute-relative TODO
- G90 -> relative
- G91 -> absolute



# Mcode commands

## stop
- M0 -> STOP after last movement (not needed) NO PARAMS
- M1 -> STOP NOW INSTANT executed when readed. (note full buffer can fuck this up) NO PARMS