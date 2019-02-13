// Mapeamento de Hardware

#define A1 16
#define A2 5 
#define A4 0
#define A3 13

void setup() {
  // Define o pino 13 como saida
  pinMode(A1, OUTPUT);
  pinMode(A2, OUTPUT);
  pinMode(A3, OUTPUT);
  pinMode(A4, OUTPUT);
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
  delay(1000);
  digitalWrite(A1,0);
  digitalWrite(A2,1);
  digitalWrite(A3,0);
  digitalWrite(A4,1);
  delay(1000);
  digitalWrite(A1,0);
  digitalWrite(A2,0);
  digitalWrite(A3,0);
  digitalWrite(A4,0);
  delay(2);
}
