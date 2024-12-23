#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

#define CE_PIN   9
#define CSN_PIN 10
#define CONTROL_PIN 4

const byte address[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

char dataReceived[4]; 
bool newData = false;
bool isMotorOn = false;

void setup() {
  pinMode(CONTROL_PIN, OUTPUT);

  Serial.begin(9600);
  printf_begin();

  radio.begin();
  radio.setDataRate( RF24_1MBPS );
  radio.openReadingPipe(1, address);
  radio.startListening();

  radio.printDetails();
}

void loop() {
    getData();
    showData();

    if(dataReceived[0] == '1') 
      isMotorOn = true;
    else 
      isMotorOn = false;
      
    toggleMotor(isMotorOn);

    delay(500);
}

void toggleMotor(bool toggle) {
  if(toggle) {
    digitalWrite(CONTROL_PIN, HIGH);
  } else {
    digitalWrite(CONTROL_PIN, LOW);
  }
}

void getData() {
  if ( radio.available() ) {
    radio.read( &dataReceived, sizeof(dataReceived) );
    newData = true;
  }
}

void showData() {
  if (newData == true) {
    Serial.print("Data received ");
    Serial.println(dataReceived);
    newData = false;
  }
}