#include <Wire.h>
#include <SPI.h>
#include <SD.h>

#define   WR_LED   13

Sd2Card card;
void(* resetFunc) (void) = 0;

void sdInit(void) {
  digitalWrite(WR_LED, HIGH);
  pinMode(WR_LED, OUTPUT);
  if(!SD.begin()) {
    Serial.println("SD init failed!");
    delay(10);
    resetFunc();
  }
  digitalWrite(WR_LED, LOW);
}

int8_t saveLog(String filename, String vlog) {
  digitalWrite(WR_LED, HIGH);
  sd_online();
  filename += ".txt";
  File f = SD.open(filename, FILE_WRITE);
  if(f) {
    f.println(vlog);
    f.close();
    digitalWrite(WR_LED, LOW);
//    Serial.println("file write success.");
    return 0;
  } else {
    return -1;
  }
}

void sd_online(void) {
  if(!card.init()) {
    Serial.println("SD doesn't insert, reset system to initialization file system.");
    delay(10); 
    resetFunc();     
  }
}

int8_t file_exist(String filename) {
char cname[12];  
  filename += ".txt";
  filename.toCharArray(cname, 12);    //There's some bug with function exists(String),
  if(SD.exists(cname))                //we change String to char array to avoid this bug.
    return 0;
  else 
    return -1;   
}

