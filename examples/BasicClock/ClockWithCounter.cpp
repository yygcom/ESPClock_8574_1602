#include <ESPClock_8574_1602.h>

ESPClock_8574_1602 myClock(0x3F, 8, 9);
long myCount = 0;
bool isWorking = false; // 状态标志位

void displayTask(void *pvParameters) {
    for (;;) {
        if (!isWorking) {
            // 模式1：大字显示日期和时间
            myClock.updateDisplay();
        } else {
            // 模式2：显示时间的同时显示计数器
            myClock.updateDisplayWithCounter(myCount, "Steps");
        }
        vTaskDelay(1000 / portTICK_PERIOD_MS);
    }
}

void setup() {
    myClock.begin();
    myClock.syncTimeAndOffline("SSID", "PASS");

    // 模拟某种业务逻辑：5秒后进入工作状态并开始计数
    xTaskCreate(displayTask, "LCD", 4096, NULL, 1, NULL);
    
    delay(5000);
    isWorking = true;
}

void loop() {
    if (isWorking) {
        myCount++;
        delay(500); // 计数增加
    }
}
