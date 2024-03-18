
// modified by Andi Ikhwan March,17, 2024

#include "BME280.h"
#include "U8glib.h" 
//Initialize display.
U8GLIB_SSD1306_128X64 u8g(U8G_I2C_OPT_NONE | U8G_I2C_OPT_DEV_0);
BME280 bme(Wire,0x76);

#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#include <Wire.h>
#include <DS3231.h>

DS3231 clock;
RTCDateTime dt;


LiquidCrystal_I2C lcd(0x27,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display

void setup()
{

  Serial.begin(115200);
  Serial.println("reset");
  if (bme.begin() < 0) {
    Serial.println("Error communicating with sensor, check wiring and I2C address");
    while(1){}
  }
  u8g.setFont(u8g_font_unifont);
  
 
   
  clock.begin();
  // Set sketch compiling time
  clock.setDateTime(__DATE__, __TIME__);
  lcd.init();                      // initialize the lcd 
  //lcd.init();
  // Print a message to the LCD.
  lcd.backlight();
  lcd.setCursor(0,0);
  dt = clock.getDateTime();
  lcd.printf("%d/%02d/%02d",dt.year,dt.month,dt.day);
  lcd.setCursor(0,1);
  lcd.print(" < TESTING I2C >  ");

   
  
}


void loop()
{
  dt = clock.getDateTime();
  lcd.setCursor(0,0);
  lcd.printf("%d/%02d/%02d %02d:%02d",dt.year,dt.month,dt.day,
              dt.hour,dt.minute);
  delay(500);
  lcd.setCursor(0,0);
  lcd.printf("%d/%02d/%02d %02d %02d",dt.year,dt.month,dt.day,
              dt.hour,dt.minute);
  delay(500);

   u8g.firstPage();
    do {
        draw();
       } while (u8g.nextPage());

    bme.readSensor();

  // displaying the data
  Serial.print(bme.getPressure_Pa(),6);
  Serial.print("\t");
  Serial.print(bme.getTemperature_C(),2);
  Serial.print("\t");
  Serial.println(bme.getHumidity_RH(),2);   
}

void draw(void)
{
    //Write text. (x, y, text)
    u8g.drawStr(10, 10, " TEMPERATURE  ");

    uint32_t mtemp1;
    uint32_t mtemp2;
    mtemp1 =  (uint32_t) bme.getTemperature_C();
    mtemp2 =  (uint32_t) (bme.getTemperature_C() *100);
    mtemp2 = mtemp2 % 100;
    char stemp[40];
    sprintf(stemp,"     %ld.%02d  ",mtemp1,(uint16_t) mtemp2);
    u8g.drawStr(10, 22, stemp);
    u8g.drawStr(10, 40, "   HUMIDITY  ");
    mtemp1 =  (uint32_t) bme.getHumidity_RH();
    mtemp2 =  (uint32_t) (bme.getHumidity_RH() *100);
    mtemp2 = mtemp2 % 100;
      sprintf(stemp,"     %ld.%02d  ",mtemp1,(uint16_t) mtemp2);
  
    u8g.drawStr(10, 52, stemp);
}
