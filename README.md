# Stage Sens2Sea Frees Printer

## Context

Sens2Sea is een klein bedrijf dat radar systemen en radar video technologie ontwikkeld. Het bedrijf word geleid door Geert Mosterdijk als directeur. Deze technologie word gebruikt op zee om bewegende en stilstaande objecten te detecteren bij goed en slecht zicht. Ook kan deze radar techonologie olievlekken in water detecteren en de zeebodem inkaart brengen. 

Het bedrijf is bezig met pulsradar technologie. Hiervoor wilt deze parabool antennes gebruiken. Deze zijn makkelijk te verkrijgen in standaard maten. Alleen heeft Sens2Sea custom maten nodig. Deze zijn veel duurder dan de standaard maten antennes. Hierom willen ze zelf een machine bezitten om de radar te maken. Dit is een freesmachine die een werkruimte van 4m x 1m x 0.4m moet hebben. Daarvoor moet ik de code maken en electronica doen.

Er is opensource software zoals GRBL dat dit werk kan doen alleen is deze niet aangepast op wat de opdrachtgever wilt. Omdat de freesmachine over de afstand van 4 meter moet kunnen werken wilt Sens2Sea dat de aandrijving en locatie bepaling gescheiden word. Dit is niet het geval bij GRBL. Deze software bepaalt de positie van de kop op basis van de stappen die uitgevoerd worden te tellen bij de stappen motoren. Maar stepper motoren kunnen een stap verspringen. Hierdoor zou de vorm van een antenne verslechteren waardoor die minder effectief word. Hierom wilt Sens2Sea dat de positie van de kop door sensoren bepaald word.

## Info
### Project owner
Geert Mosterdijk  
[Website](https://sens2sea.com/)

### Contributors
- Olaf Goudriaan (1071349)

### GIT
https://github.com/OlafGoud/Freesbank (CLOSED)
#### Docu git: 
- \[docu\] changed readme / added file.md -> documentation
- \[fix\] file.h, fixed ..... -> bugfix
- \[feature\] file.h, added ..... -> feature add
- \[refact\] file.h -> changed code by refactoring
- \[del\] file.h, removed: ..... -> deleted feature

## sources
https://github.com/gnea/grbl/wiki | https://github.com/gnea/grbl
https://en.wikipedia.org/wiki/Radar#Radar_signal

//Port ** Arduino Pin Number ** pin designation
PA 0 ** 22 ** D22	
PA 1 ** 23 ** D23	
PA 2 ** 24 ** D24	
PA 3 ** 25 ** D25	
PA 4 ** 26 ** D26	
PA 5 ** 27 ** D27	
PA 6 ** 28 ** D28	
PA 7 ** 29 ** D29	

PB 0 ** 53 ** SPI_SS	
PB 1 ** 52 ** SPI_SCK	
PB 2 ** 51 ** SPI_MOSI	
PB 3 ** 50 ** SPI_MISO	
PB 4 ** 10 ** PWM10	
PB 5 ** 11 ** PWM11	
PB 6 ** 12 ** PWM12	
PB 7 ** 13 ** PWM13	

PC 0 ** 37 ** D37	
PC 1 ** 36 ** D36	
PC 2 ** 35 ** D35	
PC 3 ** 34 ** D34	
PC 4 ** 33 ** D33	
PC 5 ** 32 ** D32	
PC 6 ** 31 ** D31	
PC 7 ** 30 ** D30	

PD 0 ** 21 ** I2C_SCL	
PD 1 ** 20 ** I2C_SDA	
PD 2 ** 19 ** USART1_RX	
PD 3 ** 18 ** USART1_TX	
PD 7 ** 38 ** D38	

PE 0 ** 0 ** USART0_RX	
PE 1 ** 1 ** USART0_TX	
PE 3 ** 5 ** PWM5	
PE 4 ** 2 ** PWM2	
PE 5 ** 3 ** PWM3	

PF 0 ** 54 ** A0	
PF 1 ** 55 ** A1	
PF 2 ** 56 ** A2	
PF 3 ** 57 ** A3	
PF 4 ** 58 ** A4	
PF 5 ** 59 ** A5	
PF 6 ** 60 ** A6	
PF 7 ** 61 ** A7	

PG 0 ** 41 ** D41	
PG 1 ** 40 ** D40	
PG 2 ** 39 ** D39	
PG 5 ** 4 ** PWM4	

PH 0 ** 17 ** USART2_RX	
PH 1 ** 16 ** USART2_TX	
PH 3 ** 6 ** PWM6	
PH 4 ** 7 ** PWM7	
PH 5 ** 8 ** PWM8	
PH 6 ** 9 ** PWM9	

PJ 0 ** 15 ** USART3_RX	
PJ 1 ** 14 ** USART3_TX	

PK 0 ** 62 ** A8	
PK 1 ** 63 ** A9	
PK 2 ** 64 ** A10	
PK 3 ** 65 ** A11	
PK 4 ** 66 ** A12	
PK 5 ** 67 ** A13	
PK 6 ** 68 ** A14	
PK 7 ** 69 ** A15	

PL 0 ** 49 ** D49	
PL 1 ** 48 ** D48	
PL 2 ** 47 ** D47	
PL 3 ** 46 ** D46	
PL 4 ** 45 ** D45	
PL 5 ** 44 ** D44	
PL 6 ** 43 ** D43	
PL 7 ** 42 ** D42	


encoders
x: PD0 en PD1 (20 en 21)
y: PD2 en PD3 (18 en 19)
z: PE4 en PE4 (2 en 3)

step pins
X: PA1 (23)
Y: PA3 (25)
Z: PA5 (27)

dir pins
X: PA0 (22)
Y: PA2 (24)
Z: PA4 (26)
pinouts:
https://devboards.info/boards/Arduino-mega2560-rev3

  /* model refrence control */
		peter.vanderklugt@pkmarine.nl
