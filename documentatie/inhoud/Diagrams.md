# Systeem Diagrammen


## Machine
De machine is een hybride productie systeem. Dat betekend dat er zowel additieve en subtractieve processen uitgevoerd kunnen worden. Hierbij gaat het over 3D-printen en CNC-snijden. Deze machine heeft een paar invloeden van buiten het systeem. De gebruiker heeft invloed op het systeem door bepaalde commando's uit te voeren. Ook heeft de machine stroom nodig. Daarnaast kan de omgevings tempratuur ook een rol spelen voor bepaalde sensoren.  
![Enviroment](../bronnen/systemimages/Environment.png)  
*Diagram 1: invloeden uit de omgeving.*

De gebruiker kan een monteur, ontwikkelaar of iemand die iets wilt printen zijn. Want alle commando's gaan door dezelfde interace.

De omgevings tempratuur kan invloed hebben op gevoelige sensoren. Deze worden verder in de documentatie verwaarloost.

Er is stroom nodig voor de componenten. Hiervoor is een aansluiting naar het net nodig.

## Onderdelen
De machine bestaat uit hardware en software en de interfaces ertussen. 

### Connecties
Er zijn 2 verschillende soorten interfaces. interface connectie tussen de microcontroller en de gebruikers computer en de interface tussen de microcontroller en de hardware. Deze zijn te zien op Diagram 2.

### Hardware
De hardware bestaat uit actuatoren en sensoren. Sommige actuatoren worden aangestuurd doormiddel van een driver board. In z'n board komt dan ook de extra voltage die nodig is voor het onderdeel binnen.

![SystemDiagram](../bronnen/systemimages/SystemDiagram.png)
*Diagram 2: Hardware*