#include "smartconfig.h"

#include "led.h"

Ticker ticker_Smartconfig;

// SmartConfig 任务，如果连接成功就停止
static void SmartConfig_cb(void) {
    if (WiFi.smartConfigDone()) {
        Set_LED_mode(ZQ_STATUS_OK, 500);
#if DEBUG_PRINTF
        Serial.println("SmartConfig Success!");
        Serial.printf("SSID:%s\r\n", WiFi.SSID().c_str());
        Serial.printf("PASSWORD:%s\r\n", WiFi.psk().c_str());
        Serial.printf("IP address: ");
        Serial.println(WiFi.localIP());
#endif
        zqStatus_t.SMARTCONFIG_STATUS = ZQ_STATUS_OK;
        zqStatus_t.WIFI_STATUS = ZQ_STATUS_OK;
        zqStatus_t.TCP_STATUS = ZQ_STATUS_WAIT;
        ticker_Smartconfig.detach();
    } else {
        zqStatus_t.SMARTCONFIG_STATUS = ZQ_STATUS_WAIT;
#if DEBUG_PRINTF
        Serial.println("Please start network configuration!");  //等待配网
#endif
    }
}

void smartconfig_done() {}
// smartconfig 初始化任务等待配网
void Smartconfig_init(void) {
    WiFi.beginSmartConfig();                            //开启smartconfig
    ticker_Smartconfig.attach_ms(500, SmartConfig_cb);  // smartconfig任务
}
