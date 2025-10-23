incre - https://www.amazon.nl/Wisamic-Incrementale-draaigever-Incremental-ingangsspanningsbereik/dp/B015GYY7XU/ref=pd_day0_d_sccl_2_2/257-3496402-6638921?pd_rd_w=LSlp9&content-id=amzn1.sym.ed8bc3b8-5667-426c-8dfe-015eb9a717cf&pf_rd_p=ed8bc3b8-5667-426c-8dfe-015eb9a717cf&pf_rd_r=5D6DWHBV65H4G3K97YVN&pd_rd_wg=xGFAz&pd_rd_r=3d146bb0-2234-4d36-8ff4-0b38eca60d46&pd_rd_i=B015GYY7XU&psc=1


https://eu.robotshop.com/nl/products/roterende-encoder-600p-r-2-kanalen-6-mm-5v-npn

absolute: https://nl.mouser.com/ProductDetail/Bourns/EMS22A50-D28-LT6?qs=%2FxQVPCMPNzgB7Z6m6UP8xg%3D%3D&mgh=1


met round count: https://www.broadcom.com/products/motion-control-encoders/absolute-encoders/multi-turn-encoders/as20-m42m
https://nl.rs-online.com/web/p/motion-control-sensors/0227925?cm_mmc=aff-_-nl-_-netcomponents-_-227925


met pin togglen kan ik
?,399116348 interupts per seconde doen.

09:36:47.772 -> 848785
09:36:53.204 -> 1484065

```cpp
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


https://www.pololu.com/product/2133



