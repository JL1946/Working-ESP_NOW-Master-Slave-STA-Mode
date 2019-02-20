# Working-ESP_NOW-Master-Slave-STA-Mode

There is a Catch-22 built into the ESP-NOW function, wherein, if a Sender does not know the MAC address of the Receiver or
vice-versa, data transfers will not occur and the Receiver won't be able to capture the Sender's MAC address from 
the data packet if the Sender does not know the MAC address of the Receiver to which it is sending the data packet.

One could argue that if the Sender uses Broadcast instead of Unicast, The Receiver will capture the Sender's MAC address, but
this method does not always work (does not work for ESP8266 devices), and may be hit-or-miss for ESP32 devices. The net result
is the user scratching his head, not understanding why transfers are not occurring, and endless searches for solutions.

When using ESP_NOW to transfer data from Master-to-Slave/Slaves or Slave/Slaves-to-Master, the least complicated method
is to pre-assign a known MAC address to each device.

The ESP32 firmware modifies the Factory installed Base MAC address to assign a MAC address whenever a device is programmed
as an Access Point. Even a custom MAC is modified by the firmware if AP Mode is programmed. 

However, MAC addresses assigned to Station Mode are not changed by the firmware.

The MAC address can be assigned with a call to the esp_wifi_set_mac(ESP_IF_WIFI_STA, &CustomMac[0]) function.

Therefore, by setting the Mode to STA for all devices, the MAC address can be directly assigned to each device, peered
in the Setup routine, and will not change on reboot/restart.

This method will allow ESP-NOW transfers to occur successfully because the correct MAC addresses are peered during Setup
for the Sender and Receiver and not as a result of an "OnDataReceived" or other such routine.

Multiple Slaves can be peered in the Master sketch and added to the Slave sketch for each Slave. Yes... you wind up having
multiple Slave sketches due to the MAC address setup, but that is a small price to pay for successful data transfers.
