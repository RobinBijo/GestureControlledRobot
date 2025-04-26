# README -> READ THIS BEFORE CONTINUING
## Key Changes 
1. **WHAT: Removed `#include <Arduino.h>` from the headers** <br>
WHY: When you use the espressif library in the Additional Boards URL Arduino compilation is used by default
2. **WHAT: Baud rate changed to 115200 in `Serial.begin(115200);`**<br>
WHY: ESP32 is designed to communicate on 115200 when using WiFi(Acc:Espressif Systems).However too high the rate less stable communication
4. **WHAT: `ledcSetup` and `ledcAttachPin` were removed and `analogWrite(**args)` is used**<br>
WHY: `ledcSetup` and `ledcAttachPin` are not supported fully in Arduino2 and works mostly in PlatformIO

Co-authored and git pushed to Github with ❤ by @Swastikvoid 
