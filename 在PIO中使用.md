### 1. 安装与创建项目
1.  在 VS Code 中安装 **PlatformIO IDE** 插件。
2.  点击 PIO 图标 -> **Project Examples** -> **Create New Project**。
3.  **Name**: `ESP32C3_Clock`
4.  **Board**: 搜索并选择 `Espressif ESP32-C3 DevKitM-1` (或者你具体的 C3 板型号)。
5.  **Framework**: `Arduino`。

---

### 2. 配置文件 `platformio.ini` (核心步骤)
PIO 的核心是根目录下的 `platformio.ini`。你只需要在这里写好依赖，它会自动帮你下载 `LiquidCrystal_I2C` 和 GitHub 库。

将文件内容修改为：

```ini
[env:esp32c3]
platform = espressif32
board = esp32-c3-devkitm-1
framework = arduino

; 串口监视器波特率
monitor_speed = 115200

; 自动下载依赖库
lib_deps =
    # 1. 自动从 GitHub 下载你的时钟库
    https://github.com/yygcom/ESPClock_8574_1602.git
    
    # 2. 自动下载 LCD 依赖库并锁定版本
    marcoschwartz/LiquidCrystal_I2C @ ^1.1.4

; 如果你的 ESP32-C3 串口无法打印，开启这个 (针对内置 USB 转串口)
build_flags = 
    -D ARDUINO_USB_MODE=1
    -D ARDUINO_USB_CDC_ON_BOOT=1
```

---

### 3. 编写主代码
在 PIO 中，主文件是 `src/main.cpp`。
**注意**：PIO 的 `.cpp` 文件必须包含 `#include <Arduino.h>`。

```cpp
#include <Arduino.h>
#include <ESPClock_8574_1602.h>

// 实例化 (地址, SDA, SCL)
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
    if (myClock.syncTimeAndOffline("你的WiFi", "密码")) {
        Serial.println("Sync Success!");
    }

    xTaskCreate(displayTask, "LCD_Task", 4096, NULL, 1, NULL);
}

void loop() {
    // 主循环业务
    delay(10);
}
```

---

### 4. 编译与上传
在 VS Code 底部状态栏有几个图标：
-   **✔ (Build)**: 编译代码。第一次点击时，PIO 会自动下载 `LiquidCrystal_I2C`。
-   **→ (Upload)**: 上传到板子。
-   **🔌 (Serial Monitor)**: 打开串口监视器查看对时情况。
