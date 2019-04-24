// controller.ino - 23/04/2019

/*  
    Código responsável por receber os comandos processados pela
    estratégia e enviar para os jogadores via RF através do módulo
    NRF24L01. 
    
    Versão: 1.0

    Características da versão:
        - Faz a leitura de dois push butons ligados nas portas
        D2 e D4.
        - Caso nenhum dos botões esteja acionado, o controller deve
        mandar o comando para o robô segir em linha reta.
        - Caso D2 esteja acionado, o controller deve eviar o comando
        para que o robô faça uma curva para a esquerda.
        - Caso D4 esteja acionado, o controller deve eviar o comando
        para que o robô faça uma curva para a direita.


    Colaboradores:
        Lyang Leme de Medeiros
    
*/

#define ESQ 2
#define DIR 4

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN

const byte address[6] = "00001";

bool esq, dir = false;

void setup()
{
    pinMode(ESQ, INPUT_PULLUP);
    pinMode(DIR, INPUT_PULLUP);

    Serial.begin(115200);

}

void loop()
{
    checkbuttons();

    if(!esq && !dir) Serial.println('r'); // Comando para o robô seguir reto
    if(esq) Serial.println('e'); // Comando para o robô fazer curva à esquerda
    if(dir) Serial.println('d'); // Advinha!

    delay(20);
}

void checkbuttons()
{
    esq = digitalRead(ESQ);
    dir = digitalRead(DIR);
}
