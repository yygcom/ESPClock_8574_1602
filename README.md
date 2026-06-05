# ESPClock_8574_1602

这是一个专为 ESP32 系列（包括 **ESP32、ESP32-C3、ESP32-S3**）设计的网络对时时钟库。它配合带有 **PCF8574** 转换器的 **LCD1602** 液晶屏使用，具有极高的稳定性。

## 核心特性

- **蓝牙兼容模式**：对时成功后自动执行 `WiFi.mode(WIFI_OFF)`。由于 ESP32 的 WiFi 和蓝牙共用天线，关闭 WiFi 可确保蓝牙获得 100% 的射频性能。
- **防卡死设计**：在每次写入 LCD 前都会进行 I2C 握手检查。即使在运行中拔掉 LCD 屏幕，主程序（及蓝牙业务）依然能流畅运行，不会因 I2C 阻塞导致 CPU 触发看门狗重启。
- **任务化驱动**：完美适配 FreeRTOS。建议在独立任务中每秒刷新一次，不占用主循环（`loop`）资源。
- **残影修复**：优化了显示逻辑，彻底解决了由于时间字符串长度变化导致的屏幕末尾“字母残留”问题。

## 依赖库

本库依赖以下第三方库，请务必预先安装：
- **LiquidCrystal I2C** (作者：Marco Schwartz, 建议版本：v1.1.4)

## I2C 地址全表

根据您转接板上芯片型号和焊盘（A0, A1, A2）的状态，请参考下表设置地址。
*注：**悬空** = 逻辑 1 (默认)；**短路** = 逻辑 0 (焊接连通)*

| A2 | A1 | A0 | PCF8574T 系列地址 | PCF8574AT 系列地址 |
| :--- | :--- | :--- | :--- | :--- |
| 悬空 | 悬空 | 悬空 | **0x27** (默认) | **0x3F** (默认) |
| 悬空 | 悬空 | 短路 | 0x26 | 0x3E |
| 悬空 | 短路 | 悬空 | 0x25 | 0x3D |
| 悬空 | 短路 | 短路 | 0x24 | 0x3C |
| 短路 | 悬空 | 悬空 | 0x23 | 0x3B |
| 短路 | 悬空 | 短路 | 0x22 | 0x3A |
| 短路 | 短路 | 悬空 | 0x21 | 0x39 |
| 短路 | 短路 | 短路 | 0x20 | 0x38 |

## 快速上手

### 1. 硬件连接 (以 ESP32-C3 为例)
- **VCC** -> 5V (3.3V 可能会导致屏幕对比度不足)
- **GND** -> GND
- **SDA** -> GPIO 8
- **SCL** -> GPIO 9

### 2. 示例代码
```cpp
#include <ESPClock_8574_1602.h>

// 参数：I2C地址, SDA引脚, SCL引脚
ESPClock_8574_1602 myClock(0x3F, 8, 9);

// 定义后台刷新任务
void displayTask(void *pvParameters) {
    while (1) {
        myClock.updateDisplay();
        vTaskDelay(1000 / portTICK_PERIOD_MS); // 每秒刷新一次
    }
}

void setup() {
    Serial.begin(115200);
    
    // 初始化并连接 WiFi 对时
    myClock.begin();
    if (myClock.syncTimeAndOffline("WiFi名称", "密码")) {
        Serial.println("对时成功，WiFi 已关闭，现在可以启动蓝牙了。");
    }

    // 启动 FreeRTOS 任务
    xTaskCreate(displayTask, "LCD_Task", 4096, NULL, 1, NULL);
}

void loop() {
    // 你的其他逻辑（如蓝牙通信）
    delay(10);
}
```

## 安装方法

1. 将本仓库下载为 ZIP 文件。
2. 在 Arduino IDE 中选择：`项目` -> `包含库` -> `添加 .ZIP 库`。
3. 或者直接将源码克隆到你的 `Documents/Arduino/libraries` 目录下。

## 在PIO中使用
[在PIO中使用.md](在PIO中使用.md)

## C3 super mini BUG
```
; 👇 在这里手动注入一个自定义的宏，专门标记这是 SuperMini 硬件缺陷板
build_flags = 
    -D ESP32C3_SUPERMINI_BUGGY_WIFI
```

## 许可证
[MIT](LICENSE)
