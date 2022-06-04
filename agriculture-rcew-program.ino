#include <ThingSpeak.h>
#include <ESP8266WiFi.h>
#include <DHT.h>

DHT dht(D3,DHT11);
int motorPin=D0;
float h;
float t;
int soil=A0;

char ssid[] = "Sivani Reddy";   // your network SSID (name) 
char pass[] = "10231023";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 1756436;
const char * myWriteAPIKey = "I3U417FF6DLKGKFB";

String myStatus = "";

void setup() {
  pinMode(motorPin,OUTPUT);
  digitalWrite(motorPin,1);
  Serial.begin(9600);
  pinMode(soil,INPUT);
  dht.begin();
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak

}

void loop() {
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }

  h=dht.readHumidity();
  t=dht.readTemperature();
  if (isnan(h) || isnan(t))
    return;
  float mp=(100.00-(analogRead(soil)/1023.00)*100.00);
  Serial.print("Humidity: ");
  Serial.print(h);
  Serial.print(", Temp: ");
  Serial.print(t);
  Serial.print(", Moisture: ");
  Serial.print(mp);

  // set the fields with the values
  ThingSpeak.setField(1, mp);
  ThingSpeak.setField(2, t);
  ThingSpeak.setField(3,h);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  motor();
  delay(20000); // Wait 20 seconds to update the channel again

}

void motor() {
  float motorStatus = ThingSpeak.readFloatField(1756567, 1);
  Serial.print("Motor: ");
  Serial.print(motorStatus);
  if(motorStatus>0) {
    Serial.println("MOTOR ON");
    digitalWrite(motorPin,0);
  } else {
    Serial.println("MOTOR OFF");
    digitalWrite(motorPin,1);
  }
}
