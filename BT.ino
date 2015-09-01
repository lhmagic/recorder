#define   bt_serial   Serial2
#define   bt_status   2

static String ID = "0x000000000000";

enum ebt_fsm{
  INIT,
  INQUERY,
  CONNECT,
  GET0
};

ebt_fsm bt_fsm;

void btInit(String bID) {
  ID = bID;
  pinMode(bt_status, INPUT);
  bt_serial.begin(115200);
  bt_serial.setTimeout(10);
  if(digitalRead(bt_status) == HIGH) {
    bt_fsm = GET0;
  } else {
    bt_fsm = INIT;
  }
}

String getHeartRate(void) {
static int8_t index, offset, i, timeout;
static String id[8];
static String buf ;

  if(digitalRead(bt_status) == HIGH) {
    bt_fsm = GET0;
  } 

  switch(bt_fsm) {
    case INIT:
      if(digitalRead(bt_status) == HIGH) {
        bt_fsm = GET0;
        break;
      }
      bt_serial.println("AT+INQ");
      index=0; offset=0; i=0; timeout=0; buf = "";
      bt_fsm = INQUERY;
      break;
    case INQUERY:
      if(digitalRead(bt_status) == HIGH) {
        bt_fsm = GET0;
        break;
      }
      if(bt_serial.available()) {
        timeout = 0;
        buf += bt_serial.readString();
        if(buf.indexOf("+INQE") != -1) {
          while((index=buf.indexOf("+INQ:", offset)) != -1) {
            id[i%8] = buf.substring(index+7, index+21);
            if(id[i].equals(ID)) {
              bt_serial.print("AT+CONN");bt_serial.println(i+1);
              bt_fsm = CONNECT;
              timeout = 0;
              break;
            }
            index++; i++; offset+=21;
          }
        }
      } else {
         if(++timeout >= 10) {
          bt_fsm = INIT;
         }
      }
      break;
    case CONNECT:
      if(digitalRead(bt_status) == HIGH) {
        bt_fsm = GET0;
        break;
      }
      if(digitalRead(bt_status) == HIGH) {
        bt_fsm = GET0;
      } else {
        bt_serial.print("AT+CONN");bt_serial.println(i+1);
      }
      if(++timeout >= 10){
        bt_fsm = INIT;
      } 
      break;
    case GET0:
      bt_serial.println("$GET0");
      
      buf = bt_serial.readStringUntil('\n');
      if(buf.startsWith("$")) {
        return buf.substring(1,4);;
      } else {
        bt_fsm = INIT;
      }
      break;
  }

  return "###";
}



