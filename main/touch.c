/*

  Flopper ESP32 / Toiúuch
  
  This software is released under the MIT License.
  https://opensource.org/licenses/MIT

*/


///////////////////////////////////////////////////////////////////



int threshold = 25;
bool touch_prev = false;
bool touch_next = false;
bool touch_random = false;
bool touch_pause = false;
bool touch_rec = false;
bool touch_like = false;

bool accept_touch_rec = true;

bool readyToRec = false;

void touch_act_prev(){
 touch_prev = true;
}

void touch_act_next(){
 touch_next = true;
}

void touch_act_random(){
 touch_random = true;
}

void touch_act_pause(){
 touch_pause = true;
}




void touch_act_rec(){

 static unsigned long last_interrupt_time = 0;
 unsigned long interrupt_time = millis();
 // If interrupts come faster than 200ms, assume it's a bounce and ignore
 if (interrupt_time - last_interrupt_time > 2000)
 {
    if(url_to_rec!=""){
      flag_rec_now = 1; // write url to tag
    } else {
      Serial.println("Show recording in UI");
      // show rec page on web ui
    }
 }
 last_interrupt_time = interrupt_time;

  
  touch_rec = true;
  
  /*
  if(accept_touch_rec){
    Serial.println("XXX");
   accept_touch_rec = false;
   touch_rec = true;
   nfcSaveUrl();
   accept_touch_rec = true;
  }
*/
}

void touch_act_like(){
 touch_like = true;
}


void setup_touch() {
   // pinMode(PIN_TOUCH_RANDOM, OUTPUT);
    
  touchAttachInterrupt(PIN_TOUCH_PREV, touch_act_prev, threshold);
  touchAttachInterrupt(PIN_TOUCH_NEXT, touch_act_next, threshold);
  touchAttachInterrupt(PIN_TOUCH_RANDOM, touch_act_random, threshold);
  touchAttachInterrupt(PIN_TOUCH_PAUSE, touch_act_pause, threshold);
  touchAttachInterrupt(PIN_TOUCH_REC, touch_act_rec, threshold);
  touchAttachInterrupt(PIN_TOUCH_LIKE, touch_act_like, threshold);
}

void loop_touch(void * parameter){

 if(touch_prev){
    touch_prev = false;
    Serial.println("Touch: PREV");
  }
  if(touch_next){
    touch_next = false;
    Serial.println("Touch: NEXT");
  }
  if(touch_random){
    touch_random = false;
    Serial.println("Touch: RANDOM");
  }
  if(touch_pause){
    touch_pause = false;
    Serial.println("Touch: PAUSE");
  }  
  if(touch_rec){
    touch_rec = false;
    Serial.println("Touch: REC");
  }  
  if(touch_like){
    touch_like = false;
    Serial.println("Touch: LIKE");
  }    
  delay(100);
  loop_touch(NULL);
}
