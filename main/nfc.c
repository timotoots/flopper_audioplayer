/*

  Flopper ESP32 / NFC
  
  This software is released under the MIT License.
  https://opensource.org/licenses/MIT

*/

////////////////////////////////////////////////////////////////

int number_times_before_read = 0;
String payloadAsString = "";
 int payloadLength  = 0;

#include <SPI.h>
#include <PN532_SPI.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_SPI pn532spi(SPI, PIN_SPI_SS);
NfcAdapter nfc = NfcAdapter(pn532spi);

////////////////////////////////////////////////////////////////

void setup_nfc(){

  nfc.begin();

}

////////////////////////////////////////////////////////////////

void loop_nfc(void * parameter ){

    readNDEF();  
    delay(100);
    loop_nfc(NULL);

    // Future idea, enable IRQ?
    // https://forums.adafruit.com/viewtopic.php?f=31&t=58903&start=15

}

////////////////////////////////////////////////////////////////

/*
// Gets trigger by touch button
void nfcSaveUrl(){

      Serial.println("nfcSaveUrl!");

      if(url_to_rec!=""){
        ledRecReady(); // LED status
        writeNDEF(url_to_rec); // Save to NF
        url_to_rec = "";
      } else {
        Serial.println("nothing to rec!");
        // httpShowRecPage();  
      }
}

*/

////////////////////////////////////////////////////////////////

void readNDEF(){

 if (nfc.tagPresent()){

    number_times_before_read++;

    // Don't read before tag is stable in the reader, otherwise gives errors
    if(number_times_before_read<5){
      delay(100);
      return;
    }
    
    NfcTag tag = nfc.read();

    // Are we recording?
    if(flag_rec_now==1){  
      flag_rec_now = 0;
      Serial.println("Write to NDEF");
      writeNDEF(url_to_rec);
      url_to_rec = "";
      current_floppy_uid = "";
      delay(1000);
      return;
    }

    // Read the same floppy only once
    if(current_floppy_uid == tag.getUidString()){
      return;
    }

    // Get UID
    String tag_uid = tag.getUidString();
    tag_uid.replace(" ","");
    current_floppy_uid = tag.getUidString();

    Serial.println("--------------------");
    Serial.println("NEW FLOPPY INSERTED!");
    Serial.print("Tag Type: ");
    Serial.println(tag.getTagType());

    Serial.print("Tag UID: ");   
    Serial.println(tag_uid);

  
    if (tag.hasNdefMessage()){

      current_floppy_ndef = 1;
      NdefMessage message = tag.getNdefMessage();
      
     // Serial.print("NDEF records:");
    //  Serial.println(message.getRecordCount());

      int url_found = 0;
      
      int recordCount = message.getRecordCount();
      
      for (int i = 0; i < recordCount; i++){

        NdefRecord record = message.getRecord(i);

        if(DEBUG_ENABLED){
          
          Serial.print("NDEF Record ");
          Serial.print(i);
          Serial.print(" / TNF=");
          Serial.print(record.getTnf());
          Serial.print(" / Type=");
          Serial.print(record.getType());

        }

        payloadAsString = "";
        
        if(record.getType()=="U" && url_found==0){ // if URL
        
          int payloadLength = record.getPayloadLength();
          byte payload[payloadLength];
          record.getPayload(payload);
  
          
          for (int c = 0; c < payloadLength; c++) {
            payloadAsString += (char)payload[c];
          }

           current_floppy_url = "";
           for (int c = 0; c < payloadLength; c++) {
              current_floppy_url += (char)payload[c];
            }

          if(DEBUG_ENABLED){
          Serial.print(" / Payload=");
          Serial.print(payloadAsString);
          }
          
          url_found = 1;
          
        } // if URL

        Serial.println();
        
      } // for

        if(url_found==1){
          ledFloppyInserted();
          Serial.print("Tag URL:");
          Serial.println(payloadAsString);

            if(payloadAsString.length()<10){

             current_floppy_url = "";

            
          } 

        
        }

      Serial.println("---------------");

    } else {
      Serial.println("Tag has no NDEF");
      current_floppy_ndef = 0;
      current_floppy_url = "";
      Serial.println("---------------");
    }
    
  } else if(current_floppy_uid!="") {

      Serial.println("---------------");
      Serial.println("FLOPPY REMOVED!");
      current_floppy_ndef = 0;
      Serial.println("---------------");
      ledFloppyRemoved();
      current_floppy_uid = "";
      current_floppy_url = "";
      number_times_before_read = 0;
  }
  delay(100);


  /////////////////////
  
} // readNdef

////////////////////////////////////////////////////////////////


void writeNDEF(String url){

    Serial.println("--------------------");
    Serial.println("REC: Started");

    bool success = false;

    if (nfc.tagPresent()) {

      Serial.println("REC: Floppy found!");

      if (current_floppy_ndef){
          Serial.println("REC: Floppy is already NDEF formatted, overwriting!");
          success = true;
      } else {
          Serial.println("REC: Floppy is not formatted to NDEF!");
          delay(500);
          success = nfc.format();
          if(success){
            Serial.println("REC: Floppy format success!");
          } else {
            Serial.println("REC: Floppy format failed!");
          }
      }
       
      if (success) {
          
          NdefMessage message = NdefMessage();
          message.addUriRecord(url);
  
          success = nfc.write(message);
          if (success) {
            Serial.println("REC: Floppy write success!");   
            current_floppy_uid = "";     
          } else {
            Serial.println("REC: Floppy write failed!");
            current_floppy_uid = "";   
          }
      } else {
        Serial.println("REC: Floppy format failed!??");
        current_floppy_uid = "";   
      }

      Serial.println("--------------------");

    } else {
      Serial.println("REC: No floppy found");
      delay(3000);
     // writeNDEF(url);
      Serial.println("--------------------");

    }
    
}



////////////////////////////////////////////////////////////////
