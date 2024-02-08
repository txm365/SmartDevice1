// u8g2_functions.h

#ifndef U8G2_FUNCTIONS_H
#define U8G2_FUNCTIONS_H

#include <U8g2lib.h>
#include <Adafruit_HTS221.h>
#include <Adafruit_Sensor.h>
// Declare the external variables
extern const unsigned char image_Battery_26x8_bits[] U8X8_PROGMEM;
extern const unsigned char image_Volup_8x6_bits[] U8X8_PROGMEM;
extern const unsigned char image_Bluetooth_Idle_5x8_bits[] U8X8_PROGMEM;
extern const unsigned char image_ir_10px_bits[] U8X8_PROGMEM;

class U8g2Functions {
public:
  U8g2Functions(U8G2_SSD1306_128X64_NONAME_F_SW_I2C &u8g2);
  void prepare();
  void homePage();
  void tempPage();
  void humPage();
  void presPage();
  void headerIcons();
  void updatePage(uint8_t progress);
  void u8g2Initialization();
  void welcomePage();
private:
  U8G2_SSD1306_128X64_NONAME_F_SW_I2C &u8g2;
};
extern U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2_instance;
extern U8g2Functions u8g2Functions;
#endif // U8G2_FUNCTIONS_H
