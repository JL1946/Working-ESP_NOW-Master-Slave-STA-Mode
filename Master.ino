#include <Arduino.h>
#include <esp_now.h>
#include <WiFi.h>
#include <esp_wifi.h>

#define WIFI_CHANNEL 1
const uint8_t slaveCustomMac[] = {0xB7, 0xE6, 0x2D, 0xB2, 0x1B, 0x36};  //Custom mac address for Remote Slave
const uint8_t masterCustomMac[] = {0x30, 0xAE, 0xA4, 0x89, 0x92, 0x71}; //Custom mac address for This Master
const uint8_t maxDataFrameSize=200;
esp_now_peer_info_t slave;
const esp_now_peer_info_t *peer = &slave;
uint8_t dataToSend[maxDataFrameSize];
byte cnt=0;

/*-------------------Declarations-------------------*/
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status);
void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len);

void initVariant()
{
	Serial.println("In initVariant"); // esp_base_mac_addr_set
	WiFi.mode(WIFI_MODE_STA);                                                          // WIFI_STA Mode
	esp_err_t setMacResult = esp_wifi_set_mac(ESP_IF_WIFI_STA, &masterCustomMac[0]);   // #include <esp_wifi.h> required to compile this line
	Serial.print("setMacResult Status: "); Serial.println(setMacResult);
}

void addSlaveAsPeer()
{
	memcpy( &slave.peer_addr, &slaveCustomMac, 6 );
	slave.channel = WIFI_CHANNEL;
	slave.encrypt = 0;
	if( esp_now_add_peer(peer) == ESP_OK)
	{
		Serial.println("Slave Added As Peer!");
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
		Serial.println("ESP NOW INIT!");
	}
	else
	{
		Serial.println("ESP NOW INIT FAILED....");
	}
	addSlaveAsPeer();

	esp_now_register_send_cb(OnDataSent);
	esp_now_register_recv_cb(OnDataRecv);
}

void loop()
{
	for(cnt=0; cnt<maxDataFrameSize; cnt++)
	{
		dataToSend[cnt]++;
	}
	if( esp_now_send(slave.peer_addr, dataToSend, maxDataFrameSize) == ESP_OK)
	{
		Serial.printf("\r\nSuccess Sent Value->\t%d", dataToSend[0]);
	}
	else
	{
		Serial.printf("\r\nDID NOT SEND....");
	}
	delay(250);
}

void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status)
{
	Serial.print("\r\nLast Packet Send Status:\t");
	Serial.print(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}

void OnDataRecv(const uint8_t *mac_addr, const uint8_t *data, int data_len)
{
	Serial.printf("\r\nReceived\t%d Bytes\t%d", data_len, data[0]);
}
