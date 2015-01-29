#include "DHT.h"
#define DHTPIN 12
#define DHTTYPE DHT22

#define SSID "[your-ssid]"
#define PASS "[your-pass]"
#define IP "184.106.153.149" // thingspeak.com
String GET = "GET /update?key=[insert write api key]&field1=";
String GET2 = "&field2=";



DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(9600);
  dht.begin();
  Serial.println("AT");
  delay(5000);
  if(Serial.find("OK")){
    connectWiFi();
  }
}

void loop(){
  float tempC = dht.readTemperature();
  tempC = tempC * 1.8 +32;
  char buffer[10];
  String tempF = dtostrf(tempC, 4, 1, buffer);
//  delay(10000);
  float tempH = dht.readHumidity();
  char bufferhum[10];
  String tenmpH = dtostrf(tempH, 4, 1, bufferhum);
  updateTemp(tempF, tenmpH);

//  updateHum(tenmpH);
  delay(50000);
}

void updateTemp(String tenmpF, String tenmpH){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  cmd = GET;
  cmd += tenmpF;
  cmd += GET2;
  cmd += tenmpH;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }else{
    Serial.println("AT+CIPCLOSE");
  }
}

void updateHum(String tenmpH){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  cmd = GET2;
  cmd += tenmpH;
  cmd += "\r\n";
  Serial.print("AT+CIPSEND=");
  Serial.println(cmd.length());
  if(Serial.find(">")){
    Serial.print(cmd);
  }else{
    Serial.println("AT+CIPCLOSE");
  }
}

 
boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    return true;
  }else{
    return false;
  }
}

