# Handleiding Freesbank
Deze handleiding is voor het gebruik en aansluiten van de freesbank bij Sens2sea.

## Aansluiten
Het systeem bestaat uit de volgende hardware:
- Stepper motoren (2 voor X, 1 voor Y, 1 voor Z).
- Handfrees (werkt op netstroom).
- Encoders.
- Arduino mega microcrontroller.
- Freesbank.
- CNCshield.
- 4x DRV8824 stepper motor drivers.

**Let op:** Om deze freesbank aan te zetten is er ook een labvoeding nodig.  

Sluit het systeem aan zoals aangegeven op het elektrische schema.
Omdat er een CNCShield gebruikt wordt kan de hoge voltage naar de CNCShield en zijn er geen losse condensatoren nodig. Verder moet de handfrees op het netstroom en handmatig aangezet worden.


## Software.
De [software](/Freesbank/software/Created/Grbl-Edited/) kan worden geupload op de arduino mega. Hierna kunnen de commando's die ondersteund worden uitgevoerd worden. 

### Ondersteunde commando's
#### Systeem commandos
> $?: Krijg de huidige informatie. (state, huidige locatie, eindpunt beweging, beginpunt beweging).

#### G-Code
**Snelle rechte beweging (G0)**
Dit commando laat de machine snel naar de gekozen locatie gaan in een rechte lijn. Hierbij is de tool niet actief.
> Parameters:  
> X: X positie in mm  
> Y: Y positie in mm  
> Z: Z positie in mm  

**Snelle rechte beweging (G1)**
Dit commando laat de machine snel naar de gekozen locatie gaan in een rechte lijn. Hierbij is de tool wel actief.
> Parameters:  
> X: X positie in mm  
> Y: Y positie in mm  
> Z: Z positie in mm  
> F: Feedrate in mm/min (niet geimplementeerd.)

#### M-code
**Stop stop direct (M1)**
dit commando zet de state van de machine op error. Hierdoor gaat de machine niet verder. (restart nodig)

## Dev guide.

#### Hieronder zijn verschillende onderdelen die handig zijn voor developers van het systeem.
- Global variables.
- Datatypes.
- Assen veranderen / toevoegen
- Bitwise poorten aansturen.
- Interrupt timers.

### Globals.
Er zijn verschillende macros en globals om de code leesbaarder te maken.


## Macros & constanten & globals

### Globals
**systemState**
Systemstate houd de statemachine bij van het programma.
- IDLE (0) -> De machine wacht op instructies
- RUNNING (1) -> De machine is bezig met commando's uit te voeren.
- ERROR (2) -> Error in het systeem. Kan worden op gelost door het systeem
- INTERNAL_ERROR_RESTART_REQUIRED (3) -> Een error die het systeem zelf niet kan verhelpen. Hiervoor is ook een restart nodig

**stepperState**
Stepperstate houd de status van de stepper motoren bij. Deze hebben 1 plek in een buffer waarin een segment gezet word. 
- STEPPER_FULL (0) -> De stepperbuffer heeft inhoud. Deze wordt uitgevoerd.
- STEPPER_EMPTY (1) -> De stepperbuffer is leeg. Indien deze niet word aangevuld zal het systeem naar IDLE gaan.
- STEPPER_ERROR (2) -> Er is een fout opgetreden. Word gefixt door main loop en anders wordt de systemstate INTERNAL_ERROR_RESTART_REQUIRED.

**selectedPlane**
Is te vinden in de GCodeSettings struct. Houd bij op welke plane de machine werkt voor arc's.
- AXIS_PLANE_XY (0) -> XY plane
- AXIS_PLANE_XZ (1) -> XZ plane
- AXIS_PLANE_YZ (2) -> YZ plane

**SystemAccuracy**  
Een float met daarin de nauwkeurigheid in mm/stap

### Macros

**BAUD**
Baud is de snelheid waarmee de seriele verbinding praat. (Alleen 9600 suported)

**MAX_LINE_SIZE** 
Maxlinesize is de lengte van een lijn zonder spaties en commentaar (aangegeven met `()` en `;`) die kan worden uitgelezen via de seriele monitor. Als deze langer is word er een error gegeven.

#### Pins

**X_STEP_PIN**
Deze pin is de pin die de stappen uitvoert voor de motor op de x as.

**X_DIR_PIN**
Deze pin zet de richting voor de motor. 0 gaat deze naar -x en bij 1 gaat de motor naar +x.

#### Datatypes
Omdat de arduino zijn grote van een integer kleiner is dan die van andere computers zijn de standaard 16, 32 en 64 bit integers uit `<iostream>` niet correct. Hierom zijn er custom macro's. 
| iostream |grote (bit)| expanded           | custom | grote (bit) | expanded           |
| -------- | --------- | ------------------ | ------ | ----------- | ------------------ |
| -        | -         | -                  | uchar  | 8           | unsigned char      |
| uint8_t  | 8         | unsigned char      | uint8  | 8           | unsigned char      |
| int8_t   | 8         | char               | int8   | 8           | char               |
| uint16_t | 16        | unsigned short     | uint16 | 16          | unsigned int       |
| int16_t  | 16        | short              | int16  | 16          | int                |
| uint32_t | 32        | unsigned int       | uint32 | 32          | unsigned long int  |
| int32_t  | 32        | int                | int32  | 32          | long int           |
| uint64_t | 64        | unsigned long long | -      | -           | -                  |
| int64_t  | 64        | long long          | -      | -           | -                  |



## Supported commands

## $ Commands
- $X, Zet de X positie van de encoder naar 0.
- $D, Debug

### G commands


**Arc movement**  
- G2, (x, y, z, i, j, r), clockwise
- G3, (x, y, z, i, j, r), counter clockwise

### M commands  
- M1 -> stop instantly (not implmenented)



G90 absolute pos
G91 relative pos
G92 set current 0
F = feedrate
S = spindle speed, tempratuur extruder
M3 - 5 spindle (clocks, counter, off)
M0 pause program 
M2 end
M30 end en reset



## Timers
In dit programma zitten verschillende interrupts. Hieronder zijn Timers & hardware interrupts.


### Hardware interrupts

### Internal timer compare interrupts
Een arduino heeft meerdere timer compare interrupts die te gebruiken zijn. Een uno heeft Timer0 (8 bits), Timer1(16 bits) en Timer2 (8 bits). Deze pinnen worden ook gebruikt voor pwm outputs, dus veranderen van de configuration zal de pwm channels veranderen. Hiernaast wordt timer0 gebruikt voor de delay, millis en micros. Hier de configuraties van veranderen zal dit veranderen.



https://deepbluembedded.com/arduino-timers/
https://deepbluembedded.com/arduino-timer-calculator-code-generator/