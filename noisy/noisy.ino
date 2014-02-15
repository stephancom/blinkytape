//            _         
//   _ _  ___(_)____  _ 
//  | ' \/ _ \ (_-< || |
//  |_||_\___/_/__/\_, |
//                 |__/ 
// by stephan.com 2014
//
// a blinkytape program that selects a new color on every button push
// and fades to the new color by random diffusion
//
// TODO debounce button

#include <FastSPI_LED2.h>

#define LED_COUNT 60 // BlinkyTape has 60 LEDs!
struct CRGB leds[LED_COUNT]; // this struct contains 60 CRGB values.  This is where 

#ifdef REVB // RevB boards have a slightly different pinout.

#define LED_OUT      5
#define BUTTON_IN    13
#define ANALOG_INPUT A11
#define IO_A         15

#else

#define LED_OUT      13
#define BUTTON_IN    10
#define ANALOG_INPUT A9
#define IO_A         7
#define IO_B         11

#endif

#define PAUSE_TIME 50
#define FADE_STEPS 40

int count = 0;
CRGB old_color;
CRGB new_color;

CRGB random_color() {
  return random() & 0xffffff;
}


// first, let's get ready to blink using some FastSPI_LED2 routines
// take a look at the FastSPI_LED2 example called Fast2Dev for more usage info
void setup()
{  
  LEDS.addLeds<WS2811, LED_OUT, GRB>(leds, LED_COUNT); // this configures the BlinkyBoard - leave as is.
  LEDS.setBrightness(10);
  LEDS.showColor(old_color = random_color());
    new_color = random_color();
}

void do_fade() {
  for(int i=0 ; i < LED_COUNT ; i++) {
    if(random() % FADE_STEPS < count) {
      leds[i] = old_color;
    } else {
      leds[i] = new_color;
    }
  }
  
  LEDS.show();
}

void loop() {
  if (Serial.available()){
     while(Serial.read() != -1){};
     count = FADE_STEPS;
  }
  else if(digitalRead(BUTTON_IN) == 1 && count == 0){
     count = FADE_STEPS; 
  } else if(count){
    do_fade();
    count--;
    if(!count){
      old_color = new_color;
      LEDS.showColor(old_color);
    new_color = random_color();
    }
  }
  
  delay(PAUSE_TIME);
}
