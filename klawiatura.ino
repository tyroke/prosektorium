#include "Keypad.h"
#include <SoftwareSerial.h>
#include <Wire.h>
#include <ZX_Sensor.h>


#define PIN_1 22
#define led1 40
#define led2 41

//Arduino Pin  ZX Sensor Board  Function
//---------------------------------------
//5V           VCC              Power
//GND          GND              Ground
//20           DA               I2C Data
//21           CL               I2C Clock

long previousMillis = 0;
unsigned long currentMillis = 0;
long previousMillis2 = 0;
unsigned long currentMillis2 = 0;
long interval = 15000;
long miganie = 1000;
int ledState = LOW;

// Constants
const int ZX_ADDR = 0x10;    // ZX Sensor I2C address

// Global Variables
ZX_Sensor zx_sensor = ZX_Sensor(ZX_ADDR);
GestureType gesture;
char hand[] = {'2', '3', '2', '3', '2'};
int licznik_hand = 0;

SoftwareSerial Thermal(2, 3);

int heatTime = 80;
int heatInterval = 255;
char printDensity = 15;
char printBreakTime = 15;

const byte ROWS = 4; //four rows
const byte COLS = 4; //three columns
char password[] = {'1', '7', '9', '5', '3'};
int licznik = 0;
String x;
char keys[ROWS][COLS] = {
  {'1', '2', '3' , 'A'},
  {'4', '5', '6' , 'B'},
  {'7', '8', '9' , 'C'},
  {'*', '0', '#' , 'D'}
};
byte rowPins[ROWS] = {10, 11, 4, 5};  //od niebieskiego Piny, do których podłączamy wyprowadzenia od rzędów
byte colPins[COLS] = {6, 7, 8 , 9}; //Piny, do których kolumn wyprowadzenia od rzędów

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

void setup() {
  Serial.begin(9600);
  pinMode(22, INPUT);
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  Thermal.begin(19200); // to write to our new printer
  initPrinter();
  if ( zx_sensor.init() ) {
    Serial.println("ZX Sensor initialization complete");
  } else {
    Serial.println("Something went wrong during ZX Sensor init!");
  }
}

void initPrinter()
{
  //Modify the print speed and heat
  Thermal.write(27);
  Thermal.write(55);
  Thermal.write(7); //Default 64 dots = 8*('7'+1)
  Thermal.write(heatTime); //Default 80 or 800us
  Thermal.write(heatInterval); //Default 2 or 20us
  //Modify the print density and timeout
  Thermal.write(18);
  Thermal.write(35);
  int printSetting = (printDensity << 4) | printBreakTime;
  Thermal.write(printSetting); //Combination of printDensity and printBreakTime
  Serial.println();
  Serial.println("Printer ready");

}



void dol() {
  Thermal.write(10);
  Thermal.write(29);
  Thermal.write(33);
  Thermal.write(255);
  Thermal.println(" /\\    /\\    ||    /\\");
  Thermal.println("/||\\  /||\\   ||   /||\\");
  Thermal.println(" ||    ||    ||    ||");
  Thermal.println(" ||    ||   \\||/   ||");
  Thermal.println(" ||    ||    \\/    ||");
  Thermal.write(10);
}
void gora() {
  Thermal.write(10);
  Thermal.write(29);
  Thermal.write(33);
  Thermal.write(255);
  Thermal.println(" /\\    ||    ||    ||");
  Thermal.println("/||\\   ||    ||    ||");
  Thermal.println(" ||    ||    ||    ||");
  Thermal.println(" ||   \\||/  \\||/  \\||/");
  Thermal.println(" ||    \\/    \\/    \\/");
  Thermal.write(10);
}
void loop() {
  //  if (digitalRead(22)) {
  //    Thermal.write(10);
  //    Thermal.write(10);
  //    dol();
  //    gora();
  //    Thermal.write(10);
  //    Thermal.write(10);
  //    Thermal.write(10);
  //    Thermal.write(10);
  //  }

  if (Serial.available() == 0) {
  }
  else {
    x = Serial.readString();
    if (x == "klawiatura") {
      previousMillis = millis();
      currentMillis = millis();
      while (licznik < 5) {
        char key = keypad.getKey();
        currentMillis = millis();
        if (currentMillis - previousMillis > interval) {
          previousMillis = currentMillis;
          digitalWrite(led1, LOW);
          Serial.write("b");
          break;
        }
        if (currentMillis - previousMillis >= miganie) {
          previousMillis = currentMillis;
          if (ledState == LOW) {
            ledState = HIGH;
          } else {
            ledState = LOW;
          }
          digitalWrite(led2, ledState);
        }
        if (key) {
          if (key == password[licznik]) {
            licznik++;

          }
          else {
            licznik = 0;
          }
        }
      }
      if (licznik == 5) {
        digitalWrite(led1, LOW);
        Serial.write("1");
        licznik = 0;
      }
    }

    if (x == "odczyt1") {
      delay(2000);
      Serial.write("2");

    }
    if (x == "odczyt2") {
      delay(2000);
      Serial.write("3");
    }
    if (x == "koniec") {
      licznik_hand = 0;
      previousMillis2 = millis();
      currentMillis2 = millis();
      while (licznik_hand <= 4) {
        currentMillis2 = millis();
        if (currentMillis2 - previousMillis2 > interval) {
          previousMillis2 = currentMillis2;
          digitalWrite(led2, LOW);
          Serial.write("c");
          break;
        }
        if (currentMillis2 - previousMillis2 >= miganie) {
          previousMillis2 = currentMillis2;
          if (ledState == LOW) {
            ledState = HIGH;
          } else {
            ledState = LOW;
          }
          digitalWrite(led2, ledState);
        }
        if ( zx_sensor.gestureAvailable() ) {
          Serial.println(licznik_hand);
          gesture = zx_sensor.readGesture();
          switch ( gesture ) {
            case NO_GESTURE:

              break;
            case RIGHT_SWIPE:
              if (hand[licznik_hand] == '3') {
                licznik_hand++;
              }

              break;
            case LEFT_SWIPE:
              if (hand[licznik_hand] == '2') {
                licznik_hand++;
              }


              break;
            case UP_SWIPE:
              if (hand[licznik_hand] == '1') {
                licznik_hand++;
              }

              break;
            default:
              break;
          }
        }


      }
      if (licznik_hand > 4 ) {
        digitalWrite(led2, LOW);
        Thermal.write(10);
        Thermal.write(10);
        dol();
        gora();
        Thermal.write(10);
        Thermal.write(10);
        Thermal.write(10);
        Thermal.write(10);
        Serial.write("/");
        licznik_hand = 0;

      }

    }

    x = "";
    delay(10);
  }

}
