#include "ESPClock_8574_1602.h"

ESPClock_8574_1602::ESPClock_8574_1602(uint8_t addr, uint8_t sda, uint8_t scl) 
    : _lcd(addr, 16, 2), _sda(sda), _scl(scl), _addr(addr) {}

void ESPClock_8574_1602::begin() {
    Wire.begin(_sda, _scl);
    Wire.setTimeOut(50); 
    if (isLcdConnected()) {
        _lcd.init();
        _lcd.backlight();
        _lcd.clear();
    }
}

bool ESPClock_8574_1602::isLcdConnected() {
    Wire.beginTransmission(_addr);
    return (Wire.endTransmission() == 0);
}

bool ESPClock_8574_1602::syncTimeAndOffline(const char* ssid, const char* password) {
    if (isLcdConnected()) {
        _lcd.clear();
        _lcd.print("Connecting WiFi");
    }

    WiFi.begin(ssid, password);
    int retry = 0;
    while (WiFi.status() != WL_CONNECTED && retry < 40) {
        delay(500);
        retry++;
    }

    if (WiFi.status() == WL_CONNECTED) {
        configTime(8 * 3600, 0, "ntp.aliyun.com", "time.apple.com");
        int timeRetry = 0;
        while (!getLocalTime(&_timeinfo) && timeRetry < 10) {
            delay(500);
            timeRetry++;
        }
        
        WiFi.disconnect(true);
        WiFi.mode(WIFI_OFF); // 关闭WiFi释放资源给蓝牙

        if (isLcdConnected()) {
            _lcd.clear();
            _lcd.print("Sync Success!");
            delay(1500);
            _lcd.clear();
        }
        return true;
    }
    WiFi.mode(WIFI_OFF);
    return false;
}

// ---------------------------------------------------------
// 模式1：标准日期时间模式
// ---------------------------------------------------------
void ESPClock_8574_1602::updateDisplay() {
    if (!isLcdConnected()) return; 
    if (!getLocalTime(&_timeinfo)) return;

    char dBuf[17], tBuf[17];
    strftime(dBuf, sizeof(dBuf), "%Y-%m-%d", &_timeinfo);
    strftime(tBuf, sizeof(tBuf), "%H:%M:%S", &_timeinfo);

    _lcd.setCursor(0, 0);
    _lcd.print("Date: "); _lcd.print(dBuf);
    _lcd.print("  "); // 覆盖旧残影

    _lcd.setCursor(0, 1);
    _lcd.print("Time: "); _lcd.print(tBuf);
    _lcd.print("  "); // 解决末尾字母F残留问题
}

// ---------------------------------------------------------
// 模式2：复合计数模式
// ---------------------------------------------------------
void ESPClock_8574_1602::updateDisplayWithCounter(long counter, const char* label) {
    if (!isLcdConnected() || !getLocalTime(&_timeinfo)) return;

    char line0[17]; // 第一行缓存
    char line1[17]; // 第二行缓存

    // 第一行显示完整的月-日 时:分:秒 (共14位)
    strftime(line0, sizeof(line0), "%m-%d %H:%M:%S", &_timeinfo);

    // 第二行显示自定义标签和数字
    snprintf(line1, sizeof(line1), "%s: %ld", label, counter);

    _lcd.setCursor(0, 0);
    _lcd.print(line0);
    _lcd.print("  "); // 覆盖残影

    _lcd.setCursor(0, 1);
    _lcd.print(line1);
    _lcd.print("      "); // 覆盖长数字变短后的残影
}
