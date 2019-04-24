// player.ino - 23/04/2019

/*  
    Código responsável por receber os comandos enviados pelo
    controller via RF e atuar nas rodas do jogador de acordo. 
    
    Versão: 1.0

    Características da versão:
        - Recebe a um char via RF e de acordo com o seu valor
        deve atuar (ou não) nas rodinhas

    Colaboradores:
        Lyang Leme de Medeiros
    
*/

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define ESQ1 2  // D2 do nano ligado ao Pino 2 da ponte H
#define ESQ2 3  // D3 do nano ligado ao P 7 da ponte
#define DIR1 4  // D4 do nano ligado ao P 10 da ponte
#define DIR2 5  // D5 do nano ligado ao P 15

bool e1, e2, d1, d2;

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

void setup() 
{

  pinMode(ESQ1, OUTPUT);
  pinMode(ESQ2, OUTPUT);
  pinMode(DIR1, OUTPUT);
  pinMode(DIR2, OUTPUT);

  radio.begin();
  radio.openReadingPipe(0, address);
  radio.setPALevel(RF24_PA_MIN);
  radio.startListening();

  Serial.begin(115200);
 
}

void loop() 
{
  char rec;

  Serial.println("rec: ");
  if (radio.available()) {
    radio.read(&rec, sizeof(rec));
    Serial.println(rec);
    setWheels(rec);
  }

  digitalWrite(ESQ1, e1);
  digitalWrite(ESQ2, e2);
  digitalWrite(DIR1, d1);
  digitalWrite(DIR2, d2);
    
}

void setWheels(char rec)
{
  switch (rec) {
  case 'r':
    d1 = true;
    e1 = false;
    e2 = false;
    d2 = false;
    break;
  case 'e':
    e1 = true;
    d1 = false;
    d2 = false;
    e2 = false;
    break;
  default:
    d1 = false;
    d2 = false;
    e1 = false;
    e2 = true;
    break;
}
}
