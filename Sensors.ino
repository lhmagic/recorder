#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_ADXL345_U.h>
#include <Adafruit_HMC5883_U.h>
#include <Adafruit_L3GD20.h>

Adafruit_BMP085 bmp;
Adafruit_L3GD20 gyro;
Adafruit_ADXL345_Unified accel = Adafruit_ADXL345_Unified(345);
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(5883);

void sensorInit(void) {
  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP085 sensor, check wiring!");
  }
  if(!mag.begin()) {
    Serial.println("Ooops, no HMC5883 detected ... Check your wiring!");
  }  
  if (!gyro.begin(gyro.L3DS20_RANGE_250DPS)) {
    Serial.println("Oops ... unable to initialize the L3GD20. Check your wiring!");
  }
  if (!accel.begin()) {
    Serial.println("Ooops, no ADXL345 detected ... Check your wiring!");
  } else {
    accel.setRange(ADXL345_RANGE_16_G);
  }
}

String getTemperature(void) {
char str[12];
  dtostrf(bmp.readTemperature(), 2, 2, str);
  return (String)str+" ";
}

String getSealevelPressure(void) {
char str[12];
  sprintf(str, "%ld", bmp.readSealevelPressure());
  return (String)str+" ";
}

String getHeadingDegrees(void) {
char str[12];    
  sensors_event_t event; 
  mag.getEvent(&event);

  float heading = atan2(event.magnetic.y, event.magnetic.x);
  float declinationAngle = 0.22;
  heading += declinationAngle;
  if(heading < 0)
    heading += 2*PI;
  if(heading > 2*PI)
    heading -= 2*PI;
  dtostrf(heading * 180/M_PI, 3, 2, str);  
  return (String)str+" ";
}

String getAccel(void) {
String str;
char x[12], y[12], z[12];  

  sensors_event_t event; 
  accel.getEvent(&event);

  dtostrf(event.acceleration.x, 2, 2, x);
  dtostrf(event.acceleration.y, 2, 2, y);
  dtostrf(event.acceleration.z, 2, 2, z);

  str = x;
  str += " ";
  str += y;
  str += " ";
  str += z;
  str += " ";

  return str;
}

String getGyro(void) {
String str;
char x[12], y[12], z[12];  

  gyro.read();
  sprintf(x, "%d", (int)gyro.data.x);
  sprintf(y, "%d", (int)gyro.data.y);
  sprintf(z, "%d", (int)gyro.data.z);

  str = x;
  str += " ";
  str += y;
  str += " ";
  str += z;
  str += " ";

  return str;
}




