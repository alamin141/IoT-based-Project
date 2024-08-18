#define BLYNK_PRINT Serial
#define BLYNK_TEMPLATE_ID "TMPL6zOk2SEI7"
#define BLYNK_TEMPLATE_NAME "Smart Poultry and Fish Farming System"
#define BLYNK_DEVICE_NAME "Smart Poultry and Fish Farming System"

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <SoftwareSerial.h>
#define BLYNK_AUTH_TOKEN "_yRiHwBUaH8HYV3r0_iVFdokYlYMavzC"
char auth[] = "_yRiHwBUaH8HYV3r0_iVFdokYlYMavzC";
SoftwareSerial uno(14,12);

char ssid[] = "project";
char pass[] = "12345678";
BlynkTimer timer;
String content = "";
String HUM = "";
String TEM = "";
char character;

int ind1;
int ind2;
int ind3;


BLYNK_CONNECTED()
{
  Blynk.syncVirtual(V0);
  Blynk.syncVirtual(V1);
  Blynk.syncVirtual(V2);  
}

void setup()
{
  // Debug console
  Serial.begin(9600);
  uno.begin(9600);
  Blynk.begin(auth, ssid, pass);
}

void loop()
{
  Blynk.run();
  timer.run();
  if (uno.available())  {
    char c = uno.read();  //gets one byte from serial buffer
    if (c == '\n') {  
      Serial.println(content);          
      uno.flush();
      ind1 = content.indexOf('=');  //finds location of first ,
      HUM = content.substring(0, ind1);   //captures first data String
      ind2 = content.indexOf('=', ind1+1 );   //finds location of second ,
      HUM = content.substring(ind1+1, ind2);   //captures second data String
      ind3 = content.indexOf('=', ind2+1 );   //finds location of second ,
      TEM = content.substring(ind2+1);   //captures second data String
      TEM.trim();
      Serial.println(TEM);
      content="";
      
      Blynk.virtualWrite(V0, HUM);
      Blynk.virtualWrite(V1, TEM); 
      if(TEM.toInt() > 33){
        Blynk.virtualWrite(V2, 1);              
      }
      else{
        Blynk.virtualWrite(V2, 0);              
      }
    }
    else{
      content += c;
    }
  }
}
