#include "led.h"

Ticker ticker_System_LED;
static uint8_t LED_Negate_flag = 0;
static uint8_t LED_Breathe_flag = 0;
static int fadeValue = 0;

void Fail_led(void) {
    digitalWrite(LED_SYSTEM, LOW);
    delay(40);
    digitalWrite(LED_SYSTEM, HIGH);
}

void blink_led(void) {
    if (LED_Negate_flag) {
        LED_Negate_flag = 0;
        digitalWrite(LED_SYSTEM, HIGH);
    } else {
        LED_Negate_flag = 1;
        digitalWrite(LED_SYSTEM, LOW);
    }
}

/*
模拟输出 Analog output
analogWrite(pin, value) 在已有的引脚上使能软件PWM功能。PWM可以用在引脚0~16。
调用analogWrite(pin, 0) 可以关闭引脚PWM。取值范围：0~ PWMRANGE,默认为1023。
PWM 范围可以使用analogWriteRange(new_range)语句来更改。
PWM 默认频率：1KHz。使用analogWriteFreq(new_frequency) 可以更改频率。
*/
void Breathe_led(void) {
    if (LED_Breathe_flag == 0) {
        fadeValue += 5;
        if (fadeValue >= 1023) LED_Breathe_flag = 1;
        analogWrite(LED_SYSTEM, fadeValue);  // val 范围0-1023
    } else {
        fadeValue -= 5;
        if (fadeValue <= 400) {
            LED_Breathe_flag = 0;
        }
        analogWrite(LED_SYSTEM, fadeValue);
    }
}

void System_LED_cb(void) {
    switch (zqStatus_t.LED_STATUS) {
        case ZQ_STATUS_OK:
            blink_led();
            break;
        case ZQ_STATUS_FAIL:
            Fail_led();
            break;
        case ZQ_STATUS_WAIT:
            Breathe_led();
            break;
        default:
            break;
    }
}

void Set_LED_mode(ZQ_STATUS_t zst, uint32_t ledms) {
    zqStatus_t.LED_STATUS = zst;
    ticker_System_LED.attach_ms(ledms, System_LED_cb);
}

//初始化LED  出低亮
void LED_init(void) {
    pinMode(LED_SYSTEM, OUTPUT);
    Set_LED_mode(ZQ_STATUS_OK, 100);
}
