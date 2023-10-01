#include<WiFi.h>
#include<Firebase_ESP_Client.h>
#include "addons/TokenHelper.h"
#include "addons/RTDBHelper.h"

#define ssid "realme 8"
#define pass "99819841"
#define  API_KEY "AIzaSyBvfd6ia-tHb65662nePZf1HCeFbGb0snM"
#define DATABASE "https://esp32hydroponic-default-rtdb.firebaseio.com/"

FirebaseData fbdo;
FirebaseAuth auth;
FirebaseConfig config;

unsigned long sendDataPrevMillis = 0;
bool signupOK = false;
int ldrData = 0;
float voltage = 0.0;

void setup()
{
  Serial.begin(9600);
  WiFi.begin(ssid,pass);
  Serial.print("Connecting to wifi");
  while(WiFi.status() != WL_CONNECTED)
  {
    Serial.print(".");
    delay(300);
  }
Serial.println();
Serial.print("Connected with IP: ");
Serial.println(WiFi.localIP());
Serial.println();
config.api_key = API_KEY;
config.database_url = DATABASE;
if(Firebase.signUp(&config, &auth, "",""))
{
  Serial.println("SihnUp OK");
  signupOK=true;
}
else
{
  Serial.print("%s\n",config.signer.signupError.message.c_str());

}

config.token_status_callback = tokenStatusCallback;
Firebase.begin(&config, &auth);
Firebase.reconnectWiFi(true);
}

voi loop()
{
  if(Firebase.ready() && signupOK && (millis() - sendDataPrevMillis > 5000 || sendDataPrevMillis == 0))
  {
    sendDataPrevMillis = millis();
    if(Firebase.RTDB.setFloat(&fbdo, "Sensor/temp_data", temp_data))
    {
      Serial.println();
      Serial.println("Successfull");
    }
    else
    {
      Serial.println("Failed: ");
    }
  }
}