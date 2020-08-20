#include <Arduino.h>
#include <TCP.h>
#include <key.h>
#include <led.h>
#include <main.h>
#include <smartconfig.h>

char cmodata[1460];
uint16_t RX_SERIAL_CNT = 0;

Ticker ticker_wifi;
Ticker tickerSerial;
zqSTATUS_t zqStatus_t = {ZQ_DEFAULT, ZQ_DEFAULT, ZQ_DEFAULT, ZQ_DEFAULT,
                         ZQ_STATUS_OK};

//检测wifi连接状态
void Check_station_connect(void) {
    switch (WiFi.status()) {
        case WL_CONNECTED:
#if DEBUG_PRINTF
            Serial.println("WL_CONNECTED.");
#endif
            Set_LED_mode(ZQ_STATUS_OK, 500);
            if (WiFi.SSID()) {
#if DEBUG_PRINTF
                Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
                Serial.printf("PASSWORD:%s\r\n", WiFi.psk().c_str());
                Serial.printf("IP address:%s\r\n",
                              WiFi.localIP().toString().c_str());
#endif
                zqStatus_t.WIFI_STATUS = ZQ_STATUS_OK;
                ticker_wifi.detach();
                break;
                case WL_NO_SSID_AVAIL:
#if DEBUG_PRINTF
                    Serial.println("WL_NO_SSID_AVAIL.");
#endif
                    break;
                case WL_CONNECT_FAILED:
#if DEBUG_PRINTF
                    Serial.println("WL_CONNECT_FAILED.");
#endif
                    break;
                case WL_IDLE_STATUS:
#if DEBUG_PRINTF
                    Serial.println("WL_IDLE_STATUS.");
#endif
                    ticker_wifi.detach();
                    Set_LED_mode(ZQ_STATUS_WAIT, 10);
                    Smartconfig_init();  //创建smartconfig 任务
                    break;
                case WL_DISCONNECTED:
                    zqStatus_t.WIFI_STATUS = ZQ_STATUS_FAIL;
#if DEBUG_PRINTF
                    Serial.println("WL_DISCONNECTED.");
#endif
                    break;
                default:
                    break;
            }
    }
}
void Serial_callback() {
    if (Serial.available() <= 0) {
        zqStatus_t.SERIAL_STATUS = ZQ_STATUS_OK;
    }
}

void setup() {
    // put your setup code here, to run once:
    Serial.begin(115200);
    Serial.println();
    Key_init();
    LED_init();
    WiFi.mode(WIFI_STA);
    ticker_wifi.attach_ms(500, Check_station_connect);
}

void loop() {
    Check_TCP_Connect();
    if (Serial.available() > 0) {
        zqStatus_t.SERIAL_STATUS = ZQ_STATUS_WAIT;
        cmodata[RX_SERIAL_CNT++] = char(Serial.read());
        tickerSerial.once_ms(100, Serial_callback);
    }
    if ((RX_SERIAL_CNT > 0) && (zqStatus_t.SERIAL_STATUS == ZQ_STATUS_OK)) {
        Send_TCP_DATA((char *)cmodata, RX_SERIAL_CNT);
        memset((char *)cmodata, 0, RX_SERIAL_CNT);
        RX_SERIAL_CNT = 0;
    }
}