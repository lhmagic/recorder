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

int8_t writeLine(String fileName, String data) {
  digitalWrite(WR_LED, HIGH);
  sd_online();
//  Serial.println(fileName);
  File f = SD.open(fileName, FILE_WRITE);
  if(f) {
    f.println(data);
    f.close();
    digitalWrite(WR_LED, LOW);
    return 0;
  } else {
//    Serial.println("can not open it.");
    return -1;
  }
}

String readLine(String fileName) {
static uint32_t pos = 0;
String str = "";
char ch;

  File f = SD.open(fileName, FILE_READ);
  if(f) {
    if(f.seek(pos)) {
      while((ch=f.read()) != -1) {
        if(ch=='\n')break;
        str += ch;
      }
      pos = f.position();
      return str;
    } 
    f.close();   
  }
  return "";
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
  filename.toCharArray(cname, 12);    //There's some bug with function exists(String),
  if(SD.exists(cname))                //we change String to char array to avoid this bug.
    return true;
  else 
    return false;   
}

String nextFileName(String dir, String fileNamePrefix) {
String fileName = "";
String fileNameAppend = "";
uint8_t append = 0;

  File Dir = SD.open(dir);
  if(Dir) {
    while(true) {
      File f = Dir.openNextFile();
      if(!f) {
        break;
      } else {
        fileName = f.name();
        Serial.println(fileName);
        if(fileName.startsWith(fileNamePrefix)) {
          uint8_t len = fileNamePrefix.length();
          append = fileName.substring(len,len+2).toInt();
        }
      }
      f.close();
    }
    Dir.close();   
  }

  append++;
  fileNameAppend = (append<10)? ((String)0+(String)append) : (String)append;

  return dir+"/"+fileNamePrefix+fileNameAppend;
}

long getFileSize(String fileName) {
long fileSize;  
  File f = SD.open(fileName, FILE_READ);
  if(f) {
    fileSize = f.size();
    f.close();
  } else {
    fileSize -1;
  }
  return fileSize;
}

void touchDir(String dirName) {
char cname[12];
  dirName.toCharArray(cname, 12);   
  if(!file_exist(cname)) {
    Serial.print(cname);Serial.println(" does not exist, create it.");
    SD.mkdir(cname);
  }
}

