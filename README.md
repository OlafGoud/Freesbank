# Freesbank

## Opdracht
In deze opdracht voor Sens2Sea word de aansturing van een freesbank gemaakt. Deze freesbank is nodig voor het bedrijf om een custom radar antenne te maken. Deze antennes moeten een parabool vorm hebben. Je kunt de antennes uitprinten in stukken maar daardoor kunnen er over grote afstanden kleine fouten ontstaan. Deze afwijkingen zijn slecht voor de nauwkeurigheid van de sensor. De freesbank heeft de volgende afmetingen in meter: 4 x 1 x 0.4. Freesmachines gebruiken steppermotoren. Door de stappen te tellen kun je weten waar de kop zit. Alleen kunnen de stappen een keer verspringen waardoor je een afwijking krijgt in de machine. Deze afwijking is niet wenselijk bij het printen van een nauwkeurige parabool vorm. Hiervoor is een sensor nodig om de exacte afstand te berekenen. Hiervoor word (een fork van) grbl gebruikt. de standaard versie kan niet doormiddel van sensoren de afstand bepalen en allen door stappen te tellen. Voor grbl word G-Code gebruikt


---------------------------------------------------
TODO: 3d printer?? 4e axis
---------------------------------------------------  





De freesbank word aangestuurd door een modificeerde versie van grbl


## Info
Geert Mosterdijk

## Contributors
- Olaf Goudriaan (1071349)

## potential sources
https://github.com/SourceRabbit/RabbitGRBL  
https://github.com/gnea/grbl/wiki   // https://lasergrbl.com/download/