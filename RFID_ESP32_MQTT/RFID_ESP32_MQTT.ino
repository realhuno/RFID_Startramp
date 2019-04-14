/*
 *  Created by TheCircuit
*/

#define SS_PIN 21  //2 Shield //D2
#define RST_PIN 22 //3 Shield //D1

#include <SPI.h>
#include <MFRC522.h>
#include <WiFi.h>
#include <PubSubClient.h>


// Replace the next variables with your SSID/Password combination
const char* ssid = "startramp";
const char* password = "startramp";

// Add your MQTT Broker IP address, example:
//const char* mqtt_server = "192.168.1.144";
const char* mqtt_server = "192.168.4.1";

// Set your Static IP address
IPAddress local_IP(192, 168, 4, 104);
// Set your Gateway IP address
IPAddress gateway(192, 168, 4, 1);

IPAddress subnet(255, 255, 0, 0);

#define ARDUINO_CLIENT_ID "rfid_4"                     // Client ID for Arduino pub/sub
#define PUB_TEMP "rfid_4/uuid"   // MTTQ topic for temperature [C]
#define PUB_HUMID "rfid_4/time"             // MTTQ topic for humidity
#define SUB_STATUS "rfid_4/status"                           // MTTQ topic for LED
#define PUBLISH_DELAY 3000        


WiFiClient espClient;
PubSubClient client(espClient);
long lastMsg = 0;
char msg[50];
int value = 0;
long previousMillis;
bool hasFailed = false;

MFRC522 mfrc522(SS_PIN, RST_PIN);   // Create MFRC522 instance.
int statuss = 0;
int out = 0;
void setup() 
{
  Serial.begin(9600);   // Initiate a serial communication
  SPI.begin();   
  mfrc522.PCD_Init();   // Initiate MFRC522
  
    // MTTQ parameters
  setup_wifi();
  client.setServer(mqtt_server, 1883);
  client.setCallback(callback);
    delay(1500); // Allow hardware to stabilize

  previousMillis = millis();


/*
if(!mfrc522.PCD_PerformSelfTest())
{
hasFailed = true;
delay(100);
}

if (hasFailed) {
mfrc522.PCD_Init();
}else{
hasFailed = false;
mfrc522.PCD_Init();
}
  */
}
void loop() 
{




  if (!client.connected())
    reconnect();
/*
  if (millis() - previousMillis > PUBLISH_DELAY)
  {
    previousMillis = millis();
    float humidity = 30; // humidity
    float tempC = 30; // temperature [C]
    char tmpBuffer[20];
    
    // check if any reads failed and exit early (to try again).
    if (isnan(humidity) || isnan(tempC))
    {
      Serial.println("error reading sensor data");
      return;
    }
    else
    {
      Serial.print("[sensor data] temperature[C]: ");
      Serial.print(tempC);
      Serial.print(", humidity: ");
      Serial.println(humidity);

      //client.publish(PUB_TEMP, dtostrf(tempC, 6, 2, tmpBuffer));
      //client.publish(PUB_HUMID, dtostrf(humidity, 6, 2, tmpBuffer));
    }
  }
*/
  client.loop();
/*
 if(hasFailed = true){
  client.publish(SUB_STATUS, "NoCardReader!!");
  delay(3000);
 }
*/
  
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) 
  {
    //client.publish(SUB_STATUS, "scanning");
    return;
  }
  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial()) 
  {
    client.publish(SUB_STATUS, "foundcard");
    return;
  }
  //Show UID on serial monitor

  String content= "";
  byte letter;
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
     Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : "");
     Serial.print(mfrc522.uid.uidByte[i], HEX);
     content.concat(String(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : ""));
     content.concat(String(mfrc522.uid.uidByte[i], HEX));
  }
  content.toUpperCase();
  Serial.println();
  char tmpBuffer[20];
  client.publish(PUB_TEMP, content.c_str());
  delay(3000);
} 

void setup_wifi() {
  delay(10);
  // We start by connecting to a WiFi network
  Serial.println();
  if (!WiFi.config(local_IP, gateway, subnet)) {
  Serial.println("STA Failed to configure");
  }
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}


void reconnect()
{
  // Loop until reconnected
  while (!client.connected()) {
    Serial.print("Attempting MQTT connection ... ");
    // Attempt to connect
    if (client.connect(ARDUINO_CLIENT_ID)) {
      Serial.println("connected");
      // (re)subscribe
      //client.subscribe(SUB_LED);
      client.publish(SUB_STATUS, "online");
    } else {
      Serial.print("Connection failed, state: ");
      Serial.print(client.state());
      Serial.println(", retrying in 5 seconds");
      delay(5000); // Wait 5 seconds before retrying
    }
  }
}

// sub callback function
void callback(char* topic, byte* payload, unsigned int length)
{
  Serial.print("[sub: ");
  Serial.print(topic);
  Serial.print("] ");
  char message[length + 1] = "";
  for (int i = 0; i < length; i++)
    message[i] = (char)payload[i];
  message[length] = '\0';
  Serial.println(message);
 

}
