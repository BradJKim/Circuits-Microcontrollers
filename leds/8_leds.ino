const int numeral[10] = {
  B11111100,
  B01100000,
  B11011010,
  B11110010,
  B01100110,
  B10110110,
  B00111110,
  B11100000,
  B11111110,
  B11110110,
};

//dp, G F E D C B A
const int segmentPins[] = { 4, 7, 8, 6, 5, 3, 2, 9 };

const int numberOfDigits = 4;

const int digitPins[] = { 10, 11, 12, 13 };

int value = 0;

void setup() {
  Serial.begin(9600);

  for (int i =0; i<8; i++)
    pinMode(segmentPins[i], OUTPUT);

  for (int i =0; i<numberOfDigits; i++)
    pinMode(digitPins[i], OUTPUT);
}

void loop() {
  // get time
  //showNumber(value);

  for(int i=0; i<4; i++) {
    
  }

  /* showDigit(5, 2);
  delay(1000);
  value++; */
}

void showNumber(int number) {
  for (int digit=numberOfDigits-1; digit>=0; digit--) {
    if(number>0){
      showDigit(number % 10, digit);
      number /= 10;
    }
  }
}

void showDigit(int number, int digit) {
  digitalWrite(digitPins[2], HIGH);
  for (int segment =1; segment<=8; segment++) {
    boolean isBitSet = bitRead(numeral[number], segment);

    //isBitSet = ! isBitSet;
    digitalWrite(segmentPins[segment], isBitSet);
  }
  delay(5);
  digitalWrite(digitPins[2], LOW);
}
