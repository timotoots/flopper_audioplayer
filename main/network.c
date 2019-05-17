/*

  Flopper ESP32 / Network Wifi and Http server
  
  This software is released under the MIT License.
  https://opensource.org/licenses/MIT

*/

///////////////////////////////////////////////////////////////////

#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

#include <time.h>
#include <AutoConnect.h>

#include <HTTPClient.h>
WebServer Server;

AutoConnect      Portal(Server);
AutoConnectConfig   Config;       // Enable autoReconnect supported on v0.9.4

#define TIMEZONE    (3600 * 9)    // Tokyo
#define NTPServer1  "ntp.nict.jp" // NICT japan.
#define NTPServer2  "time1.google.com"

///////////////////////////////////////////////////////////////////

void setup_network(){

  Server.on("/", rootPage);
  Server.on("/current_floppy/", getCurrentFloppy);
  Server.on("/get_page/", getPage);
  Server.on("/current_state/", showCurrentState);
  Server.on("/set_rec_url/", setRecUrl);

  Config.autoReconnect = true;
  Config.apid = "flopper";

  Portal.config(Config);

  if (Portal.begin()) { //   if (Portal.begin("Liug","timotimo")) {

    if (MDNS.begin("flopper")) {
      MDNS.addService("http", "tcp", 80);
    }
    Serial.println("WiFi connected: " + WiFi.localIP().toString());

    registerAtDiscovery();
     
    //configTime(TIMEZONE, 0, NTPServer1, NTPServer2);
  }

 

}
///////////////////////////////////////////////////////////////////

void httpShowRecPage(){

  
  
}

///////////////////////////////////////////////////////////////////


int currentState[6] = {0,0,0,0,0,0};
int lastState[6] = {0,0,0,0,0,0};

void showCurrentState(){

  int showState = false;
  
  for(int i=0;i<6;i++){
    if(currentState[i]!=lastState[i]){
      showState = true;
    }
  }

  if(showState==true){
    Serial.print("Current state: ");
    for(int i=0;i<6;i++){
      Serial.print(currentState[i]);
    }
    Serial.println("");
     for(int i=0;i<6;i++){
       lastState[i]=currentState[i];
     }

  
  } else {
    delay(100);
    showCurrentState();
  }

  
}

///////////////////////////////////////////////////////////////////


void loop_network(void * parameter ){
     
     Portal.handleClient();
     loop_network(NULL);
}



void registerAtDiscovery(){

        char uuid[15]; //Create a Unique AP from MAC address
        uint64_t chipid=ESP.getEfuseMac();//The chip ID is essentially its MAC address(length: 6 bytes).
        uint16_t chip = (uint16_t)(chipid>>32);
        snprintf(uuid,15,"Flopper-%04X",chip);

        HTTPClient http;

        String discovery_url ="http://play.flopper.net/discovery/?local_ip=" +  WiFi.localIP().toString() + "&uuid=" + uuid;
        
        http.begin(discovery_url);
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK) {
          Serial.println("Discovery update success!");
        } else {
          Serial.println("Discovery update failed!");

        }
  
}

///////////////////////////////////////////////////////////////////

void getPage(){

      HTTPClient http;

       if (!Server.hasArg("url")) {
         Server.send(200, "text/html",  "bad args");
      }
      String url = Server.arg("url");
  
        http.begin(url);
        int httpCode = http.GET();
        if(httpCode == HTTP_CODE_OK) {
           String payload = http.getString();
           Serial.println("http request: " + url);
                 Server.send(200, "text/html",  payload);
        } else {
                 Server.send(200, "text/html",  "error");

        }

}

///////////////////////////////////////////////////////////////////

void setRecUrl(){

      HTTPClient http;

      if (!Server.hasArg("url")) {
         Server.send(200, "text/html",  "bad args");
      }
      String url = Server.arg("url");
      url_to_rec = url;

      Serial.println("Ready to rec url: " + url);
      Server.send(200, "text/html",  "OK");
      
}
///////////////////////////////////////////////////////////////////


void getCurrentFloppy(){
  
  Serial.println("webserver access: current_floppy/");
  Serial.println(current_floppy_url);
  Server.sendHeader("Access-Control-Allow-Origin","*");
  Server.send(200, "text/plain",  current_floppy_url);
 
}

///////////////////////////////////////////////////////////////////

void rootPage() {
  Serial.println("webserver access: /");
  String  content = 
    "<html>"
    "<head>"
    "<script src=\"http://play.flopper.net/jquery.js\"></script>"
    "<script src=\"http://play.flopper.net/esp32_client.js\"></script>"
    "<meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">"
    "</head>"
    "<body>"
    "<div id=\"canvas\"></div>"
    "<h2 align=\"center\" style=\"color:blue;margin:20px;\">Flopper</h2>"
    "<h3 align=\"center\" style=\"color:gray;margin:10px;\">{{DateTime}}</h3>"
    "<p style=\"padding-top:10px;text-align:center\">" AUTOCONNECT_LINK(COG_32) "</p>"
    "</body>"
    "</html>";
 /*
  static const char *wd[7] = { "Sun","Mon","Tue","Wed","Thr","Fri","Sat" };
  struct tm *tm;
  time_t  t;
  char    dateTime[26];

  t = time(NULL);
  tm = localtime(&t);
  sprintf(dateTime, "%04d/%02d/%02d(%s) %02d:%02d:%02d.",
    tm->tm_year + 1900, tm->tm_mon + 1, tm->tm_mday,
    wd[tm->tm_wday],
    tm->tm_hour, tm->tm_min, tm->tm_sec);
    
  content.replace("{{DateTime}}", String(dateTime));
  */
  
  Server.send(200, "text/html", content);
}

///////////////////////////////////////////////////////////////////
