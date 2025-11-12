# Probleemstelling
Een radar maakt gebruik van een antenne om het bereik te vergroten en om de data nauwkeuriger te maken. Er zijn verschillende soorten antennes waarvan een parabolische antenne er 1 is. deze antennes komen in verschillende grote en maten. Er zijn 'standaard' maten die al geproduceerd worden. Alleen als er een custom maat nodig is voor een radar word de prijs per antenne veel duurder ten opzichte van een 'standaard' maat. Daarom wilt Sens2Sea een manier vinden om die antennes een stuk goedkoper te maken. Er zijn al testen gedaan met het 3d printen van de onderdelen. Dit werkte goed uit. Alleen zijn er tijdens het bevestigen kansen dat de vorm veranderd word waardoor het parabolische effect van de antenne minder effectief werkt. Hiervoor willen ze een grotere 3d printer, cnc machine combinatie. Deze moet de vormen in 1 keer kunnen uitprinten met zo min mogelijk fouten in de vorm.


# Scope
De scope van de opdracht bevat het aansturen met code van de freesbank doormiddel van een vorm van grbl. Hier nog een sensor aan toevoegen om de exacte locatie van de kop te kunnen bepalen. (En simpele elektra voor de motoren aan te sturen.)
aansturing van de 4 assen. en de localisering scheiden van de aansturing.

# Requirements

## MUST

### (F) De kop kan over 3 assen bewegen.
De kop van de machine moet over 3 assen bewegen. Deze zijn: x, y, z.

**Acceptatie:**
De kop kan omhoog, omlaag, links, rechts, vooruit en achteruit bewegen. (x+, x-, y+, y-, z+, z-)

**Taken**
[x] Test de motoren + drivers.
[ ] Sluit de motoren aan op de machine.
[ ] Maak code om de assen te bewegen.
[ ] Maak testplan.
[ ] Doe testen en zet de resultaten in testplan.
[ ] Demo video.
[ ] Aanbevelingen noteren.
***
### (F) De kop kan materiaal toevoegen
De kop kan materiaal toevoegen dat word aangegeven.

**Acceptatie:**
Er kan materiaal toegevoegd worden aan de vorm. Dit materiaal moet aan de andere vorm hechten.

**Taken**
[x] Zoek uit hoe 3d printen werkt.
[ ] Zoek uit welke 3d print kop het beste is voor de toepassing.
[ ] Maak testplan.
[ ] Maak code om de kop aan te sturen.
[ ] Doe testen en zet de resultaten in testplan.
[ ] Demo video.
[ ] Aanbevelingen noteren.
***

### (F) De kop kan materiaal verwijderen
De kop kan materiaal verwijderen.

**Acceptatie:**
Er kan materiaal verwijderd worden van de vorm.

**Taken**
[x] Zoek uit hoe CNC machines werken.
[ ] Zoek uit welke snijkop het beste is voor de toepassing.
[ ] Maak testplan.
[ ] Maak code om de kop aan te sturen.
[ ] Doe testen en zet de resultaten in testplan.
[ ] Demo video.
[ ] Aanbevelingen noteren.
***

### (F) De positie van de kop kan worden bepaald door sensoren
De positie van de kop van de machine moet bepaald kunnen worden door sensoren.

**Acceptatie:**
De positie van de kop van de machine moet op 5mm nauwkeurig gemeten worden.

**Taken**
[x] Zoek uit welke sensoren gebruikt gaan worden.
[ ] Maak testplan.
[x] Maak code om de de sensoren uit te lezen.
[ ] Doe testen en zet de resultaten in testplan.
[ ] Demo video.
[ ] Aanbevelingen noteren.
***

### (F) Er kan een g-code bestand op de machine geladen worden en worden uitgevoerd
De machine kan een g-code bestand gestreamd krijgen en het bestand uitvoeren.

**Acceptatie:**
Er kan een g-code bestand op de machine gezet worden door een seriele verbinding en de machine volgt de instructies van dat bestand.

**Taken**
[x] Zoek uit welk wat g-code is en hoe het werkt.
[x] Maak code om de g-code te kunnen uitlezen van de seriele buffer en te kunnen gebruiken.
[ ] Maak testplan.
[ ] Doe testen en zet de resultaten in testplan.
[ ] Demo video.
[ ] Aanbevelingen noteren.
***


### (N) De code moet doormiddel van interfaces werken
De code van de machine gebruikt interfaces tussen de verschillende functionaliteiten om componenten makkelijk te kunnen veranderen.

**Acceptatie:**
Er kan een onderdeel veranderd worden, nieuwe code voor dat onderdeel geschreven worden die gebruikt maakt van de interface zonder de hele code om te bouwen.

**Taken**
[x] Maak architectuur
[x/] Maak de core code.
[ ] Maak testplan.
[ ] Maak code voor alle losse onderdelen.
[ ] Laat de core gebruik maken van de lossen onderdelen.
[ ] Aanbevelingen noteren.
***


## SHOULD
## (F) De kop kan over twee rotatieassen bewegen.
De kop van de machine moet over twee rotatieassen kunnen bewegen. Dit zijn de rotaties om de x en y assen.

**Acceptatie:**
De machine kan over twee rotatieassen bewegen.

**Taken**
[ ] Test de motoren + drivers voor rotatie.
[ ] Zoek de wiskunde voor de rotatie uit.
[ ] Sluit de motoren aan op de machine.
[ ] Maak testplan.
[ ] Maak code om de rotaties te bewegen.
[ ] Doe testen en zet de resultaten in testplan.
[ ] Demo video.
[ ] Aanbevelingen noteren.
***


# Versies
10-09-2025  begin aan requirments + scope + versiebeheer toegevoegd
16-09-2025  Taken toegevoegd, SHOUD: 5 assen bewegen -> 2 rotatie assen bewegen.