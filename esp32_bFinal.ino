#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer;

#include <WiFiUdp.h>
#include <NTPClient.h>
const long utcOffsetInSeconds = 25200;
// Define NTP Client to get time
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "id.pool.ntp.org", utcOffsetInSeconds);


char auth[] = "tRHhVCzZrZkCPWKzf4tR6Y4fo8vps4tX";
char ssid[] = "Salam Sukses";
char pass[] = "sumuaph13";

int lux;
float temperature;
float phValue; 
float tdsValue;  
float distance_cm;

int jam,menit,detik;

int PH_Plus = 25;
int PH_Minus = 26;

const int RelayNutrisiA = 4;
const int RelayNutrisiB = 5;
const int RelayNutrisiAB = 18;

const int RELAY_PELTIER_PIN = 14;
const int RELAY_HEATER_PIN = 27;

const int pinRelay = 32;   // definisikan Relay pada pin digital 32

const int RELAY_POMPA = 23; 
const int RELAY_PIN = 19;

// baca slider
BLYNK_WRITE(V4){
  int pinValue = param.asInt();
  if(pinValue==1){
    Serial.println("BUTTON ON");
    digitalWrite(pinRelay,LOW);
  }
  if(pinValue==0){
    Serial.println("BUTTON OFF");
    digitalWrite(pinRelay,HIGH);
  }
}

BLYNK_WRITE(V10){
  int pinPompa = param.asInt();
  if(pinPompa==1){
    Serial.println("BUTTON ON");
    digitalWrite(RELAY_POMPA,LOW);
  }
  if(pinPompa==0){
    Serial.println("BUTTON OFF");
    digitalWrite(RELAY_POMPA,HIGH);
  }
}

int mytimer=0;
WidgetLCD lcd(V5);
void tasktimer(){
  Blynk.virtualWrite(V0, phValue);
  Blynk.virtualWrite(V1, tdsValue);
  Blynk.virtualWrite(V3, temperature);
  Blynk.virtualWrite(V2, lux);
  Blynk.virtualWrite(V11, distance_cm);

  Blynk.virtualWrite(V20, phValue);
  Blynk.virtualWrite(V21, tdsValue);
  Blynk.virtualWrite(V22, temperature);
  Blynk.virtualWrite(V23, lux);

  Serial.print(jam);
  Serial.print(":");
  Serial.print(menit);
  Serial.print(":");
  Serial.println(detik);

  lcd.clear(); 
  lcd.print(4, 0, String(jam)+":"+String(menit)+":"+String(detik));
  if(tdsValue < 1050) mytimer++;
}
  
void setup() {
  pinMode(PH_Plus, OUTPUT);
  pinMode(PH_Minus, OUTPUT);
  pinMode(RelayNutrisiA, OUTPUT);
  pinMode(RelayNutrisiB, OUTPUT);
  pinMode(RelayNutrisiAB, OUTPUT);
  pinMode(RELAY_PELTIER_PIN, OUTPUT);
  pinMode(RELAY_HEATER_PIN, OUTPUT);
  pinMode(pinRelay, OUTPUT);     // definisikan pinRelay sebagai OUTPUT
  pinMode(RELAY_POMPA, OUTPUT);
  pinMode(RELAY_PIN, OUTPUT); // set ESP32 pin to output mode

  digitalWrite(PH_Plus, HIGH);
  digitalWrite(PH_Minus, HIGH);
  digitalWrite(RelayNutrisiA, HIGH);
  digitalWrite(RelayNutrisiB, HIGH);
  digitalWrite(RelayNutrisiAB, HIGH);
  digitalWrite(RELAY_PELTIER_PIN, HIGH);
  digitalWrite(RELAY_HEATER_PIN, HIGH);
  digitalWrite(pinRelay, HIGH);     // definisikan pinRelay sebagai OUTPUT
  digitalWrite(RELAY_POMPA, HIGH);
  digitalWrite(RELAY_PIN, HIGH); // set ESP32 pin to output mode
  
  Serial2.begin(9600);
  Serial.begin(9600);
  Serial.println("starting");

  Blynk.begin(auth, ssid, pass);
  timer.setInterval(1000, tasktimer); // setting timer per detik
  timeClient.begin();
  Serial.println("ready");
  
}

