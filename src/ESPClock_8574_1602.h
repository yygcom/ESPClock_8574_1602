#ifndef ESP_CLOCK_8574_1602_H
#define ESP_CLOCK_8574_1602_H

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <WiFi.h>
#include "time.h"

/* 
 * =================================================================
 * I2C 地址全表 (PCF8574 1602 转接板)
 * =================================================================
 * 说明：短路 = 焊盘连通 (0)；悬空 = 焊盘断开 (1)
 * 
 *     位状态         |   PCF8574T 系列     |   PCF8574AT 系列
 *  A2 | A1 | A0    |   (Arduino 地址)    |   (Arduino 地址)
 * -----------------------------------------------------------------
 *  1  | 1  | 1     |     0x27 (默认)     |     0x3F (默认)
 *  1  | 1  | 0     |     0x26            |     0x3E
 *  1  | 0  | 1     |     0x25            |     0x3D
 *  1  | 0  | 0     |     0x24            |     0x3C
 *  0  | 1  | 1     |     0x23            |     0x3B
 *  0  | 1  | 0     |     0x22            |     0x3A
 *  0  | 0  | 1     |     0x21            |     0x39
 *  0  | 0  | 0     |     0x20            |     0x38
 * =================================================================
 */

class ESPClock_8574_1602 {
public:
    ESPClock_8574_1602(uint8_t addr, uint8_t sda, uint8_t scl);
    void begin();
    bool syncTimeAndOffline(const char* ssid, const char* password); 
    void updateDisplay();
    bool isLcdConnected();

private:
    LiquidCrystal_I2C _lcd;
    uint8_t _sda, _scl, _addr;
    struct tm _timeinfo;
};

#endif
