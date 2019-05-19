#include <Adafruit_NeoPixel.h>

// Which pin on the Arduino is connected to the NeoPixels?
#define LEDPIN        6 // On Trinket or Gemma, suggest changing this to 1

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 8 // Popular NeoPixel ring size

#define MOTIONSENSORPIN 2
#define LIGHTSENSORPIN A0

// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, LEDPIN, NEO_RGBW + NEO_KHZ800);

int lightIntensity = 0;
int currentBrightness = 0;
int maxBrightness = 100;
double brightnessPercentage;

int maxLedCount = 8;
uint32_t color;
int activeLedCount;

int motionDetectionStatus = 0;

void setup() {
  //Serial.begin(9600);

  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  pixels.clear();
  pixels.show();

  pinMode(MOTIONSENSORPIN, INPUT);

  color = pixels.Color(0, 0, 0, 50);
}

void loop() {
  /*
    lightIntensity = analogRead(LIGHTSENSORPIN);
    if (lightIntensity > 50)
    {
    pixels.clear();
    pixels.show();
    return;
    }
  */

  motionDetectionStatus = digitalRead(MOTIONSENSORPIN);

  if (!motionDetectionStatus)
  {
    if (currentBrightness > 0)
    {
      currentBrightness--;
      smoothLight();
    }

    delay(5);
    return;
  }

  if (currentBrightness > maxBrightness)
  {
    return;
  }

  smoothLight();
  currentBrightness++;

  delay(5);
}

void smoothLight()
{
  brightnessPercentage = calculateBrightness();
  activeLedCount = maxLedCount * brightnessPercentage / 100;

  activateLedStrip(activeLedCount);
}

double calculateBrightness() {
  return 100 / maxBrightness * currentBrightness;
}

void activateLedStrip(int activeLedCount) {
  pixels.setBrightness(currentBrightness);
  pixels.clear(); // Set all pixel colors to 'off'

  for (int i = 1; i <= activeLedCount; i++) {
    pixels.setPixelColor(i - 1, color);
  }

  pixels.show();
}
