String    dirName = "NONAME";
String    filePrefix = "NONAME";
String    bluetoothID = "0x0000000000";
String    fileName = "RECORD.TXT";

void setup() {
  Serial.begin(115200);
  sensorInit();
  gpsInit();
  sdInit();
  gen_readme();
  getConfig("CONFIG.TXT");
  btInit(bluetoothID);
}

void loop() {
  String record = getGPS();
  if (record.length() != 0) {
    record += getTemperature()+getSealevelPressure()+\
              getHeadingDegrees()+getGyro()+getAccel()+getHeartRate();  
//    if(getFileSize(fileName) >= 100) {
//      fileName = nextFileName(dirName, filePrefix)+".TXT";
//    }
    writeLine(fileName, record);
//    Serial.println(record);
  }
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
<<<<<<< HEAD
\r\n数据每秒记录一次(无效的数据用'#'表示)，包含的内容有：\r\n\
=======
\r\n数据每秒记录一次，包含的内容有：\r\n\
>>>>>>> 69ef0a3923f1d29198c169d8d921f0cb7e12238b
\r\n当前时间 | 纬度(°) | 经度(°) | 海拔(m) | 速度(km/h) | 朝向(°) | 温度(℃) | 气压(Pa) | [x,y,z]加速度(m/s2) | [x,y,z]角速度(°/s) | 心率(times/min)";  
  if(!file_exist("README.TXT")) {
    Serial.println("Readme file does not exist, create it.");
    writeLine("README.TXT", msg);
  }
}
