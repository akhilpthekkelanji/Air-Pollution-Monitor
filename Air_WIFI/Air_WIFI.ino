#include <dht.h>
#include <SoftwareSerial.h>
#define RX 6
#define TX 7
String AP = "Wifi SSID";       // CHANGE ME
String PASS = "Wifi Password"; // CHANGE ME
String API = "Your API Key Here";   // CHANGE ME
String HOST = "api.thingspeak.com";
String PORT = "80";
String field = "field1";
int countTrueCommand;
int countTimeCommand;
boolean found = false;
int valSensor = 1;
float h;
float t;
SoftwareSerial data(RX,TX);

dht DHT;
#define DHT11_PIN 2
void setup() {
  Serial.begin(115200);
  data.begin(115200);
  sendCommand("AT",5,"OK");
  sendCommand("AT+CWMODE=1",5,"OK");
  sendCommand("AT+CWJAP_CUR=\""+ AP +"\",\""+ PASS +"\"",20,"OK");
}
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print(countTrueCommand);
  Serial.print(". at command => ");
  Serial.print(command);
  Serial.print(" ");
  while(countTimeCommand < (maxTime*1))
  {
    data.println(command);//at+cipsend
    if(data.find(readReplay))//ok
    {
      found = true;
      break;
    }

    countTimeCommand++;
  }

  if(found == true)
  {
    Serial.println("OYI");
    countTrueCommand++;
    countTimeCommand = 0;
  }

  if(found == false)
  {
    Serial.println("Fail");
    countTrueCommand = 0;
    countTimeCommand = 0;
  }

  found = false;
 }

void loop() {
 //valSensor = humidity();
  int chk = DHT.read11(DHT11_PIN);
  h=DHT.humidity;
  t=DHT.temperature;
  Serial.print(DHT.humidity, 1);
  Serial.print(",\t");
  Serial.println(DHT.temperature, 1);


 String getData = "GET /update?api_key="+ API + String("&field1=" + String(t,DEC) + "&field2=" + String(h, DEC));
sendCommand("AT+CIPMUX=1",5,"OK");
 sendCommand("AT+CIPSTART=0,\"TCP\",\""+ HOST +"\","+ PORT,15,"OK");
 sendCommand("AT+CIPSEND=0," +String(getData.length()+4),4,">");
 data.println(getData);delay(1500);countTrueCommand++;
 sendCommand("AT+CIPCLOSE=0",5,"OK");
 delay(2000);
}
