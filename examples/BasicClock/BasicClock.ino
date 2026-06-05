#include <ESPClock_8574_1602.h>

// ESP32-C3 示例引脚 (SDA=8, SCL=9)
ESPClock_8574_1602 myClock(0x3F, 8, 9);

void displayTask(void *pvParameters) {
    for (;;) {
        myClock.updateDisplay();
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    Serial.begin(115200);
    myClock.begin();

    // 联网并同步时间
    if (myClock.syncTimeAndOffline("YourSSID", "YourPassword")) {
        Serial.println("Time synced, WiFi is now OFF.");
    }

    // 创建后台显示任务
    xTaskCreate(displayTask, "LCD_Task", 4096, NULL, 1, NULL);
}

void loop() {
    // 此时你可以自由使用蓝牙，WiFi已关闭
    delay(1000);
}
