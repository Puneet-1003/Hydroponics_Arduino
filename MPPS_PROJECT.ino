//Files
#include "BluetoothSerial.h"
#include "DHT.h"
#include "WiFi.h"
#include "ThingSpeak.h"


//pin setup
#define DHTPIN 32 // dht 11 gpio25
#define WATER_PIN 35 //water level pin
# define Ph_PIN 34// ph sensor pin
#define RELAY1 25// relay pin
BluetoothSerial SerialBT; //Bluetooth object
DHT dht(DHTPIN, DHT11);


// variables to store data
int water_value; // water level read
float ph; // take normalize reading of ph sensor
float Value=0;// take reading from ph sensor
const char* ssid = "realme 8";//ssid of wifi device
const char* password = "lggyiwatt"; //password of wifi device
WiFiClient client;
unsigned long myChannelNumber = 1;// channel number of thingSpeak
const char * myAPI="PWYUWO6U2JGUD0S8";// API of ThingSPeak
String username;// router name
String pass;//password

void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid,password);
  Serial.print("Connecting...");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(9600);
  dht.begin();// dht start
  pinMode(WATER_PIN,INPUT);// water level pin set
  pinMode(Ph_PIN,INPUT); // ph sesor pin set
  pinMode(RELAY1,OUTPUT);// Relay of dosing pump
  initWiFi();
  Serial.print("RRSI: ");
  Serial.println(WiFi.RSSI());
  ThingSpeak.begin(client);
  delay(1000);
}

void loop()
{
  
  // ph sensor code
  Serial.println("Ph sensor reading------------");
  Value= analogRead(Ph_PIN);
  Serial.print("Un_normalized value by ph sensor = ");
  Serial.println(Value);
  float voltage=Value*(3.3/1023.0);
  ph=(3.3*voltage);
  Serial.print("Normalized value by ph sensor = ");
  Serial.println(ph);

  Serial.println();
  //DHT PIN Temperature sensor
  Serial.println("Dht sensor reading------------");
  float temp = dht.readTemperature();
  float humid = dht.readHumidity();
  Serial.print("Temperature = ");
  Serial.print(temp);
  Serial.println("degree C");
  Serial.print("Humidity = ");
  Serial.println(humid);
  Serial.println();
  //Water Level
  Serial.println("water level sensor reading------------");
  water_value=analogRead(WATER_PIN);
  Serial.println(water_value);
  Serial.println();

  //Relay of dosing pump
  //Serial.println("Relay start dosing pump-----------------------");
  //digitalWrite(RELAY1,HIGH);
  //delay(3000);
  //digitalWrite(RELAY1,LOW);

  ThingSpeak.setField(1, ph);
  ThingSpeak.setField(2, water_value);
  ThingSpeak.setField(3, temp);
  ThingSpeak.setField(5, humid);
  int x = ThingSpeak.writeFields(myChannelNumber, myAPI);


  //Bluetooth and Password
  
  
  
  delay(5000);

}