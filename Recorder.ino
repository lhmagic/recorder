String    dirName = "NONAME";
String    filePrefix = "NONAME";
String    bluetoothID = "0x0000000000";
String    fileName = "RECORD.TXT";

#include <RBD_Timer.h>
#include <RBD_Button.h>

// input pullup enabled by default
RBD::Button button(7);

uint8_t press_cnt;
long timer, oled_off_timer;
String record, upload;

void setup() {
  Serial.begin(115200);
  oledInit();
  wlInit();
  sensorInit();
  gpsInit();
  sdInit();
  gen_readme();
  getConfig("CONFIG.TXT");
  btInit(bluetoothID);
  press_cnt = 0;

  timer = millis();
}

void loop() {
String heartRate;

  if(timer > millis()) 
    timer = millis();

  if(oled_off_timer > millis()) 
    oled_off_timer = millis();   

  if(millis() - timer >= 3000) {
    timer = millis();
    Serial3.flush();
  }

  record = getGPS();

  if (record.length() != 0) {
    timer = millis();
    record += getHeadingDegrees()+getTemperature()+\
              getSealevelPressure()+getAccel()+getGyro()+getHeartRate();
//    if(getFileSize(fileName) >= 100) {
//      fileName = nextFileName(dirName, filePrefix)+".TXT";
//    }
    upload = record;
    writeLine(fileName, record);
    disp(record, press_cnt);
  }
  
  if(button.onPressed()) {
    oled_off_timer = millis();
    if(++press_cnt >= 4) {
      press_cnt = 1;
    }
    disp(upload, press_cnt);
  }

  if(millis() - oled_off_timer >= 10000) {
    press_cnt = 0;
    disp_clear();
  }
  
  rfHandle(upload);
}

void getConfig(String fileName) {
String str;
  while(str = readLine(fileName)) {
    if(str.length()==0) break;
    if(str.startsWith("BTID:")) {
      bluetoothID = str.substring(5);
    }
  }  
}

void gen_readme(void) {
String msg = "\
\r\n数据每秒记录一次(无效的数据用'#'表示)，包含的内容有：\r\n\
\r\n当前时间 | 纬度(°) | 经度(°) | 海拔(m) | 速度(km/h) | 朝向(°) | 温度(℃) | 气压(Pa) | [x,y,z]加速度(m/s2) | [x,y,z]角速度(°/s) | 心率(times/min)";  
  if(!file_exist("README.TXT")) {
    Serial.println("Readme file does not exist, create it.");
    writeLine("README.TXT", msg);
  }
}
