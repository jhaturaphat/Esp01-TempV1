//TEST to git new

// Variables to save date and time
char daysOfTheWeek[7][12] = {"SUN", "MON", "TUE", "WED", "THU", "FRI", "SAT"};
/* Config for LCD 16x2 I2C */
#define WIDTH    16
#define HIGHT    2


#define SCL         D1 //for LCD 16x2 
#define SDA         D2 //for LCD 16x2 

LiquidCrystal_I2C lcd(0x27, WIDTH, HIGHT); 

byte mac[6];
byte degree_symbol[8] =
{
  0b00111,
  0b00101,
  0b00111,
  0b00000,
  0b00000,
  0b00000,
  0b00000,
  0b00000
};

byte termometru[8] = //icon for termometer
{
    B00100,
    B01010,
    B01010,
    B01110,
    B01110,
    B11111,
    B11111,
    B01110
};