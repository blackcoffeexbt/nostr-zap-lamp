#include <Arduino.h>
// the pin where LED is connected
int ledPin = 13;
int buttonPin = 4;

// timing variables
unsigned long lightningTime = 0; // variable to store the last time lightning was shown
int lightningInterval = 5000;    // time between lightning strikes in milliseconds
int fadeInterval = 1;            // how fast to fade the LED

int defaultLightValue = 10;

void setup() {
  Serial.begin(115200);
  pinMode(buttonPin, INPUT_PULLUP);
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
    int flashDuration = 500 / flash * random(1,3);
    delay(flashDuration / 2);

    // fast fade-out
    for (int i = 255; i >= defaultLightValue; i = i - 1) {
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

uint16_t numFlashes = 0;

unsigned long buttonPressTime = 0;
unsigned long buttonReleaseTime = 0;

void loop() {
  if (digitalRead(buttonPin) == LOW) { // button is pressed
    if (buttonPressTime == 0) { // to record only the first press event
      buttonPressTime = millis(); // save the time when button press was detected
    }

    // increase LED brightness the longer the button is pressed
    unsigned long elapsedTime = millis() - buttonPressTime;
    int brightness = min((int)(255 * elapsedTime / 10000.0), 255); // max out at 255 after 10s
    analogWrite(ledPin, brightness);  // set the LED brightness
  } else { // button is not pressed
    if (buttonPressTime != 0) { // if a previous button press was recorded
      // quickly fade the LED to 0
      for (int i = 255; i >= 0; i = i - 2) {
        analogWrite(ledPin, i);  // set the LED brightness
        delay(1);  // wait for a moment
      }  
      buttonReleaseTime = millis(); // save the time when button was released
      Serial.print(buttonReleaseTime - buttonPressTime);
      Serial.println(" milliseconds");
      unsigned long buttonPressDuration = buttonReleaseTime - buttonPressTime;
      numFlashes = buttonPressDuration / 1000;

      Serial.print("Flashing ");
      Serial.print(numFlashes);
      Serial.println(" times");

      lightningFlash(numFlashes);
      numFlashes = 0;

      buttonPressTime = 0; // reset press time for the next button press event
    }
  }
}
