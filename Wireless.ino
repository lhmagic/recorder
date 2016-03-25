#define   wlSerial  Serial1

void wlInit(void) {
  wlSerial.begin(115200, SERIAL_8O1);
  wlSerial.setTimeout(10);
}

void rfHandle(String s) {
  if(wlSerial.available()) {
    String buf;
    buf = wlSerial.readStringUntil('\n');
    if(buf.equals("R\r")) {
       wlSerial.println(s); 
    }
  }
}

