#ifndef __MAIN_H_
#define __MAIN_H_


// debug打印标志位
#define DEBUG_PRINTF 1

//枚举状态
enum ZQ_STATUS_t { ZQ_DEFAULT,ZQ_STATUS_OK, ZQ_STATUS_FAIL, ZQ_STATUS_WAIT };

//状态类型
typedef struct ZQ_STATUS {
    ZQ_STATUS_t WIFI_STATUS;
    ZQ_STATUS_t SMARTCONFIG_STATUS;
    ZQ_STATUS_t SERIAL_STATUS;
    ZQ_STATUS_t TCP_STATUS;
    ZQ_STATUS_t LED_STATUS;
} zqSTATUS_t;

//全局状态
extern zqSTATUS_t zqStatus_t;
extern Ticker ticker_System_LED;

#endif






