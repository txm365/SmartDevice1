// u8g2_functions.cpp

#include "u8g2_functions.h"


//#include "u8g2_instances.cpp"
// Definitions for the image variables
/*Batt*/const unsigned char image_Battery_26x8_bits[] U8X8_PROGMEM = {0xfe,0xff,0x7f,0x00,0x01,0x00,0x80,0x00,0x01,0x00,0x80,0x03,0x01,0x00,0x80,0x02,0x01,0x00,0x80,0x02,0x01,0x00,0x80,0x03,0x01,0x00,0x80,0x00,0xfe,0xff,0x7f,0x00};
/*Volu*/const unsigned char image_Volup_8x6_bits[] U8X8_PROGMEM = {0x48,0x8c,0xaf,0xaf,0x8c,0x48};
/*WiFi*/const unsigned char image_ir_10px_bits[] U8X8_PROGMEM = {0xfc,0x00,0x02,0x01,0x79,0x02,0x84,0x00,0x30,0x00,0x00,0x00,0x30,0x00,0x58,0x00,0x78,0x00,0xff,0x03};
/* BT */const unsigned char image_Bluetooth_Idle_5x8_bits[] U8X8_PROGMEM = {0x04,0x0d,0x16,0x0c,0x0c,0x16,0x0d,0x04};

U8g2Functions::U8g2Functions(U8G2_SSD1306_128X64_NONAME_F_SW_I2C &u8g2) : u8g2(u8g2) {}

 void U8g2Functions::u8g2Initialization(){
  u8g2.begin();
}
void U8g2Functions::prepare() {
  u8g2.setFont(u8g2_font_6x10_tf);
  u8g2.setFontRefHeightExtendedText();
  u8g2.setDrawColor(1);
  u8g2.setFontPosTop();
  u8g2.setFontDirection(0);
}
void U8g2Functions::welcomePage(){
   u8g2Functions.headerIcons();
   u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(22, 30, "WELCOME");
   u8g2.sendBuffer();
 }
void U8g2Functions::homePage() {
   u8g2Functions.headerIcons();
  struct tm timeinfo;
  char homeTime[6];
  char homeDay[10];
  char homeDate[18];

  // Get time information
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(homeTime, sizeof(homeTime), "%H:%M", &timeinfo);
  strftime(homeDay, sizeof(homeDay), "%A", &timeinfo);
  strftime(homeDate, sizeof(homeDate), "%d %B %Y", &timeinfo);
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(35, 18, homeTime);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(37, 36, homeDay);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(14, 48, homeDate);
  u8g2.sendBuffer();
}

void U8g2Functions::tempPage() {
   u8g2Functions.headerIcons();
   float temperature = 35.7;
  char tmp[5] = "";
  dtostrf(temperature, 4, 2, tmp);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(29, 20, "Temperature");
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(22, 37, tmp);
  u8g2.drawStr(100, 37, " °C");
  u8g2.sendBuffer();
}
void U8g2Functions::humPage() {
   u8g2Functions.headerIcons();
   float humidity =  68.5;
  char hum[5] = "";
  dtostrf(humidity, 4, 2, hum);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(29, 20, "Humidity");
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(22, 37, hum);
  u8g2.drawStr(90, 37, "%");
  u8g2.sendBuffer();
}
void U8g2Functions::presPage() {
   u8g2Functions.headerIcons();
   float pressure = 1005.6;
  char pres[7] = "";
  dtostrf(pressure, 7, 2, pres);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(29, 20, "Pressure");
  u8g2.setFont(u8g2_font_profont22_tr);
  u8g2.drawStr(15, 37, pres);
  u8g2.drawStr(95, 37, "kPa");
  u8g2.sendBuffer();
}

void U8g2Functions::updatePage(uint8_t progress){
   u8g2Functions.headerIcons();
 char updateProgr[5];
  //u8g2_prepare();
    //software update infomation
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(18, 17, "Firmware Update");                    //update message
  u8g2.drawStr(27, 28, "Downloading");                        //Downloading message
  //Update Status
  sprintf(updateProgr,"%d%%", progress);
  u8g2.drawFrame(12, 39, 104, 13);                               //Update border frame
  u8g2.drawBox(14, 41, progress, 9);                      //update percentage fill
  u8g2.drawStr(53, 55, updateProgr);                             //Update perentage
  u8g2.sendBuffer();
}

void U8g2Functions::headerIcons() {
  u8g2.clearBuffer();
  u8g2Functions.prepare();
  // Battery
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawXBMP(1, 2, 26, 8, image_Battery_26x8_bits);
  u8g2.drawBox(3, 4, 20, 4);
  u8g2.drawStr(29, 2, "99%");
  // Volume, Bluetooth, and WiFi
  u8g2.drawXBMP(66, 2, 8, 6, image_Volup_8x6_bits);
  u8g2.drawXBMP(77, 2, 5, 8, image_Bluetooth_Idle_5x8_bits);
  u8g2.drawXBMP(84, 2, 10, 10, image_ir_10px_bits);
  // Time
  struct tm timeinfo;
  char headerTime[6];
  // Get time information
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(headerTime, sizeof(headerTime), "%H:%M", &timeinfo);
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(96, 2, headerTime);
  // Bottom line for icons
  u8g2.drawLine(0, 12, 128, 12);
  
}
