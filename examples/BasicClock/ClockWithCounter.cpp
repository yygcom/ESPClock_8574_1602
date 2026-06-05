#include <ESPClock_8574_1602.h>

ESPClock_8574_1602 myClock(0x3F, 8, 9);
long myCount = 0;
bool isWorking = false; // 状态标志位

void displayTask(void *pvParameters) {
    for (;;) {
        // 现在的逻辑：50ms 检查一次
        // 如果计数变了，立即刷第二行
        // 如果计数没变但秒变了，刷第一行
        // 如果都没变，函数内部直接返回，不操作 I2C，不卡 CPU
        myClock.updateDisplayWithCounter(myCount, "Steps");
        
        vTaskDelay(50 / portTICK_PERIOD_MS); 
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
