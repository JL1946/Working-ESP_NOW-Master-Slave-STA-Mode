#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h> //NEED THIS TO COMPILE

#define WIFI_CHANNEL 1
const uint8_t slaveCustomMac[] = {0xB7, 0xE6, 0x2D, 0xB2, 0x1B, 0x36};  //Custom mac address for This Slave Device
const uint8_t masterCustomMac[] = {0x30, 0xAE, 0xA4, 0x89, 0x92, 0x71}; //Custom mac address for The Master Device 
esp_now_peer_info_t master;
const esp_now_peer_info_t *masterNode = &master;
const byte maxDataFrameSize = 200;
uint8_t dataToSend[maxDataFrameSize];
byte cnt=0;
esp_err_t sendResult;
/* -----------------Declarations-----------------*/
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);

void initVariant()
{
	WiFi.mode(WIFI_MODE_STA);                                                             // WIFI_STA Mode
	esp_err_t setMacResult = esp_wifi_set_mac(ESP_IF_WIFI_STA, &slaveCustomMac[0]);       // #include <esp_wifi.h> required to compile this line
	Serial.print("setMacResult Status: "); Serial.println(setMacResult);
}

void addMasterAsPeer()
{
	memcpy( &master.peer_addr, &masterCustomMac, 6 );
	master.channel = WIFI_CHANNEL;
	master.encrypt = 0;
	if( esp_now_add_peer(masterNode) == ESP_OK)
	{
		Serial.println("Master Added As Peer!");
	}
}

void setup()
{
	Serial.begin(115200);
	Serial.print("\r\n\r\n");
	initVariant();
	WiFi.disconnect();
	if(esp_now_init() == ESP_OK)
	{
		Serial.println("ESPNow Init Success!");
	}
	else
	{
		Serial.println("ESPNow Init Failed....");
	}
	addMasterAsPeer();

	esp_now_register_recv_cb(OnDataRecv);
	esp_now_register_send_cb(OnDataSent);

}

void loop()
{
	yield();
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
	Serial.printf("\r\nReceived\t%d Bytes\t%d", data_len, data[0]);
	memcpy( dataToSend, data, data_len );

	sendResult = esp_now_send(master.peer_addr, dataToSend, maxDataFrameSize);
	if (sendResult == ESP_OK) {
		Serial.println("Success");
	} else if (sendResult == ESP_ERR_ESPNOW_NOT_INIT) {
		// How did we get so far!!
		Serial.println("ESPNOW not Init.");
	} else if (sendResult == ESP_ERR_ESPNOW_ARG) {
		Serial.println("Invalid Argument");
	} else if (sendResult == ESP_ERR_ESPNOW_INTERNAL) {
		Serial.println("Internal Error");
	} else if (sendResult == ESP_ERR_ESPNOW_NO_MEM) {
		Serial.println("ESP_ERR_ESPNOW_NO_MEM");
	} else if (sendResult == ESP_ERR_ESPNOW_NOT_FOUND) {
		Serial.println("Peer not found.");
	}
	else if (sendResult == ESP_ERR_ESPNOW_IF) {
		Serial.println("Interface Error.");
	}   else {
		Serial.printf("\r\nNot sure what happened\t%d", sendResult);
	}
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
	Serial.print("\r\nLast Packet Send Status:\t");
	Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
