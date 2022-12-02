#include <PS4Controller.h>
#include <ESP32Servo.h>
#include <analogWrite.h>
#include <ESP32Tone.h>
#include <ESP32PWM.h>


//config Servo para direccion
#define MINSG90 500
#define MAXSG90 2400
#define frecuencia 50
Servo servo;

//pines de Driver L293n
#define In1 18 
#define In2 19
//In1 es In1 y 2 en el driver, In 2 es 2 y 4
#define LuzD 32
#define LuzT 33
#define Guiño_L 25
#define Guiño_R 26
bool presionado = false;

void Luz(int led) {
presionado = !presionado;
delayMicroseconds(100);
if(presionado) {
  digitalWrite(led, HIGH);
}
else digitalWrite(led, LOW);

  Serial.println(presionado);
}

void direccion() {
  int CT3;
  CT3 = PS4.LStickX();
  int grados = map(CT3, -128, 128, 45, 135);
  Serial.printf("Left Stick x at %d\n", CT3);
  Serial.printf("servo grados:  %d\n", grados);
  servo.write(grados);
}

void foward() {
  digitalWrite(In1, HIGH);
  digitalWrite(In2, LOW);
}

void reverse() {
  digitalWrite(In2, HIGH);
  digitalWrite(In1, LOW);
}

void stop() {
  digitalWrite(In1, LOW);
  digitalWrite(In2, LOW);
}

void setup() {
  Serial.begin(115200);
  PS4.begin("1a:2b:3c:01:01:01");
  Serial.println("Ready.");
  
  pinMode(In1, OUTPUT);
  pinMode(In2, OUTPUT);
  pinMode(LuzD, OUTPUT);
  pinMode(13, OUTPUT);
  ESP32PWM::allocateTimer(0);
  ESP32PWM::allocateTimer(1);
  ESP32PWM::allocateTimer(2);
  ESP32PWM::allocateTimer(3);

  servo.setPeriodHertz(frecuencia);

  servo.attach(13, MINSG90, MAXSG90);
}

void loop() {
  if (PS4.isConnected()) {
    if (PS4.R2()) {
      foward();
    }
    if (PS4.L2()) {
      reverse();
      digitalWrite(LuzT, HIGH);
    }
    if (PS4.LStickX()) {
      direccion();
    }
    if (!PS4.Square()){
      Luz(32);
    }
      stop();
    
  }
}
