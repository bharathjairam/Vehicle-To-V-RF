// Include RadioHead Amplitude Shift Keying Library
#include <RH_ASK.h>
// Include dependant SPI Library
#include <SPI.h>

#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27,16, 2);

byte degree_symbol[8] =
{
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};
// Define output strings

String str_dist;
String str_speed;
String str_out;

// Create Amplitude Shift Keying Object
RH_ASK rf_driver;

void setup()
{
  // Initialize ASK Object
  rf_driver.init();
  // Setup Serial Monitor
  Serial.begin(9600);

  lcd.init();   // initializing the LCD
  lcd.clear();
  lcd.backlight(); // Enable or Turn On the backlight
}

void loop()
{
  // Set buffer to size of expected message
  byte distance = 0;
  byte Speed = 0;
  uint8_t buf[11];
  uint8_t buflen = sizeof(buf);
  // Check if received packet is correct size
  if (rf_driver.recv(buf, &buflen))
  {

    // Message received with valid checksum
    // Get values from string

    // Convert received data into string
    str_out = String((char*)buf);

    // Split string into two values
    for (int i = 0; i < str_out.length(); i++)
    {
      if (str_out.substring(i, i + 1) == ",")
      {
        str_dist = str_out.substring(0, i);
        str_speed = str_out.substring(i + 1);
        break;
      }

    }

    // Print values to Serial Monitor
    Serial.print("Distance: ");
    Serial.print(str_dist);
    lcd.setCursor(0, 0);
    lcd.print("Distance: ");
    lcd.setCursor(9, 0);
    lcd.print(str_dist);
    lcd.setCursor(12, 0);
    lcd.print("Cm");
    
    Serial.print("  - Speed: ");
    Serial.println(str_speed);
    int SPD = str_speed.toInt();
    if (SPD >> 0)
    {
    lcd.setCursor(0, 1);
    lcd.print("Speed: ");
    lcd.setCursor(6,1);
    lcd.print(str_speed);
    lcd.setCursor(10, 1);
    lcd.print("Kmph");
    }
   
    
    delay(2000);
     lcd.clear();
  }


}
