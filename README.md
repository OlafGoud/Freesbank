# Stage Sens2Sea Frees Printer

## Context

Sens2Sea is een klein bedrijf dat radar systemen en radar video technologie ontwikkeld. Het bedrijf word geleid door Geert Mosterdijk als directeur. Deze technologie word gebruikt op zee om bewegende en stilstaande objecten te detecteren bij goed en slecht zicht. Ook kan deze radar techonologie olievlekken in water detecteren en de zeebodem inkaart brengen. 

Het bedrijf is bezig met pulsradar technologie. Hiervoor wilt deze parabool antennes gebruiken. Deze zijn makkelijk te verkrijgen in standaard maten. Alleen heeft Sens2Sea custom maten nodig. Deze zijn veel duurder dan de standaard maten antennes. Hierom willen ze zelf een machine bezitten om de radar te maken. Dit is een freesmachine die een werkruimte van 4m x 1m x 0.4m moet hebben. Daarvoor moet ik de code maken en electronica doen.

Er is opensource software zoals GRBL dat dit werk kan doen alleen is deze niet aangepast op wat de opdrachtgever wilt. Omdat de freesmachine over de afstand van 4 meter moet kunnen werken wilt Sens2Sea dat de aandrijving en locatie bepaling gescheiden word. Dit is niet het geval bij GRBL. Deze software bepaalt de positie van de kop op basis van de stappen die uitgevoerd worden te tellen bij de stappen motoren. Maar stepper motoren kunnen een stap verspringen. Hierdoor zou de vorm van een antenne verslechteren waardoor die minder effectief word. Hierom wilt Sens2Sea dat de positie van de kop door sensoren bepaald word.


## Documentatie
- [Sensoronderzoek PDF](./documentatie/inhoud/sensor.pdf)
- [requirements PDF](./documentatie/inhoud/requirments.pdf)
- [aantekeningen](./documentatie/inhoud/aantekeningen.md)
- [g-code-onderzoek](./documentatie/inhoud/g-code-onderzoek.md)
- [grbl-onderzoek](./documentatie/inhoud/grbl-onderzoek.md)
- []
- [electric sceme testopstelling](./documentatie/bronnen/CNC3DPrinter-electricsceme-testopstelling.pdf)

### Other Documents
- [PVA PDF](./documentatie/school/planvanaanpak.pdf)  
- [stakeholder PDF](./documentatie/school/stakeholder.pdf)
- [risico-iventarisatie PDF](./documentatie/inhoud/risico-iventarisatie.pdf)


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
https://docs.google.com/spreadsheets/d/1IN747rUMbjGNzZw-ie91jJ9ETKRJDh7nsuLoZcZte4w/edit?gid=0#gid=0