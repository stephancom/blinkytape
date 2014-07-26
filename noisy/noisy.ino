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
#include <Bounce2.h>

#define LED_COUNT 60 // BlinkyTape has 60 LEDs!
struct CRGB leds[LED_COUNT]; // this struct contains 60 CRGB values.  This is where 

#define LED_OUT      13
#define BUTTON_IN    10
#define ANALOG_INPUT A9
#define IO_A         7
#define IO_B         11

// uncomment this line to automatically change color
#define AUTOREPEAT

#define PAUSE_TIME 20
#define FADE_STEPS 64

int count = 0;
CRGB old_color;
CRGB new_color;
Bounce clicker = Bounce();

CRGB random_color() {
  CRGB color; // = random() & 0xffffff;
  color.setHue(random()%360);
  return color;
}


// first, let's get ready to blink using some FastSPI_LED2 routines
// take a look at the FastSPI_LED2 example called Fast2Dev for more usage info
void setup()
{  
  LEDS.addLeds<WS2811, LED_OUT, GRB>(leds, LED_COUNT); // this configures the BlinkyBoard - leave as is.
  LEDS.setBrightness(10);
  LEDS.showColor(old_color = random_color());
    new_color = random_color();
    
   clicker.interval(500);
   clicker.attach(BUTTON_IN);
   randomSeed(analogRead(0));
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

boolean do_start;

#ifdef AUTOREPEAT
  do_start = !count;
  if(do_start) { delay(1000); }
#else
  do_start = clicker.update() && !count;
#endif

  if(do_start){
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
