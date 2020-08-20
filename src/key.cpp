#include "key.h"

#include "TCP.h"
#include "smartconfig.h"
Ticker ticker_KEY;

uint8_t KeyValueNUM = 0;
/*按键检测，进入Airkiss/espTOUCH模式配网
 * */
static void Break_Wifi_Connect(void) {
    system_restore();
    system_restart();
}

//按键扫描任务
void Key_Setting_cb(void) {
    if (digitalRead(KEY_BUTTON) == LOW) {
        KeyValueNUM++;
        if (KeyValueNUM >= 100) {
            KeyValueNUM = 0;
#if DEBUG_PRINTF
            Serial.println("Enter Airkiss mode!\n");
#endif
            Close_ZQ();
            Break_Wifi_Connect();
        }
    } else {
        KeyValueNUM = 0;
    }
}

//初始化按键
void Key_init(void) {
    pinMode(KEY_BUTTON, INPUT);
    ticker_KEY.attach_ms(50, Key_Setting_cb);  // smartconfig任务
}