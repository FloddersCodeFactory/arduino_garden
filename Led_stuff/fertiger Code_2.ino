#include <FastLED.h>

// Arduino Music Visualizer 0.2

// This music visualizer works off of analog input from a 3.5mm headphone jack
// Just touch jumper wire from A0 to tip of 3.5mm headphone jack

// The code is dynamic and can handle variable amounts of LEDs
// as long as you adjust NUM_LEDS according to the amount of LEDs you are using

// LED LIGHTING SETUP
#define LED_PIN     7
#define NUM_LEDS    60
#define BRIGHTNESS  64
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];

#define UPDATES_PER_SECOND 100

// AUDIO INPUT SETUP
int audio = A0;

//MODE
int mode = 0; // 0->nothing, 1->audio reactive, 2->rainbow, 3->white adhs, 4->color adhs
int buttonpin = 5;

// STANDARD VISUALIZER VARIABLES
int loop_max = 0;
int k = 255; // COLOR WHEEL POSITION
int decay = 0; // HOW MANY MS BEFORE ONE LIGHT DECAY
int decay_check = 0;
long pre_react = 0; // NEW SPIKE CONVERSION
long react = 0; // NUMBER OF LEDs BEING LIT
long post_react = 0; // OLD SPIKE CONVERSION

// RAINBOW WAVE SETTINGS
int wheel_speed = 1;

void setup()
{
  // LED LIGHTING SETUP
  delay( 1000 ); // power-up safety delay
  FastLED.addLeds<LED_TYPE, LED_PIN, COLOR_ORDER>(leds, NUM_LEDS).setCorrection( TypicalLEDStrip );
  FastLED.setBrightness( BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
  FastLED.show();

  // SERIAL AND INPUT SETUP
  Serial.begin(115200);
  pinMode(audio, INPUT);
  Serial.println("\nListening...");

  //BUTTON SETUP
  pinMode(buttonpin, INPUT);
}

// FUNCTION TO GENERATE COLOR BASED ON VIRTUAL WHEEL
// https://github.com/NeverPlayLegit/Rainbow-Fader-FastLED/blob/master/rainbow.ino
CRGB Scroll(int pos) {
  CRGB color (0,0,0);
  if(pos < 85) {
    color.g = 0;
    color.r = ((float)pos / 85.0f) * 255.0f;
    color.b = 255 - color.r;
  } else if(pos < 170) {
    color.g = ((float)(pos - 85) / 85.0f) * 255.0f;
    color.r = 255 - color.g;
    color.b = 0;
  } else if(pos < 256) {
    color.b = ((float)(pos - 170) / 85.0f) * 255.0f;
    color.g = 255 - color.b;
    color.r = 1;
  }
  return color;

  mode = 0;
}

// FUNCTION TO GET AND SET COLOR
// THE ORIGINAL FUNCTION WENT BACKWARDS
// THE MODIFIED FUNCTION SENDS WAVES OUT FROM FIRST LED
// https://github.com/NeverPlayLegit/Rainbow-Fader-FastLED/blob/master/rainbow.ino



// +++ MODES +++ \\

void white_adhs()      // WHITE ADHS
{
  leds[NUM_LEDS] = CRGB(255, 255, 255);
  delay(100);
  FastLED.show();
  leds[NUM_LEDS] = CRGB(0, 0, 0);
  delay(100);
  FastLED.show();
}

void rainbow()         // RAINBOW
{
  for(int i = NUM_LEDS - 1; i >= 0; i--) {
    if (i < react)
      leds[i] = Scroll((i * 256 / 50 + k) % 256);
    else
      leds[i] = CRGB(0, 0, 0);
  }
  FastLED.show();
}

void color_adhs()
{
  leds[NUM_LEDS] = CRGB(255, 0, 0); //rot
  delay(70);
  leds[NUM_LEDS] = CRGB(0, 255, 0); //grün
  delay(70);
  leds[NUM_LEDS] = CRGB(0, 0, 255); //blau
  delay(70);
  leds[NUM_LEDS] = CRGB(255, 255, 0); //gelb
  delay(70);
  leds[NUM_LEDS] = CRGB(255, 165, 0); //orange
  delay(70);
  leds[NUM_LEDS] = CRGB(0, 255, 255); //cyan
  delay(70);
  leds[NUM_LEDS] = CRGB(255, 0, 255); //magenta
  delay(70);

  FastLED.show();
}

void music_reactive()
{
  int audio_input = analogRead(audio) *3 - 900; // ADD x2 HERE FOR MORE SENSITIVITY

  if (audio_input > 0)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 30L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
      react = pre_react;

    Serial.print(audio_input);
    Serial.print(" -> ");
    Serial.print(pre_react);
    Serial.print(" -> ");
    Serial.println(react);
  }

  rainbow(); // APPLY COLOR

  k = k - wheel_speed; // SPEED OF COLOR WHEEL
  if (k < 0) // RESET COLOR WHEEL
    k = 255;

  // REMOVE LEDs
  decay_check++;
  if (decay_check > decay)
  {
    decay_check = 0;
    if (react > 0)
      react--;
  }
}




void loop()
{
    // MODE AUSWAHL
  if(digitalRead(buttonpin) == HIGH)
  {
    if(mode == 3)
    {
      mode = 1;
    } else
    {
      mode = mode + 1;
    }
  }

  Serial.println(mode);

  //---------------------------------------------


  switch (mode)
  {
    case 0:

    break;

    case 1:
      music_reactive();
    break;

    case 2:
      rainbow();
    break;

    case 3:
     white_adhs();
    break;

    case 4:
      color_adhs();
    break;
  }


}
