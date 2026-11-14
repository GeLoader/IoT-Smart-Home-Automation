#define BLYNK_TEMPLATE_ID "xxxxxx"
#define BLYNK_TEMPLATE_NAME "xxxxxx"
#define BLYNK_AUTH_TOKEN "xxxxxx"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <DHT.h>
#define DHTPIN D5 
#define DHTTYPE DHT11
#define ButtonSwitch1 D6
#define ButtonSwitch2 D7
 
 BlynkTimer timer; 
char auth[] = BLYNK_AUTH_TOKEN;

 
char ssid[] = "xxxxxx";
char pass[] = "xxxxxx";

#define switchBulb1 D1
#define switchBulb2 D2
#define switchFan D3

bool bulb1 = 0;
bool bulb2 = 0;
float humDHT = 0;
float tempDHT = 0;

bool bulb1On = false;
bool bulb2On = false;
bool FanOn = false;

bool toggleRelay = false;
bool AutoMode = true;


String currMode;

DHT dht(DHTPIN, DHTTYPE);


BLYNK_WRITE(V3)
{
  bulb1On = param.asInt();
 
  if(bulb1On == 1)
  {
    digitalWrite(switchBulb1, LOW);
    
  }else{
     digitalWrite(switchBulb1, HIGH);
   
  }
  
}

BLYNK_WRITE(V4)
{
  bulb2On = param.asInt();
 
  if(bulb2On == 1)
  {
    digitalWrite(switchBulb2, LOW);
    
  }else{
     digitalWrite(switchBulb2, HIGH);
    
  }
}

 
BLYNK_WRITE(V5) {
    FanOn = param.asInt();
 
  if (FanOn == 1) {
     digitalWrite(switchFan, LOW);
  } else {
    digitalWrite(switchFan, HIGH);
    
  }
  
 
}

BLYNK_WRITE(V2) {
  if (!AutoMode && bulb1On) {
    digitalWrite(switchBulb1, LOW);

    bulb1On = false;
  }  
   if (!AutoMode && bulb2On) {
    digitalWrite(switchBulb2, LOW);

    bulb2On = false;
  }  

     if (!AutoMode && FanOn) {
    digitalWrite(switchFan, LOW);

    FanOn = false;
  } 
  
  currMode = AutoMode ? "AUTO" : "MANUAL";
}

 
 
void measureTemp() {

    humDHT = dht.readHumidity();
      tempDHT = dht.readTemperature();
 
 
  if (tempDHT>33)
  {
        if (AutoMode) {
        if (!FanOn) {
          digitalWrite(switchFan, LOW);
 
          FanOn = true;
          Blynk.virtualWrite(V5, FanOn);
        }
      }
    } else {
 
      if (AutoMode) {
        if (FanOn) {
          digitalWrite(switchFan, HIGH);
 
          FanOn = false;
          Blynk.virtualWrite(V5, FanOn);
        }
      }
 
  } 

  Blynk.virtualWrite(V1, tempDHT);
 
  
}
 
void setup()
{
  Serial.begin(115200);
 
  pinMode(D0,OUTPUT);
  pinMode(switchBulb1,OUTPUT);
  pinMode(switchBulb2,OUTPUT);
  pinMode(switchFan,OUTPUT);
  pinMode(ButtonSwitch1, INPUT_PULLUP);
pinMode(ButtonSwitch2, INPUT_PULLUP);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

   Blynk.virtualWrite(V3, 0);
     digitalWrite(switchBulb1, HIGH);
      Blynk.virtualWrite(V4, 0);
     digitalWrite(switchBulb2, HIGH);
       Blynk.virtualWrite(V5, 0);
     digitalWrite(switchFan, HIGH);
    Serial.println(F("DHTxx test!"));
    dht.begin();
   Blynk.begin(auth, ssid, pass);
  
      currMode = AutoMode ? "AUTO" : "MANUAL";
        timer.setInterval(1000L, measureTemp);
 
        delay(1000);
        Blynk.virtualWrite(V2, AutoMode);
  Blynk.virtualWrite(V5, FanOn);
}

void loop()
{
  Blynk.run();
   timer.run();


    if (digitalRead(ButtonSwitch1) == LOW) {
    delay(200);
    if (!bulb1On) {
      digitalWrite(D1, LOW);
      Blynk.virtualWrite(V3, 1);
      bulb1On = true;
    } else {
      digitalWrite(D1, HIGH);
      Blynk.virtualWrite(V3, 0);
      bulb1On = false;
    }
  }
  
  if (digitalRead(ButtonSwitch2) == LOW) {
    delay(200);
    if (!bulb2On) {
      digitalWrite(D2, LOW);
      Blynk.virtualWrite(V4, 1);
      bulb2On = true;
    } else {
      digitalWrite(D2, HIGH);
      Blynk.virtualWrite(V4, 0);
      bulb2On = false;
    }
  }
    
}
