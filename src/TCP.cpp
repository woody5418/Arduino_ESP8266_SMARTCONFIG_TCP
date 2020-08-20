#include "TCP.h"

const char* host = ZQ_SERVER_IP;
const uint16_t port = ZQ_SERVER_PORT;

WiFiClient client;
Ticker tickerWIFI;

void Connect_WIFI_callback(void) {
    if (WiFi.status() != WL_CONNECTED) {
#if DEBUG_PRINTF
        Serial.print(".");
#endif
        zqStatus_t.WIFI_STATUS = ZQ_STATUS_FAIL;
        tickerWIFI.once_ms(500, Connect_WIFI_callback);
    } else {
        tickerWIFI.detach();
        zqStatus_t.TCP_STATUS = ZQ_STATUS_WAIT;
#if DEBUG_PRINTF
        Serial.println("");
        Serial.println("WiFi connected");
        Serial.println("IP address: ");
        Serial.println(WiFi.localIP());
#endif
        // tickerCreatTCP.once_ms(1, Connect_ZQ_callback);
    }
}

void Close_ZQ(void) {
    // Close the connection
#if DEBUG_PRINTF
    Serial.println();
    Serial.println("closing connection");
#endif
    client.stop();
}

void Check_TCP_Connect(void) {
    if (((zqStatus_t.TCP_STATUS == ZQ_STATUS_WAIT) &&
         (zqStatus_t.WIFI_STATUS == ZQ_STATUS_OK)) ||
        (client.status() == CLOSED)) {
#if DEBUG_PRINTF
        Serial.print("connecting to ");
        Serial.print(host);
        Serial.print(':');
        Serial.println(port);
#endif
        if (!client.connect(host, port)) {
#if DEBUG_PRINTF
            Serial.println("connection failed");
#endif
            delay(5000);
        } else {
#if DEBUG_PRINTF
            Serial.println("connection OK");
#endif
            zqStatus_t.WIFI_STATUS = ZQ_STATUS_OK;
            zqStatus_t.TCP_STATUS = ZQ_STATUS_OK;
        }
    }
}

void Send_TCP_DATA(char* pdata, uint16_t plen) {
    if (zqStatus_t.TCP_STATUS == ZQ_STATUS_OK ||
        zqStatus_t.WIFI_STATUS == ZQ_STATUS_OK) {
        if (client.connected()) {
#if DEBUG_PRINTF
            Serial.write((char*)pdata, plen);
#endif
            client.write((char*)pdata, plen);
        }
    } else {
        if (zqStatus_t.WIFI_STATUS != ZQ_STATUS_OK) {
            Close_ZQ();
            tickerWIFI.once_ms(
                1, Connect_WIFI_callback);  //每500ms调用 tickerCreatTCP
        }
    }
}
