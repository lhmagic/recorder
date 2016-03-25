#define   bt_serial   Serial2
#define   bt_status   2

static String ID = "0xFFFFFFFFFFFF";

enum ebt_fsm{
  BT_IDLE,
  BT_INQ,
  BT_LIST,
  BT_CONN,
  BT_AUTH,
  BT_POLL,
  BT_READ,
};

ebt_fsm bt_fsm;

void btInit(String bID) {
  ID = bID;
  pinMode(bt_status, INPUT);
  bt_serial.begin(115200);
  bt_serial.setTimeout(10);
}

String getHeartRate(void) {
static String buf;
static int8_t i,j, timeout, count;

  switch(bt_fsm) {
    case BT_IDLE:
      bt_serial.print("Q");
      timeout=0; buf = "";
      bt_fsm = BT_INQ;
      break;
    case BT_INQ:
      if(bt_serial.available()) {
        timeout = 0;
        buf += bt_serial.readString();
        
        if(buf.indexOf("Connected") != -1) {
          bt_fsm = BT_AUTH;
          break;  
        }
        
        i = buf.indexOf("Devices Found");
        if(i != -1) {
          i += sizeof("Devices Found");
          j = buf.indexOf("\r\n");
          buf = buf.substring(i,j);
          count = buf.toInt();
          if(count > 0) {
            bt_serial.print("L");
            i=0; j=0; timeout=0; buf = "";
            bt_fsm = BT_LIST;
            break;
          } else {
             bt_fsm = BT_IDLE;  
          }
        }
      } else {
         if(++timeout >= 10) {
          bt_fsm = BT_IDLE;
         }
      }
      break;
    case BT_LIST:
      if(bt_serial.available()) {
        timeout = 0;
        buf += bt_serial.readString();
        i = buf.indexOf("0x");
        if(i != -1) {
          if((buf=buf.substring(i, i+14)) == ID) {
            bt_serial.print("C");
            i=0; j=0; timeout=0; buf = "";
            bt_fsm = BT_CONN;
            break;             
          } else if(j<count) {
            bt_serial.print("L");
            j++;  
          } else {
             bt_fsm = BT_IDLE;
          }
        }
      } else {
         if(++timeout >= 10) {
          bt_fsm = BT_IDLE;
         }
      }        
    case BT_CONN:
      if(bt_serial.available()) {
        timeout = 0;
        buf += bt_serial.readString();
        i = buf.indexOf("Connected");
        if(i != -1) {
          bt_fsm = BT_AUTH;
        }
      } else {
         if(++timeout >= 10) {
          bt_fsm = BT_IDLE;
         }
      } 
      break;
    case BT_AUTH:
          bt_serial.print("A"); 
          bt_fsm = BT_POLL;    
          break;
    case BT_POLL:
         bt_serial.print("M");
         bt_fsm = BT_READ;
         break;
    case BT_READ:
      static int8_t no_rcv_cnt;
      buf = bt_serial.readStringUntil('\n');
      if(buf.startsWith(">")) {
        uint8_t st = buf[2]-0x30;
        no_rcv_cnt = 0;
        if(st == 0){
          return buf.substring(4,buf.indexOf("\r\n"));
        }
      } else {
        if(no_rcv_cnt++ >= 2)
          bt_fsm = BT_POLL;
      }

      if(no_rcv_cnt >= 5) {
        bt_fsm = BT_IDLE;
      }
      break;
  }
  return "###";
}



