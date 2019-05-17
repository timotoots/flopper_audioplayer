/*

  Flopper ESP32 / Potentiometer
  
  This software is released under the MIT License.
  https://opensource.org/licenses/MIT

*/

///////////////////////////////////////////////////////////////////

int lastPotVal = 0;

void setup_pot() {
  pinMode(PIN_POT_IN, INPUT);  // declare the ledPin as an OUTPUT
  pinMode(PIN_POT_OUT, OUTPUT);  // declare the ledPin as an OUTPUT

}

void loop_pot(void * parameter ) {

  
  int potVal = map( analogRead(PIN_POT_IN), 4095,0,0,100);

  digitalWrite(PIN_POT_OUT, HIGH);  // add analogWrite?

  if( abs(potVal - lastPotVal) > 30 ){
    Serial.print("Potentiometer change: ");
    Serial.println(potVal);
    
    lastPotVal = potVal;
  }
  
  delay(500);  // delay in between reads for stability
  
  loop_pot(NULL);
}
