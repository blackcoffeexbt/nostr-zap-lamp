#include <Arduino.h>
// the pin where LED is connected
int ledPin = 13;

// timing variables
unsigned long lightningTime = 0; // variable to store the last time lightning was shown
int lightningInterval = 5000;    // time between lightning strikes in milliseconds
int fadeInterval = 1;            // how fast to fade the LED

void setup() {
  pinMode(ledPin, OUTPUT); // initialize digital ledPin as an output.
  randomSeed(analogRead(0)); // initialize random seed with a changing analog value (noise)
}

void lightningFlash(int numberOfFlashes) {
  for(int flash = 1; flash <= numberOfFlashes; flash++) {
    // turn the LED on
    digitalWrite(ledPin, HIGH);

    // wait for the specified time, longer for the first flash and shorter for subsequent flashes
    int flashDuration = 250 / flash * random(1,5);
    delay(flashDuration / 2);

    // fast fade-out
    for (int i = 255; i >= 0; i = i - 2) {
      analogWrite(ledPin, i);  // set the LED brightness
      delay(1);  // wait for a moment
    }
  }
}

void loop() {
   if (millis() - lightningTime >= lightningInterval) {
    // record the current time
    lightningTime = millis();
    // create a lightning flash with a random number of flashes
    lightningFlash(random(1,5));
  }
 delay(1000); 
}
