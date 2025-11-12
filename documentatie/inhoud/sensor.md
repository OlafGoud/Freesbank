# Sensor Onderzoek
## Context
De opdrachtgever wilt dat de machine niet af gaat op de stappen die geteld zijn om te weten waar de kop van de freesbank zich bevind. Dit omdat een stappermotor weleens van stap verspringt. Na wat online zoeken is er gebleken dat er 'closed loop' motoren zijn die een encoder ingebouwd hebben. Hierdoor zijn de stappen die geteld zijn door het programma wel kloppend. Dit heb is voorgelegd aan de opdrachtgever. Maar dit was niet de oplossing omdat de de machine dan afhankelijk is van de leverancier met welke encoder die gaat. Hierdoor bepaald de leverancier de inhoud van de encoder. dus als je wat anders wilt doen dan is dat niet mogelijk. dus ben je afhankelijk van de leverancier. Uit het gesprek bleek dat de opdrachtgever wel een encoder wilt gebruiken, maar als die meer nauwkeurigheid nodig heeft hij wilt kunnen overschakelen naar een glaslineaal of iets anders. Het moet modulair zijn. Voor de proof of concept wilde de opdrachtgever een encoder gebruiken.

## Soorten Encoders
Er zijn drie soorten encoders. De incrementele encoder geeft een puls iedere keer als deze een stukje draait. Een absolute encoder onthoud waar deze zich bevind in het rondje. Er zijn ook absolute encoders die ook bijhouden op welke rotatie deze zijn. Hiernaast is er ook een lineare encoder. Bij deze encoder gaat de sensor langs een coderings schaal.

### Incrementeel
Incrementele encoders zijn encoders die door een schijf met gaatjes bepalen of de as is verschoven. Deze encoders nauwkeurigheid word afgedrukt in PPR (pulses per rotation). Door de pulses te tellen die uit de outputs komen kun je de snelheid en locatie bepalen. Er zijn incrementele encoders die 2 output poorten hebben. Deze kunnen in 4x resolutie werken. Dit komt omdat de gaten in de schijf op met een verschuiving van de fases staan. hierdoor heb kun je door gebruik van de 2 poorten je resolutie verviervoudigen.

| A | B |
| - | - |
| 0 | 0 |
| 1 | 0 |
| 1 | 1 |
| 0 | 1 | 

Hierboven zijn alle verschillende mogelijkheden te zien als een encoder draait op deze manier. Door te kijken welke fase de vorige was kun je met deze encoder ook de richting bepalen.

#### Interupts
Omdat deze encoders met 2 signalen werken die per ronde x aantal keer afgeven is het handig om deze op interupts te zetten. Maar omdat dit best veel interupts zijn is de vraag of de arduino dat aan kan.

Hiervoor heb ik code gemaakt om dat te testen. Deze code gebruikt pin toggelen op de B Port van de arduino om zo snel mogelijk de pin te switchen. Hier 399116348 interupts per seconde doen.

09:36:47.772 -> 848785
09:36:53.204 -> 1484065

``` cpp
volatile long int count = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(2), addCount, CHANGE);
  Serial.begin(115200);
  Serial.println("available");
  pinMode(13, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  if(Serial.available()) {
    noInterrupts();
    if(Serial.read() == '?') {
      Serial.println(count);
    }
    interrupts();
  }

  PINB = (1 << PB5);    
}

void addCount() {
  count++;
}
```

### Absolute
Een absolute encoder geeft een unike code voor elke hoek van de as. Deze zijn er mechanisch, magnetisch en optisch. Door de as in bepaalde manieren te encoden weet de encoder tussen welke hoeken hij zich zal bevinden. Deze encoders moeten uitgelezen worden via een protocol. Er zijn ook Absolute encoders die de aantallen revoluties bijhouden.

### Liniear
Linieare encoders zijn net als incrementele encoders maar in plaats van dat de gaatjes op een schijf zit, zitten deze op de zijkant waarna de leeskop daarlangs beweegt. Deze zijn minder flexibel dan andere encoders omdat deze gemaakt worden voor een vooraf gedefineerde afstand.


## Conclusie
Na voorgelegd te hebben aan de opdrachtgever is er besloten om een incrementele encoder te gebruiken. Dit omdat deze meer vrijheid bieden dan de andere encoders.




# Bronnen 
- https://en.wikipedia.org/wiki/Incremental_encoder
- https://en.wikipedia.org/wiki/Rotary_encoder#Absolute_rotary_encoder

## Encoder opties: 
### incrementeel
- https://www.amazon.nl/Wisamic-Incrementale-draaigever-Incremental-ingangsspanningsbereik/dp/B015GYY7XU/ref=pd_day0_d_sccl_2_2/257-3496402-6638921?pd_rd_w=LSlp9&content-id=amzn1.sym.ed8bc3b8-5667-426c-8dfe-015eb9a717cf&pf_rd_p=ed8bc3b8-5667-426c-8dfe-015eb9a717cf&pf_rd_r=5D6DWHBV65H4G3K97YVN&pd_rd_wg=xGFAz&pd_rd_r=3d146bb0-2234-4d36-8ff4-0b38eca60d46&pd_rd_i=B015GYY7XU&psc=1
- https://www.pololu.com/product/2133
- https://eu.robotshop.com/nl/products/roterende-encoder-600p-r-2-kanalen-6-mm-5v-npn

### Absolute
- https://nl.mouser.com/ProductDetail/Bourns/EMS22A50-D28-LT6?qs=%2FxQVPCMPNzgB7Z6m6UP8xg%3D%3D&mgh=1

#### Absolute met ronde teller counter
- https://www.broadcom.com/products/motion-control-encoders/absolute-encoders/multi-turn-encoders/as20-m42m
- https://nl.rs-online.com/web/p/motion-control-sensors/0227925?cm_mmc=aff-_-nl-_-netcomponents-_-227925


# Versies
12-11-2025  Aantekeningen netjes gemaakt en verwerkt tot onderzoek.
