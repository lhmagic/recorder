#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// If using software SPI (the default case):
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13 
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

void oledInit(void) {
  display.begin(SSD1306_SWITCHCAPVCC);  
  display.clearDisplay();
  display.display();
}

void disp_clear(void) {
  display.clearDisplay();
  display.display();  
}

void disp(String s, uint8_t press_cnt) {
int n;
String data[15];

  if((record.length() == 0) || (press_cnt == 0)) {
    disp_clear();
    return ;
  }

  s += ' ';
  for(n=0; n<15; n++) {
    int index = s.indexOf(' ');
    data[n] = s.substring(0, index+1);
    s = s.substring(index+1);
  }
  
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);  
  display.setCursor(0,0);

  display.print("GPS:");
  if(data[1].startsWith("#")) {
    display.print("NO");
  } else {
    display.print("OK");
  }
  display.print("      HR:");
  display.println(data[14]);
  display.println();

  if(press_cnt == 1) {
    display.setTextSize(2);
    display.print(" ");
    display.print(data[0].substring(2,4));display.print("/");
    display.print(data[0].substring(4,6));display.print("/");
    display.println(data[0].substring(6,8));display.print(" ");
    display.print(data[0].substring(8,10));display.print(":");
    display.print(data[0].substring(10,12));display.print(":");
    display.println(data[0].substring(12,14));
  } else if(press_cnt == 2) {
    display.setTextSize(1);display.print(data[1].substring(10, 11));display.print(": ");display.println(data[1].substring(0,10));
    display.setTextSize(1);display.print(data[2].substring(11, 12));display.print(": ");display.println(data[2].substring(0,11));
    display.setTextSize(1);display.print("H: ");display.setTextSize(2);display.println(data[3]);
    display.setTextSize(1);display.print("V: ");display.setTextSize(2);display.print(data[4]);  
  } else if(press_cnt == 3){
    display.setTextSize(1);display.print("TEMP  ");display.setTextSize(2);display.println(data[6]); 
    display.setTextSize(1);display.print("PRES  ");display.setTextSize(2);display.println(data[7]);
    display.setTextSize(1);display.print("OREN  ");display.setTextSize(2);display.println(data[5]);     
  }
  
  display.display();   
}

void oled_disp_err(String s) {
  display.clearDisplay();
  display.setCursor(0,0);  
  display.setTextSize(2);
  display.println(s);
  display.display();  
}

