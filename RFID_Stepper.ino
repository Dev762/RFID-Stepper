/*
* Read a card using a mfrc522 reader on your SPI interface
* Pin layout should be as follows (on Arduino Uno):
* MOSI: Pin 11 / ICSP-4
* MISO: Pin 12 / ICSP-1
* SCK: Pin 13 / ISCP-3
* SS/SDA: Pin 10
* RST: Pin 9
*/

#include <SPI.h>
#include <RFID.h>
#include <Stepper.h>

const int stepsPerRevolution = 20;  // change this to fit the number of steps per revolution
// for your motor
// initialize the stepper library on pins 8 through 11:
Stepper myStepper(stepsPerRevolution, 4, 5, 6, 7);

#define SS_PIN 10
#define RST_PIN 9

RFID rfid(SS_PIN,RST_PIN);

int led = 7;
int power = 8; 
int serNum[5];
int cards[2][5] = {
  {208,216,238,27,253},{61,27,16,82,100}  
};

bool access = false;

void setup(){

      // set the speed at 60 rpm:
  myStepper.setSpeed(200);
    Serial.begin(9600);
    SPI.begin();
    rfid.init();

    pinMode(led, OUTPUT);
    digitalWrite(led, LOW);
}

void loop(){
    
    if(rfid.isCard()){
    
        if(rfid.readCardSerial()){
            Serial.print(rfid.serNum[0]);
            Serial.print(" ");
            Serial.print(rfid.serNum[1]);
            Serial.print(" ");
            Serial.print(rfid.serNum[2]);
            Serial.print(" ");
            Serial.print(rfid.serNum[3]);
            Serial.print(" ");
            Serial.print(rfid.serNum[4]);
            Serial.println("");
            
            for(int x = 0; x < sizeof(cards); x++){
              for(int i = 0; i < sizeof(rfid.serNum); i++ ){
                  if(rfid.serNum[i] != cards[x][i]) {
                      access = false;
                      break;
                  } else {
                      access = true;
                  }
              }
              if(access) break;
            }
           
        }
        
       if(access)
      {
          Serial.println("Welcome!");
          for(int i = 0; i < 13; i++)
          {
            Serial.println("clockwise");
            myStepper.step(stepsPerRevolution);
           
          }

          for(int i = 0; i < 13; i++)
          {
            Serial.println("Anticlockwise");
            myStepper.step(-stepsPerRevolution);
           
          }
          
      } else { 
           Serial.println("Not allowed!");     
              }
    }   
    rfid.halt();
}
