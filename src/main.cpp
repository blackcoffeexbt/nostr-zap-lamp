#include <Arduino.h>
// the pin where LED is connected
int ledPin = 13;

// timing variables
unsigned long lightningTime = 0; // variable to store the last time lightning was shown
int lightningInterval = 5000;    // time between lightning strikes in milliseconds
int fadeInterval = 1;            // how fast to fade the LED

int defaultLightValue = 20;

void setup() {
  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  randomSeed(analogRead(0)); // initialize random seed with a changing analog value (noise)
}

void fadeOutFlash(int intensity) {
  analogWrite(ledPin, defaultLightValue + intensity); // set the LED to the desired intensity

  delay(50); // delay to give the LED time to light up

  // fast fade-out
  for (int i = defaultLightValue + intensity; i >= defaultLightValue; i--) {
    analogWrite(ledPin, i); // set the LED brightness
    delay(1);  // wait for a moment
  }

  delay(50);
}

void lightningFlash(int numberOfFlashes) {

  fadeOutFlash(2);
  fadeOutFlash(10);
  fadeOutFlash(15);

  delay(50);

  for(int flash = 1; flash <= numberOfFlashes; flash++) {
    // turn the LED on
    digitalWrite(ledPin, HIGH);

    // wait for the specified time, longer for the first flash and shorter for subsequent flashes
    int flashDuration = 250 / flash * random(1,5);
    delay(flashDuration / 2);

    // fast fade-out
    for (int i = 255; i >= defaultLightValue; i = i - 2) {
      analogWrite(ledPin, i);  // set the LED brightness
      delay(1);  // wait for a moment
    }
  }

  delay(50);

  fadeOutFlash(15);
  fadeOutFlash(5);

  // set led to low light
  analogWrite(ledPin, defaultLightValue);

}

void loop() {
  lightningFlash(1);
  delay(2000); 
  lightningFlash(2);
  delay(2000); 
  lightningFlash(3);
  delay(2000); 
}
