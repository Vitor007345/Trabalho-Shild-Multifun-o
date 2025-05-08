/*
TRABALHO COM UTILIZAÇÃO DO SHILD (MFS)

INTEGRANTES:
VITOR ALBUQUERQUE DINELLI
RAFAEL ASSIS CARVALHO LACERDA
GABRIEL ASSIS CARVALHO LACERDA
EDUARDO DE SOUZA SILVA
*/


#include <TimerOne.h>                     // Bibliotec TimerOne 
#include <Wire.h>                         // Biblioteca Wire 
#include <MultiFuncShield.h>              // Biblioteca Multifunction shield]

#define _ECHO 5
#define _TRIGGER 6

double readUltrasonicDistance(int triggerPin, int echoPin){
  pinMode(triggerPin, OUTPUT);  // Clear the trigger
  digitalWrite(triggerPin, LOW);
  delayMicroseconds(2);
  // Sets the trigger pin to HIGH state for 10 microseconds
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(triggerPin, LOW);
  pinMode(echoPin, INPUT);
  // Reads the echo pin, and returns the sound wave travel time in microseconds
  return 0.01723 * pulseIn(echoPin, HIGH);
}


void setup() {
  Serial.begin(9600);
  Timer1.initialize();                    // inicializa o Timer 1
  MFS.initialize(&Timer1);                // initializa a biblioteca Multi função

}
bool ligado = true;
bool alarmeLigado = false;
void loop() {
  byte btn = MFS.getButton();
  if(btn){
    byte buttonNumber = btn & B00111111;                 // comparando os bits das portas analógicas
    byte buttonAction = btn & B11000000; 
    if(buttonNumber == 1 && buttonAction == BUTTON_PRESSED_IND){
      if(ligado){
        ligado = false;
      }else{
        ligado = true;
      }
    }

    
  }
  
  if(ligado){
      double distancia = readUltrasonicDistance(_TRIGGER, _ECHO);
      MFS.write(distancia, 2);
      if(distancia < 50){
        Serial.println("<50");
        MFS.writeLeds(LED_2, ON);
        if(distancia < 30){
          Serial.println("<30");
          MFS.writeLeds(LED_3, ON);
          if(distancia < 15){
            Serial.println("<15");
            MFS.writeLeds(LED_4, ON);
            alarmeLigado = true;
          }else{
            MFS.writeLeds(LED_4, OFF);
            alarmeLigado = false;
          }
        }else{
          MFS.writeLeds(LED_3, OFF);
        }
      }else{
        MFS.writeLeds(LED_2, OFF);
      }
      delay(100);
  }else{
    MFS.write("");
    MFS.writeLeds(LED_ALL, OFF);
    alarmeLigado = false;
  }

  if(alarmeLigado){
    MFS.beep();
  }


  
}
