/*

  Flopper ESP32 / Led
  
  This software is released under the MIT License.
  https://opensource.org/licenses/MIT

*/
#include <analogWrite.h>

///////////////////////////////////////////////////////////////////

int currentLed[] = {0,0,0};


void setup_led() {
  analogWriteResolution(PIN_LED_R, 12);
  analogWriteResolution(PIN_LED_G, 12);
  analogWriteResolution(PIN_LED_B, 12);

}

void ledRecReady(){

  int newLed[] = {6,0,0};
  showLed(newLed);
    
}

void ledFloppyInserted(){

  int newLed[] = {0,6,0};
  showLed(newLed);
    
}

void ledFloppyRemoved(){

  int newLed[] = {0,0,0};
  showLed(newLed);
}

void showLed(int newLed[]){

  analogWrite(PIN_LED_R, newLed[0]);
  analogWrite(PIN_LED_G, newLed[1]);
  analogWrite(PIN_LED_B, newLed[2]);
  
}

void loop_led(void * parameter ) {
  /*
  analogWrite(PIN_LED_R, 6);
  analogWrite(PIN_LED_G, 10);
  analogWrite(PIN_LED_B, 10);

// digitalWrite(PIN_LED, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(100);                       // wait for a second
 // digitalWrite(PIN_LED, LOW);    // turn the LED off by making the voltage LOW
  analogWrite(PIN_LED_R, 0);
  analogWrite(PIN_LED_G, 0);
  analogWrite(PIN_LED_B, 0);
*/
   delay(1000);
   
  loop_led(NULL);
}