String in="";
int lock=0;
void loop() {
  Blynk.run();
  timer.run();
  timeClient.update();

  jam = timeClient.getHours();
  menit = timeClient.getMinutes();
  detik = timeClient.getSeconds();

  if(Serial.available()){
  char c = Serial.read();
  if(isDigit(c) || c=='.' || c=='-') in+=c;
  if(c==' ') in="";

  if(c=='A'){
    lux = in.toFloat();
    in = "";
    Serial.print(" lux ");
    Serial.print(lux);
  }

  if(c=='B'){
    temperature = in.toFloat();
    in = "";
    Serial.print(" temp ");
    Serial.print(temperature);
  }

  if(c=='C'){
    phValue = in.toFloat();
    in = "";
    Serial.print(" ph ");
    Serial.print(phValue);
  }

  if(c=='D'){
    tdsValue = in.toFloat();
    in = "";
    Serial.print(" tds ");
    Serial.print(tdsValue);
  }

  if(c=='E'){
    distance_cm = in.toFloat();
    in = "";
    Serial.print(" jarak ");
    Serial.println(distance_cm);
  }
  
 }

    if(distance_cm < 9){
    
    //LOGIKA PH-------------------------------------------
    if ((phValue >= 6.0) && (phValue <= 7.0)) {
       digitalWrite (PH_Plus,  HIGH);  //Perintah Mematikan Pompa UP
       digitalWrite (PH_Minus, HIGH);  //Perintah Mematikan Pompa DOWN 
       
    }
    if (phValue < 6.0) {
      digitalWrite (PH_Plus,  LOW);//Perintah Menyalakan Pompa PH UP
      digitalWrite (PH_Minus, HIGH);//Perintah Mematikan Pompa PH DOWN
      
    }
    if (phValue > 7.0) {
      digitalWrite (PH_Plus,  HIGH);//Perintah Mematikan Pompa PH UP
      digitalWrite (PH_Minus, LOW);//Perintah Menyalakan Pompa PH DOWN   
      
    }

   

    if(tdsValue < 1050){
   
      if(mytimer < 5){
    digitalWrite(RelayNutrisiB,LOW);
    digitalWrite(RelayNutrisiA,HIGH);
    digitalWrite(RelayNutrisiAB,HIGH);
      }

      if(mytimer >= 5 && mytimer < 10){
    digitalWrite(RelayNutrisiB,HIGH);
    digitalWrite(RelayNutrisiA,LOW);   
    digitalWrite(RelayNutrisiAB,HIGH);
      }
      
      if(mytimer >= 10 && mytimer < 15){
    digitalWrite(RelayNutrisiB,HIGH);
    digitalWrite(RelayNutrisiA,HIGH);
    digitalWrite(RelayNutrisiAB,LOW);
      }
      
      if(mytimer >= 15) mytimer=0;
  
  }
  else {
    digitalWrite(RelayNutrisiB,HIGH);
    digitalWrite(RelayNutrisiA,HIGH);
    digitalWrite(RelayNutrisiAB,HIGH);
    mytimer=0;
  }

  if(temperature > 30)
  {
    digitalWrite(RELAY_PELTIER_PIN, LOW); // turn on
    digitalWrite(RELAY_HEATER_PIN, HIGH); // turn off
  } 
  if(temperature >= 25 && temperature <= 30)
  {
    digitalWrite(RELAY_PELTIER_PIN, HIGH); // turn off
    digitalWrite(RELAY_HEATER_PIN, HIGH); //turn off
    
  }
  if(temperature < 25)
  {
    digitalWrite(RELAY_HEATER_PIN, LOW); //turn on
    digitalWrite(RELAY_PELTIER_PIN, HIGH); // turn off
  }

    }
    else{
    digitalWrite (PH_Plus,  HIGH);  //Perintah Mematikan Pompa UP
    digitalWrite (PH_Minus, HIGH);  //Perintah Mematikan Pompa DOWN 
    digitalWrite(RELAY_HEATER_PIN, HIGH); //turn off
    digitalWrite(RELAY_PELTIER_PIN, HIGH); // turn off
    digitalWrite(RelayNutrisiB,HIGH);
    digitalWrite(RelayNutrisiA,HIGH);
    digitalWrite(RelayNutrisiAB,HIGH);
    digitalWrite(RELAY_PELTIER_PIN, HIGH); // turn off
    digitalWrite(RELAY_HEATER_PIN, HIGH); //turn off
    }

  if(distance_cm >= 9)
    {
      digitalWrite(RELAY_PIN, LOW); // turn on Relay
      
    }
  else
    {
      digitalWrite(RELAY_PIN, HIGH);  // turn off Relay  
      
    }

   if(jam>=7&&jam<19){
   if(lock==0){
    lock=1;
    digitalWrite(pinRelay,LOW); 
    
   }
   }
   else{
    if(lock==1){
    lock=0;
    digitalWrite(pinRelay,HIGH); 
    
   }
   }

}
