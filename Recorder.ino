
void setup() {
  Serial.begin(115200);
  sensorInit();
  btInit();
  gpsInit();
  sdInit();
  gen_readme();
}

void loop() {
  String record = getGPS();
  if (record.length() != 0) {
    record += getTemperature()+getSealevelPressure()+\
              getHeadingDegrees()+getGyro()+getAccel()+getHeartRate();  
    saveLog("record", record);
    Serial.println(record);
  }
}

void gen_readme(void) {
String msg = "\
\r\n数据每秒记录一次，包含的内容有：\r\n\
\r\n当前时间 | 纬度(°) | 经度(°) | 海拔(m) | 速度(km/h) | 朝向(°) | 温度(℃) | 气压(Pa) | [x,y,z]加速度(m/s2) | [x,y,z]角速度(°/s) | 心率(times/min)";  
  if(file_exist("readme")) {
    Serial.println("Readme file does not exist, create it.");
    saveLog("readme", msg);
  }
}
