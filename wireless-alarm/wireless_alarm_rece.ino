#include <Wire.h>
#include <SPI.h>
#include <nRF24L01.h> 
#include <RF24.h>      
#include <LiquidCrystal_I2C.h> 

#define CE_PIN 13
#define CSN_PIN 14
#define SCK_PIN   6
#define MISO_PIN  4
#define MOSI_PIN  7

const int startPin = 21;
const int resetPin = 20;
const int addSecondPin = 19; 
const int removeSecondPin = 18;
const int audioPin = 22; 

LiquidCrystal_I2C lcd(0x27, 16, 2);
RF24 radio(CE_PIN, CSN_PIN);

const uint64_t address = 0xF0F0F0F0E1LL;

int receivedData = 0;

bool isPaused = true;
bool isCounting = false;
bool isIdle = true;

int startCoolTime = 0;
int resetCoolTime = 0;
int incCoolTime = 0;
int decCoolTime = 0;

int totalTime = 20000;
int currentTime;
int startTime = 0;
int pausedTimeStart = 0;
int pausedTime = 0;
int elastedTime;

int displayTime;

void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  
  Serial.begin(9600);

  SPI.setSCK(SCK_PIN);
  SPI.setMISO(MISO_PIN);
  SPI.setMOSI(MOSI_PIN);

  pinMode(audioPin, OUTPUT);
  pinMode(startPin, INPUT);
  pinMode(resetPin, INPUT);
  pinMode(addSecondPin, INPUT);
  pinMode(removeSecondPin, INPUT); 

  Wire.setSDA(16);
  Wire.setSCL(17); 
  Wire.begin();   

  lcd.begin(16, 2);
  lcd.backlight();
  lcd.print("Timer");

  if (!radio.begin()) {
    Serial.println("Error in nRF module");
  } else {
    radio.setChannel(30);                                  
    radio.setDataRate(RF24_250KBPS);                   
    radio.setPALevel(RF24_PA_LOW);                 
    radio.openReadingPipe(1, address);            
    radio.startListening(); 
    Serial.println("nRF24L01+ initialized successfully and listening");
  } 

  Serial.println("Setup done");

}

void loop() {
  digitalWrite(LED_BUILTIN, HIGH);

  boolean startStop = digitalRead(startPin);
  boolean reset = digitalRead(resetPin);
  boolean addSecond = digitalRead(addSecondPin);
  boolean removeSecond = digitalRead(removeSecondPin); 

  currentTime = millis();

  if(startStop) {
    startStopTimer();
  }
  if(reset) {
    resetTimer();
  }
  if(addSecond) {
    incTimer();
  }
  if(removeSecond) {
    decTimer();
  } 

  if(isCounting) {
    if(!isPaused) {
      elastedTime = currentTime - startTime;
    }
    
    int remainingTime = totalTime - (elastedTime);
    displayTime = remainingTime / 1000;
    
  } 
  else if(isIdle) {
    displayTime = totalTime / 1000;
  }
 

  printTime(displayTime);

  if(displayTime == 0) {   
    reset = digitalRead(resetPin);

    while(!reset) {
      if(radio.available()) {
        radio.read(&receivedData, sizeof(receivedData));
        if(receivedData == 1) {
          reset = 1;
        }
      }

      digitalWrite(audioPin, HIGH);
      delay(250);
      digitalWrite(audioPin, LOW);
      delay(250);
    }

    resetTimer();
  }  

}

void startStopTimer() {
  if(startCoolTime + 1000 <= currentTime) {
    if(isCounting) {
      if(isPaused) {
        pausedTime = currentTime - pausedTimeStart;
        startTime += pausedTime;
        isPaused = false;
      } else {
        pausedTimeStart = millis();
        isPaused = true;
      }
    } else {
      isCounting = true;
      isPaused = false;
      isIdle = false;
      startTime = millis();
    }

    startCoolTime = currentTime;
  }
}

void resetTimer() {
  if(resetCoolTime + 1000 <= currentTime) {
    isPaused = true;
    isCounting = false;
    isIdle = true;
    elastedTime = 0;
    
    resetCoolTime = currentTime;
  }
}

void incTimer() {
  if (isIdle && incCoolTime + 1000 <= currentTime) {
    totalTime += 1000;
    incCoolTime = currentTime;
  }
}

void decTimer() {
  if (isIdle && incCoolTime + 1000 <= currentTime) {
    totalTime -= 1000;
    incCoolTime = currentTime;
  }
}

void printTime(int display) {
  lcd.setCursor(0, 1);
  lcd.print("Seconds: ");

  if(display <= 0){
    lcd.print(0);
  }
  else{
    if(display < 10) {
      lcd.print("  ");
      lcd.setCursor(9, 1);
    }
    lcd.print(display);
  }
}

