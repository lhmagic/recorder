#define   wlSerial  Serial1

//void setup() {//  // put your setup code here, to run once:
//  Serial.begin(115200);
//}
//
//void loop() {
//  // put your main code here, to run repeatedly:
//  wlSend("Hello");
//  delay(1000);
//}

void wlInit(void) {
  wlSerial.begin(115200);
}

void wlSend(String msg) {
  wlSerial.println(msg);
}

