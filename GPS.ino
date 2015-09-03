#include  <Time.h>
#include  <time.h>

#define   gps_serial    Serial3

#define   KNOT2KMPH     1.852

String GPS_DATA[32];

String RFLatitude;
String RFLongitude;
String RFSpeed;

void gpsInit(void) {
  //  gps_serial.begin(4800);
  //  gps_serial.println("$PMTK314,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0*28"); //RMC GGA
  //  gps_serial.println("$PMTK251,115200*1F");
  //  gps_serial.end();
  gps_serial.begin(115200);
  gps_serial.setTimeout(10);
  gps_serial.flush();
}

String getGPS(void) {
  String  NMEA;
  String curTime, curLatitude, curLongitude, curAltitude, curSpeed;

  if (gps_serial.available()) {
    gps_serial.readStringUntil('$');
    NMEA = gps_serial.readStringUntil('\n');
//    Serial.println(NMEA);
    if(gpsParse(NMEA) == 2) {
      curLatitude = "####.###### "; curLongitude = "#####.###### ";
      curAltitude = "####.# "; curSpeed = "#.## "; curTime = "############## ";
          
      if (GPS_DATA[9].length() != 0) {  //date
        curTime = GPS_DATA[1].substring(0, 6)+GPS_DATA[9];
        curTime = localeTime(curTime)+" ";
      } 
//      else if(GPS_DATA[1].length() != 0) {   //time
//        curTime = GPS_DATA[1].substring(0, 6)+" ";
//        curTime = "########"+curTime;
//      }
      
      if (GPS_DATA[2] == "A") {
        curLatitude = GPS_DATA[3] + GPS_DATA[4] + " ";               //latitude
        curLongitude = GPS_DATA[5] + GPS_DATA[6] + " ";              //longitude
        curAltitude = GPS_DATA[22] + " ";                            //altitude
        char v[12]; dtostrf(GPS_DATA[7].toFloat()*KNOT2KMPH, 2, 2, v);
        curSpeed = v; curSpeed += " ";
      }

      RFLatitude = curLatitude;
      RFLongitude = curLongitude;
      RFSpeed = curSpeed;
      
      return (curTime+curLatitude+curLongitude+curAltitude+curSpeed);
    }
  }
  return "";
}

int8_t gpsParse(String nmea) {
  int start=0, i=0, j=0, ret=0;
  
  if (nmea.startsWith("GPRMC")) {
    j = 0;
    while (nmea[i] != '*') {
      if (nmea[i] == ',') {
        GPS_DATA[j++] = nmea.substring(start, i);
        start = i + 1;
      }
      if (i >= 120) {
        ret = -1;
        break;
      }
      i++;
    }
    ret = 1;
  } else if (nmea.startsWith("GPGGA")) {
    j = 13;
    while (nmea[i] != '*') {
      if (nmea[i] == ',') {
        GPS_DATA[j++] = nmea.substring(start, i);
        start = i + 1;
      }
      if (i >= 120) {
        ret = -1;
        break;
      }
      i++;
    }
    ret = 2;
  } else {
    for(i=0; i<32; i++) {
      GPS_DATA[i] = "";
    }
  }
  return ret;
}

String getRFData(void) {
  return RFLatitude+" "+RFLongitude+" "+RFSpeed+" ";
}

String localeTime(String curTime)  {   //hour, minite, second, day, month, year
String curYear, curMonth,curDay, curHour,curMinute, curSecond;
  
  setTime(curTime.substring(0,2).toInt(),curTime.substring(2,4).toInt(),curTime.substring(4,6).toInt(),
          curTime.substring(6,8).toInt(),curTime.substring(8,10).toInt(),curTime.substring(10,12).toInt());
  adjustTime(8*3600);
  curYear = String(year());
  if(month() < 10) curMonth = "0";
  curMonth += String(month());
  if(day() < 10) curDay = "0";
  curDay += String(day());
  if(hour() < 10) curHour = "0";
  curHour += String(hour());
  if(minute() < 10) curMinute = "0";
  curMinute += String(minute());  
  if(second() < 10) curSecond = "0";
  curSecond += String(second()); 
    
  return curYear+curMonth+curDay+curHour+curMinute+curSecond;
}
