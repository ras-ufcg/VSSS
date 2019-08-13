// core.ino - v0.1 - 13/08/2019 - Lyang Leme de Medeiros
/* =====================================================================
 * =====================================================================
 * 
 * Descrição:
 * 
 * Código que roda nos jogadores.
 * 
 * =====================================================================
 * 
 * Requisitos:
 * 
 * Receber e tratar os dados enviados pelo controlador via RF.
 * Calibrar PWM dos motores ao ligar
 * Controla a velocidade dos motores do jogador.
 * 
 * =====================================================================
 * 
 * Detalhes da implementação:
 * 
 * Na versão atual o código apenas realiza a captura de interupções
 * nas portas ligadas aos encoders dos motores para validar tal 
 * funcionalidade.
 * 
 * =====================================================================
 * 
 * =====================================================================
 */

 #define encL    2
 #define encR    3


 void leftPulseCount();
 void rightPulseCount();
 
 int pulseNumLeft;
 int pulseNumRight;

 int displayTime = 500;
 int multiplyFactor = 1000/displayTime;
 
 void setup()
 {
    Serial.begin(115200);

    pinMode(encL, INPUT);
    pinMode(encR, INPUT);

    attachInterrupt(digitalPinToInterrupt(encL), leftPulseCount, CHANGE);
    attachInterrupt(digitalPinToInterrupt(encR), rightPulseCount, CHANGE);
 }

void loop()
{
    delay(displayTime);
    
    Serial.print("RPM roda esquerda: ");
    Serial.println(pulseNumLeft*multiplyFactor);

    Serial.print("RPM roda direita: ");
    Serial.println(pulseNumRight*multiplyFactor);

    Serial.print("Diferença: ");
    Serial.println(abs(pulseNumLeft - pulseNumRight)*multiplyFactor);

   
    pulseNumLeft = 0;
    pulseNumRight = 0;
}

void leftPulseCount()
{
  pulseNumLeft++; 
}

void rightPulseCount()
{
  pulseNumRight++; 
}
