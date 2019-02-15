// Mapeamento de Hardware

#define A1 16 // D0 do nodemcu ligado ao Pino 2 da ponte H
#define A2 5  // D1 do nodemcu ligado ao P 7 da ponte
#define A3 4  // D2 do nodemcu ligado ao P 10 da ponte
#define A4 0  // D3 do nodemcu ligado ao P 15

#define LED 2 // Led de indicação 
void setup() {

  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
  pinMode(LED,OUTPUT);
  
}
void loop() {
  digitalWrite(A1,0);
  digitalWrite(A2,0);
  digitalWrite(A3,0);
  digitalWrite(A4,0);
  delay(2);
  digitalWrite(A1,1);
  digitalWrite(A2,0);
  digitalWrite(A3,1);
  digitalWrite(A4,0);
  digitalWrite(LED,1);
  delay(1000);
  digitalWrite(A1,0);
  digitalWrite(A2,0);
  digitalWrite(A3,0);
  digitalWrite(A4,0);
  delay(2);
  digitalWrite(A1,0);
  digitalWrite(A2,1);
  digitalWrite(A3,0);
  digitalWrite(A4,1);
  digitalWrite(LED,0);
  delay(1000);
  
}
