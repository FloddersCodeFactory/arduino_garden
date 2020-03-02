#include <IRremote.h>
int irPin = 8; //Pin am Arduino Nano für den IR Receiver
IRrecv irrecv(irPin); //Objekt initialisieren für die IR Übertragung
decode_results results;
int IRinput = 0;


#include <FastLED.h>
FASTLED_USING_NAMESPACE

// Arduino Music Visualizer 0.2

// This music visualizer works off of analog input from a 3.5mm headphone jack
// Just touch jumper wire from A0 to tip of 3.5mm headphone jack

// The code is dynamic and can handle variable amounts of LEDs
// as long as you adjust NUM_LEDS according to the amount of LEDs you are using

// LED LIGHTING SETUP
#define LED_PIN     7
#define NUM_LEDS    63
#define BRIGHTNESS  255
#define LED_TYPE    WS2812B
#define COLOR_ORDER GRB
CRGB leds[NUM_LEDS];
uint8_t hue = 0;

#define UPDATES_PER_SECOND 100

// AUDIO INPUT SETUP
int audio = A0;
int audio_input = 0;

//MODE
int modeTS = 0; // 0->nothing, 1->audio reactive, 2->white adhs, 3->color adhs, 4->rainbow
int buttonpin = 12;

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
  FastLED.setBrightness(  BRIGHTNESS );

  // CLEAR LEDS
  for (int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 0);
  FastLED.show();

  // SERIAL AND INPUT SETUP
  Serial.begin(9600);
  pinMode(audio, INPUT);
  Serial.println("\nListening...");

  //BUTTON SETUP
  pinMode(buttonpin, INPUT);

  // IR SETUP
  pinMode(irPin, INPUT);  //Den IR Pin als Eingang deklarieren.
  irrecv.enableIRIn(); //Den IR Pin aktivieren
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

}

void music_rainbow() {
  for (int i = NUM_LEDS - 1; i >= 0; i--)
  {
    if (i < react)
    {
      leds[i] = Scroll((i * 256 / 50 + k) % 256);
    }
    else
    {
      leds[i] = CRGB(0, 0, 0);
    }
  }
  FastLED.show();
}

// FUNCTION TO GET AND SET COLOR
// THE ORIGINAL FUNCTION WENT BACKWARDS
// THE MODIFIED FUNCTION SENDS WAVES OUT FROM FIRST LED
// https://github.com/NeverPlayLegit/Rainbow-Fader-FastLED/blob/master/rainbow.ino


// +++ MODES +++ \\

void music_reactive()
{
  audio_input = analogRead(audio) *3 - 900; // ADD x2 HERE FOR MORE SENSITIVITY

  if (audio_input > 0)
  {
    pre_react = ((long)NUM_LEDS * (long)audio_input) / 50L; // TRANSLATE AUDIO LEVEL TO NUMBER OF LEDs

    if (pre_react > react) // ONLY ADJUST LEVEL OF LED IF LEVEL HIGHER THAN CURRENT LEVEL
    {
      react = pre_react;
    }
   Serial.print(audio_input);
    Serial.print(" -> ");
    Serial.print(pre_react);
    Serial.print(" -> ");
    Serial.println(react);
  }

  music_rainbow(); // APPLY COLOR

  k = k - wheel_speed; // SPEED OF COLOR WHEEL

  if (k < 0) // RESET COLOR WHEEL
  {
    k = 255;
  }
  // REMOVE LEDs
  decay_check++;

  if (decay_check > decay)
  {
    decay_check = 0;

    if (react > 0)
      {
        react--;
      }

  }
}

void rainbow()         // RAINBOW
{
  fill_rainbow( leds, NUM_LEDS, hue, 7);
  FastLED.show();
  hue++;
}

void white_adhs()      // WHITE ADHS
{
  fill_solid( leds, NUM_LEDS, CRGB(255,255,255));
  FastLED.show();
  delay(40);
  fill_solid( leds, NUM_LEDS, CRGB(0,0,0));
  FastLED.show();
  delay(40);
}



void color_adhs()
{
  fill_solid( leds, NUM_LEDS, CRGB(255, 0, 0));
  FastLED.show();
  delay(40);
  fill_solid( leds, NUM_LEDS, CRGB(0, 255, 0));
  FastLED.show();
  delay(40);
  fill_solid( leds, NUM_LEDS, CRGB(0, 0, 255));
  FastLED.show();
  delay(40);
  fill_solid( leds, NUM_LEDS, CRGB(255, 255, 0));
  FastLED.show();
  delay(40);
  fill_solid( leds, NUM_LEDS, CRGB(255, 165, 0));
  FastLED.show();
  delay(40);
  fill_solid( leds, NUM_LEDS, CRGB(0, 255, 255));
  FastLED.show();
  delay(40);
  fill_solid( leds, NUM_LEDS, CRGB(255, 0, 255));
  FastLED.show();
  delay(40);

  FastLED.show();
}


void switchModeButton()
{
  if (digitalRead(buttonpin) == LOW)
  {
    return;
  }

  if (modeTS < 4)
  {
    modeTS = modeTS + 1;
    Serial.println("modeTS +1");
    delay(1000);
  }
  else
  {
     modeTS = 0;
     Serial.println("modeTS 0 (reset)");
     delay(1000);
   }
}

void switchModeIR()
{
  if (irrecv.decode(&results)) { //Wenn etwas gelesen wurde dann...
    IRinput = results.value;

  } //if(irrecv.decode(&results))
} //void

void loop()
{

  if (irrecv.decode(&results)) { //Wenn etwas gelesen wurde dann...
      //Ausgabe des Wertes auf die Serielle Schnittstelle.
      IRinput =  results.value;

      irrecv.resume(); // auf den nächsten Wert warten
      delay(250); // kurze Pause von 250ms damit die LED aufleuchten kann.

      switch(IRinput) {

      case 26775:
        modeTS = 0;
        break;
      case 12495:
        modeTS = 1;
        audio_input = 0;
        break;
      case 6375:
        modeTS = 2;
        break;
      case 31365:
        modeTS = 3;
        break;
      case 4335:
        modeTS = 4;
        break;
    } //switch(IRinput)

 }

  Serial.print(modeTS);
  Serial.print(" - ");
  Serial.println(IRinput);

 // switchModeButton();
 // switchModeIR();


  switch (modeTS)
  {
    case 0:
      //Serial.println("case 0");
      fill_solid(leds, NUM_LEDS, CRGB(0, 0, 0));
      FastLED.show();
      break;
    case 1:
      //Serial.println("case 1");
      music_reactive();
      break;
    case 2:
      //Serial.println("case 2");
      white_adhs();
      break;
    case 3:
      //Serial.println("case 3");
      color_adhs();
      break;
    case 4:
      //Serial.println("case 4");
      rainbow();
      break;
  } //switch(modeTS)


  delay(20);

} //loop
