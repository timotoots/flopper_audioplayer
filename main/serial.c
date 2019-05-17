/*

  Flopper ESP32 / Led
  
  This software is released under the MIT License.
  https://opensource.org/licenses/MIT

*/
#include <analogWrite.h>

///////////////////////////////////////////////////////////////////


void setup_serial() {
 

}


void loop_serial(void * parameter ) {
  
  Serial.setTimeout(100); 

  if(Serial.available()){
     
    String a = Serial.readStringUntil('\n');// read the incoming data as string
    if(a.length()>10){
      url_to_rec = a;
      }
    Serial.print("new URL from serial:");
    Serial.println(a);

  }

  delay(100);
  //loop_serial(NULL);
}
