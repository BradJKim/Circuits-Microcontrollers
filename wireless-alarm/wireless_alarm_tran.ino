#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

// Define CE and CSN pins
#define CE_PIN 9
#define CSN_PIN 10
#define INPUT_PIN 8

// Create RF24 radio object
RF24 radio(CE_PIN, CSN_PIN);

// Define addresses for communication
const uint64_t address = 0xF0F0F0F0E1LL;

void setup() {
  Serial.begin(9600);

  SPI.begin();

  pinMode(INPUT_PIN, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  
  if (!radio.begin()) {
    Serial.println("Error in nRF module");
    while (1);
  }

  radio.setChannel(30);
  radio.setDataRate(RF24_250KBPS);
  radio.setPALevel(RF24_PA_LOW);
  radio.openWritingPipe(address);
  radio.setRetries(15, 15);
  radio.stopListening();
  Serial.println("nRF24L01+ initialized successfully");
}

void loop() {
    boolean isPressed = digitalRead(INPUT_PIN);
    
    if(isPressed) {
      
      int signal = 1;
      bool success = radio.write(&signal, sizeof(signal));

      if (success) {
        Serial.println("Message sent successfully!");
      } else {
        Serial.println("Message failed to send!");
      }

      delay(100);
    }
}
