#include <LiquidCrystal.h>

LiquidCrystal lcd(7, 8, 9, 10, 11, 12);

const int BUZZER = 3; 
const int START= 2;
const int STOP = 4;
const int INCREMENT = 5;
const int DECREMENT = 6;

bool isStart = false;
bool isStop = true;

int incTime = 0;
int incCurrTime = 0;
int decTime = 0;
int decCurrTime = 0;

int setTimer = 20;
int startTime;
int currTime;
int displayTime;
bool starting = false;

void setup() {
  lcd.begin(16, 2);
  lcd.print("Timer");

  pinMode(BUZZER, OUTPUT);
  pinMode(START, INPUT);
  pinMode(STOP, INPUT);
  pinMode(INCREMENT, INPUT);
  pinMode(DECREMENT, INPUT);

  Serial.begin(9600);
}

void loop() {
  int startInput = digitalRead(START);
  int stopInput = digitalRead(STOP);
  int addInput = digitalRead(INCREMENT);
  int subInput = digitalRead(DECREMENT);

  if(startInput && !stopInput) {
    isStart = true;
    isStop = false;
  }
  else if(!startInput && stopInput) {
    isStart = false;
    isStop = true;
  }

  if(addInput) {
    if ( incTime == 0 ) {
      incTime = millis() / 1000;
    }

    incCurrTime = millis() / 1000;

    if(incCurrTime - incTime == 1 && setTimer < 99){
      setTimer++;
      incTime = 0;
    }
  }
  else{
    incTime = 0;
  }

  if(subInput) {
    if ( decTime == 0 ) {
      decTime = millis() / 1000;
    }

    decCurrTime = millis() / 1000;

    if(decCurrTime - decTime == 1 && setTimer > 0){
      setTimer--;
      decTime = 0;
    }
  }
  else{
    decTime = 0;
  }

  if (isStart) {
    if(!starting) {
      starting = true;
      startTime = millis() / 1000;
    }
    currTime = millis() / 1000;

    displayTime = setTimer - ( currTime - startTime );

    if(displayTime <= 0) {
      printTime(displayTime);
      tone(BUZZER, 261);
      delay(500);
      noTone(BUZZER);
      delay(500);
    }
  }
  
  if (isStop) {
    starting = false;
    displayTime = setTimer;
  }

  printTime(displayTime);
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