
#define ONE_WIRE_BUS D5  // DS18B20 Sensors 
#define TEMPERATURE_PRECISION 10  //ตั้งความละเอียดการอ่นอุณหภูมิ
#define DEVICE_DISCONNECTED -127

#define button_Pin  D7 //for Switch select mode 
#define button_test  D6 //for Switch test message line notify

static float tempOneC = 0;
static float tempOneF = 0;
static float tempTwoC = 0;
static float tempTwoF = 0;
static int lowAlarmOne = 0;
static int highAlarmOne = 0;
static int lowAlarmTwo = 0;
static int highAlarmTwo = 0;

String TOKEN = "TftxtM2bfiZkqZjEpIbEURIT3gIDtNLGcwdm82QzwX0";
String location = "";
unsigned long previousMillis = 0; //กำหนดตัวแปรเก็บค่า เวลาสุดท้ายที่ทำงาน
const long interval = 1000 * 60 * 10; //กำหนดค่าตัวแปร ให้ทำงานทุกๆ 1 นาที
int day_time = 60; // 60 วัน
const long interval_restart = 1000 * 60 * 60 * (24 * day_time); //รีสตาร์ททุก 2 เดือน ขึ้นกับตัวแปร day_time
