# RFID_Startramp
RFID_Startramp


Automatic Pilot detection with RFID for FPV Trackers.... Currently working racegod!!
Delta5.... currently not working!
Chorus.... currently not working!
Livetime.. currenlty not working!


Hardware and Software:
1x Raspberry pi act as Wifi Access Point with mqtt server and node-red
4x ESP32 each connected to RC522 RFID modul (SPI)

ssid and password is set so startramp!

change the values:
const char* mqtt_server = "192.168.4.1";  // raspberry pi IP

// Set your Static IP address
IPAddress local_IP(192, 168, 4, 104); // IP
// Set your Gateway IP address
IPAddress gateway(192, 168, 4, 1); //GW

IPAddress subnet(255, 255, 0, 0);

#define ARDUINO_CLIENT_ID "rfid_4" //Change this every ESP32 rfid_1 ... rfid_2... to identify
#define PUB_TEMP "rfid_4/uuid"   // ESP32 Nr1 = rfid_1/uuid .....  ESP32 Nr 4 rfid_4/uuid
#define PUB_HUMID "rfid_4/time"  // ESP32 Nr1 and so on....       
#define SUB_STATUS "rfid_4/status" //rfid_1/status 
