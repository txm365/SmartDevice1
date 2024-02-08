// u8g2_instances.cpp

#include "u8g2_functions.h"

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2_instance(U8G2_R0, /* clock=*/26, /* data=*/25, /* reset=*/U8X8_PIN_NONE);
U8g2Functions u8g2Functions(u8g2_instance);

