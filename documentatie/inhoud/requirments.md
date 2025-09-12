# Probleemstelling
Een radar maakt gebruik van een antenne om het bereik te vergroten en om de data nauwkeuriger te maken. Er zijn verschillende soorten antennes waarvan een parabolische antenne er 1 is. deze antennes komen in verschillende grote en maten. Er zijn 'standaard' maten die al geproduceerd worden. Alleen als er een custom maat nodig is voor een radar word de prijs per antenne veel duurder ten opzichte van een 'standaard' maat. Daarom wilt Sens2Sea een manier vinden om die antennes een stuk goedkoper te maken. Er zijn al testen gedaan met het 3d printen van de onderdelen. Dit werkte goed uit. Alleen zijn er tijdens het bevestigen kansen dat de vorm veranderd word waardoor het parabolische effect van de antenne minder effectief werkt. Hiervoor willen ze een grotere 3d printer, cnc machine combinatie. Deze moet de vormen in 1 keer kunnen uitprinten met zo min mogelijk fouten in de vorm.


# Scope
De scope van de opdracht bevat het aansturen met code van de freesbank doormiddel van een vorm van grbl. Hier nog een sensor aan toevoegen om de exacte locatie van de kop te kunnen bepalen. (En simpele elektra voor de motoren aan te sturen.)
aansturing van de 4 assen. en de localisering scheiden van de aansturing


leverancier bepaalt de inhoud van de encoder. dus als je wat anders wilt doen dan is dat niet mogelijk. dus ben je afhankelijk van de leverancier.

# Requirements

## MUST

### (F) De kop kan over 3 assen bewegen.
De kop van de machine moet over 3 assen bewegen. Deze zijn: x, y, z.

**Acceptatie:**
De kop kan omhoog, omlaag, links, rechts, vooruit en achteruit bewegen. (x+, x-, y+, y-, z+, z-)
***
### (F) De kop kan materiaal toevoegen
De kop kan materiaal toevoegen dat word aangegeven.

**Acceptatie:**
Er kan materiaal toegevoegd worden aan de vorm. Dit materiaal moet aan de andere vorm hechten.
***
### (F) De kop kan materiaal verwijderen
De kop kan materiaal verwijderen.

**Acceptatie:**
Er kan materiaal verwijderd worden van de vorm.

***
### (F) De positie van de kop kan worden bepaald door sensoren
De positie van de kop kan bepaald worden door sensoren en worden teruggegeven aan een closed loop driver of de microcontroller

**Acceptatie:**
De locatie van de kop word gecorrigeerd als er een stap vergeten word of verspringt. 
***

### (F) Er kan een g-code bestand op de machine geladen worden en worden uitgevoerd
De machine kan een g-code bestand gestreamd krijgen en het bestand uitvoeren.

**Acceptatie:**
Er kan een g-code bestand op de machine gezet worden door een seriele verbinding en de machine volgt de instructies van dat bestand.
***

### (N) De machine gebruikt een aangepaste variant van grbl
De machine gebruikt een aangepaste variant van grbl voor het aansturen.

***



## SHOULD
## (F) De kop kan over 5 assen bewegen.
De kop van de machine moet over 5 assen kunnen bewegen. Dit zijn de x, y, z en ook een rotatie om de x as.

**Acceptatie:**
De machine kan over alle assen bewegen.
***


# Versies
10-9-2025   begin aan requirments + scope + versiebeheer toegevoegd