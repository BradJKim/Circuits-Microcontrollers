#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <printf.h>

#define CE_PIN   9
#define CSN_PIN 10
#define BUTTON_INPUT 4
#define LED_OUTPUT 2

const byte slaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN); 

char dataToSend[4];

unsigned long currentMillis;
unsigned long prevMillis;
unsigned long txIntervalMillis = 1000;

void setup() {
  pinMode(BUTTON_INPUT, INPUT);
  pinMode(LED_OUTPUT, OUTPUT);

  Serial.begin(9600);
  printf_begin();

  radio.begin();
  radio.setDataRate( RF24_1MBPS );
  radio.setRetries(3,5); 
  radio.openWritingPipe(slaveAddress);
  radio.stopListening();

  radio.printDetails();
}

void loop() {
  currentMillis = millis();

  bool isPressed = digitalRead(BUTTON_INPUT);
  if(isPressed) 
    dataToSend[0] = '1';
  else
    dataToSend[0] = '0';

  if (currentMillis - prevMillis >= txIntervalMillis) {
      send();
      prevMillis = millis();
  }
}

void send() {
  digitalWrite(LED_OUTPUT, HIGH);

  bool rslt;
  rslt = radio.write( &dataToSend, sizeof(dataToSend) );

  Serial.print("Data Sent ");
  Serial.print(dataToSend);
  if (rslt) {
      Serial.println("  Acknowledge received");
  }
  else {
      Serial.println("  Tx failed");
  }

  delay(250);
  digitalWrite(LED_OUTPUT, LOW);
}

