
//Контакт SH_CP
int clockPin = 10;
//Контакт ST_CP
int latchPin = 13;
//Контакт DS
int dataPin = 11;
#include <FastLED.h>
#define NUM_LEDS 24
#define PIN 8
CRGB leds[NUM_LEDS];
int numOfRegisters = 3;
byte* registerState;
void setup() {
  FastLED.addLeds <WS2811, PIN, BRG>(leds, NUM_LEDS).setCorrection(TypicalLEDStrip);
   FastLED.setBrightness(50);
//set pins to output because they are addressed in the main loop
pinMode(latchPin, OUTPUT);
	pinMode(clockPin, OUTPUT);
	pinMode(dataPin, OUTPUT);
Serial.begin(9600);
registerState = new byte[numOfRegisters];
	for (size_t i = 0; i < numOfRegisters; i++) {
		registerState[i] = 0;
	}
//Arduino doesn't seem to have a way to write binary straight into the code
//so these values are in HEX.  Decimal would have been fine, too.

}
void loop() {
  // leds[0] = CHSV(255, 0, 0); // задаем для первого пикселя синий цвет
  //  FastLED.show(); // отправляем информацию на ленту
  //  delay(500);
  leds[0] = CRGB(255, 0, 0);
  FastLED.show();
delay(3000);
  leds[0] = CRGB(0, 0, 0);
  FastLED.show();
delay(3000);
  if (Serial.available() > 0) {
 String  c = Serial.readString();
  
   int pin = c.toInt()-1;
   int reg = pin / 8;
   int actualPin = pin - (8 * reg);
   leds[pin] = CRGB(255, 255, 255);
  // leds[1] = CRGB(0, 255, 0);
  // leds[2] = CRGB(0, 0, 255);
  FastLED.show();
   regWrite(pin,HIGH);
   delay(500);
   regWrite(pin,LOW);
  
  Serial.println(c);
  delay(3000);
  leds[pin] = CRGB(0, 0, 0);

  FastLED.show();
// OpenDoor(c.toInt()-1);    
 

}


}
void regWrite(int pin, bool state){
	//Determines register
	int reg = pin / 8;
	//Determines pin for actual register
	int actualPin = pin - (8 * reg);

	//Begin session
	digitalWrite(latchPin, LOW);

	for (int i = 0; i < numOfRegisters; i++){
		//Get actual states for register
		byte* states = &registerState[i];

		//Update state
		if (i == reg){
    
			bitWrite(*states, actualPin, state);
		}

		//Write
		shiftOut(dataPin, clockPin, MSBFIRST, *states);
	}

	//End session
	digitalWrite(latchPin, HIGH);
}
  
