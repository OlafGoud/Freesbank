# Handleiding

## Aansluiten


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

**CORNER_DIVIDATION**  
Deze parameter geeft aan hoe ver het pad van de ideale scherpe hoek af mag wijken.  
- Kleiner -> scherpere hoeken, lagere snelheid, nauwkeuriger.
- Groter -> meer ronde hoeken, hogere snelheiden, minder nauwkeurig.

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
- $?, Haal de status op van de machine (Status|Positie|EindLocatie|Snelheid)
- $X, Zet de X positie van de encoder naar 0.
- $D, Debug

### G commands

**Snel rechte beweging**  
- G0, (x, y, z, f), Ga naar locatie, tool niet actief
> Parameters:  
> X: X positie in mm  
> Y: Y positie in mm  
> Z: Z positie in mm  
> F: Snelheid in (mm/min)

- G1, (x, y, z, f, e), Ga naar locatie, tool actief  
> Parameters:  
> X: X positie in mm  
> Y: Y positie in mm  
> Z: Z positie in mm  
> F: Snelheid in (mm/min)
> E: Kracht van tool (Snelheid van spindle, etc), 0 to 255

**Arc movement**  
- G2, (x, y, z, i, j, r), clockwise
- G3, (x, y, z, i, j, r), counter clockwise

### M commands  
- M1 -> stop instantly (not implmenented)