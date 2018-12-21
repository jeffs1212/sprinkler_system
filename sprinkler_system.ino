/*
Sprinkler control for a 6 valve system
merged 12/20/18
*/
//
#include <RTClib.h>   // RTClib - Version: Latest
#include <Wire.h>    // LCD communication
#include <SoftwareSerial.h>
//
SoftwareSerial lcd(11, 10);  // This is required, to start an instance of an LCD.  pin 10 = TX, pin 11 = RX (unused)
//
RTC_PCF8523 rtc;
//
char timeBuf[16];  // buffer for printing time formatted
char daysOfTheWeek[7][10] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
char valveSelected[8][6] = {"ZONE1", "ZONE2", "ZONE3", "ZONE4", "ZONE5", "ZONE6", "ALL", "NONE"};
char displayZone1[2][2] = {"-", "1"};  //display on the LCD "-" if the zone is OFF, or the zone number if the zone is ON
char displayZone2[2][2] = {"-", "2"};
char displayZone3[2][2] = {"-", "3"};
char displayZone4[2][2] = {"-", "4"};
char displayZone5[2][2] = {"-", "5"};
char displayZone6[2][2] = {"-", "6"};
int zoneArray[8] = {1,1,1,1,1,1,1,0};  // default values for zone select enables, 8-way switch = zones 1-6,ALL,nothing
int valveArray[6] = {0,0,0,0,0,0}; // default valve relay for each zone is off
//
// run time durations
int v1duration = 700;  // check these values - ideally want this to represent "minutes"
int v2duration = 500;
int v3duration = 500;
int v4duration = 500;
int v5duration = 500;
int v6duration = 1200;
int testDuration = 200;
uint16_t runTime = 0;  // initialize a variable for the run time count
int i;
//
/*
  STILL NEED TO DEFINE A VARIABLE FOR THE DAYS TO WATER
  STILL NEED TO DEFINE A VIARIABLE FOR THE TIME OF DAY TO START
*/
//
// hardware assignments
// switch assignments
//momentary push buttons assigned to pins 2 and 3 as Arduino interrupt pins
const byte enablebutton = 2;//the "enable" button toggles "on/off" the zone selected by the rotary switch
const byte testbutton = 3;//the "test" button runs the zone selected by the rotary switch.  It does not run when in the "ALL" position
int adc_select_pin = A0; // zone select 8-way rotary switch analog input pin for the voltage divider input
//initialize the zone select values
int zoneSelect = 0;
int adc_select = 0;
boolean enable=false,test=false;
byte SelectSwitch;
// valve relay switch assignments
const byte valve1relay = 4;  //digital output 4 controls relay 1
const byte valve2relay = 5;  //digital output 5 controls relay 2
const byte valve3relay = 6;  //digital output 6 controls relay 3
const byte valve4relay = 7;  //digital output 7 controls relay 4
const byte valve5relay = 8;  //digital output 8 controls relay 5
const byte valve6relay = 9;  //digital output 9 controls relay 6
//
#define ZONE1    0
#define ZONE2    1
#define ZONE3    2
#define ZONE4    3
#define ZONE5    4
#define ZONE6    5
#define ZONE_ALL 6
#define ALL_OFF  7
//
// this is the routine to read the zone select rotary switch voltage divider
int read_select_switch()
 {
  adc_select = analogRead(adc_select_pin);      // read the value from the select switch
  if (adc_select > 1000) return ALL_OFF;
  if (adc_select < 90)   return ZONE1; 
  if (adc_select < 150)  return ZONE2;  
  if (adc_select < 300)  return ZONE3; 
  if (adc_select < 450)  return ZONE4; 
  if (adc_select < 600)  return ZONE5; 
  if (adc_select < 750)  return ZONE6;
  if (adc_select < 850)  return ZONE_ALL;
  // 
  return ALL_OFF;   // if all select values fail, return this...
 }
//
//
void setup() {
  // initialize the RTC and serial monitor
  Serial.begin(9600);//Prepare serial port for use
  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1); 
  }  
  // initialize the LCD
  lcd.begin(9600);  // Start the LCD at 9600 baud
  clearDisplay();  // Clear the display
  setLCDCursor(2);  // Set cursor to the 3rd spot, 1st line
  lcd.print("Hello, world");
  Serial.println("hello world");
  setLCDCursor(16);  // Set the cursor to the beginning of the 2nd line
  lcd.print("Running clock...");
  delay(2000);
  clearDisplay();  // Clear the display
  //  
  // initialize the Ardiono pin modes
  pinMode(enablebutton,INPUT);
  pinMode(testbutton,INPUT);
  pinMode(adc_select_pin, INPUT);
  pinMode(valve1relay, OUTPUT);
  pinMode(valve2relay, OUTPUT);
  pinMode(valve3relay, OUTPUT);
  pinMode(valve4relay, OUTPUT);
  pinMode(valve5relay, OUTPUT);
  pinMode(valve6relay, OUTPUT);
}
//
void loop() {
//  
  zoneSelect = read_select_switch();  // read the select switch and return the name of the "zone"
  Serial.print(adc_select);   // display the voltage in the serial monitor for tuning the button voltage divider values
  Serial.println("mv");       // the voltage values are in millivolts
  //
  DateTime now = rtc.now();  // get the current date form the RTC
  //  
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
  //  
    delay(3);  
 //
 /*
 Tracks presses of enable and test buttons,
 */
 //
 attachInterrupt(digitalPinToInterrupt(enablebutton), EnablePushButton, FALLING);  //check in enable button has been pushed
 attachInterrupt(digitalPinToInterrupt(testbutton), TestPushButton, FALLING);  //check in test button has been pushed
 //  
 if (enable) 
    {
     Serial.print(valveSelected[zoneSelect]);  //print from the array of "valveSelected" names the zone selected when the enable button was pressed 
     Serial.println(" selected");
     clearDisplay();  // Clear the display
     setLCDCursor(0);  // Set cursor to the 1st spot, 1st line 
     lcd.print(valveSelected[zoneSelect]);  // display the zone selected by the enable button
     lcd.print(" selected");
     delay(1000);     //  delay to have a chance to read the zone enabled or disabled
     clearDisplay();  // Clear the display
     if (zoneSelect == 6) {   //  if select = ALL turn all the "zone enables" ON 
       for (i=0; i < 6; i++) {
         zoneArray[i] = 1;   };
     } 
     if (zoneSelect == 7) {   //  if select = ALL OFF disable all zones and turn off all valves
       for (i=0; i < 6; i++) {
         zoneArray[i] = 0;   };
       for (i=0; i < 6; i++) {
         valveArray[i] = 0;  };
     }
     zoneArray[zoneSelect] = !zoneArray[zoneSelect];  // if select equals an individual zone, (0-5), toggle the selected zone enable
     enable = false;
    }
 //
 if (test) 
    {
     if (runTime == 0) {
       Serial.print("Test:");   // print the zone to start testing
       Serial.print(valveSelected[zoneSelect]);  //print from the array of "valveSelected" names the zone selected when the test button was pressed 
       Serial.println(" on");
       clearDisplay();  // Clear the display
       setLCDCursor(0);  // Set cursor to the 1st spot, 1st line 
       lcd.print("Test:");     // display the zone to start testing
       lcd.print(valveSelected[zoneSelect]);  // display the zone selected by the test button
       lcd.print(" on"); 
       valveArray[zoneSelect] = 1;  // turn on the selected valve
       runTime = testDuration;     // set the run time to the test duration
     }   
     else {
       Serial.println("Test done");  // if Test is pressed again to stop the test
       clearDisplay();  // Clear the display
       setLCDCursor(3);  // Set cursor to the 1st spot, 1st line 
       lcd.println("Done");
       valveArray[zoneSelect] = 0;  // turn off the selected valve
       runTime = 0; // reset runtime to 0
       delay(5);     //  might needto make this longer
     }
     test = false;
    }
 // 
 delay(200);//So serial port isn't overwhelmed by too-rapidly
             //repeated instances of sending the answer again and again
 //
 // if no button presses this is the default display output and turn all the valves off
 if (runTime == 0) {
    setLCDCursor(3);  // Set cursor to the 4th spot, 1st line
    if (now.hour()<=9)
     {lcd.print("0");}
    lcd.print(now.hour(), DEC);
    lcd.print(':');
    if (now.minute()<=9)
     {lcd.print("0");}    
    lcd.print(now.minute(), DEC);
    lcd.print(':');
    if (now.second()<=9)
     {lcd.print("0");}
    lcd.print(now.second(), DEC);
    lcd.println();
    setLCDCursor(16);  // Set cursor to the 1st spot, 2nd line
    lcd.print("Zones: ");
    lcd.print(displayZone1[zoneArray[0]]);
    lcd.print(displayZone2[zoneArray[1]]);
    lcd.print(displayZone3[zoneArray[2]]);
    lcd.print(displayZone4[zoneArray[3]]);
    lcd.print(displayZone5[zoneArray[4]]);
    lcd.print(displayZone6[zoneArray[5]]);
    lcd.println();
    for (i=0; i<6; i++) {
      valveArray[i] = 0;};   // if runtime = 0, turn off the valves
 }
 //
 /*
   STILL NEED TO WRITE THE CODE TO CHECK IF THE RTC EQUALS THE START TIME ON A WATER DAY
 */
 //
 // valve control
  digitalWrite(valve1relay, valveArray[0]);
  digitalWrite(valve2relay, valveArray[1]);
  digitalWrite(valve3relay, valveArray[2]);
  digitalWrite(valve4relay, valveArray[3]);
  digitalWrite(valve5relay, valveArray[4]);
  digitalWrite(valve6relay, valveArray[5]);
  //
  // for sprinkler zone running
  if (runTime > 0) {  
    runTime = runTime-1;
    setLCDCursor(16);  // Set cursor to the 1st spot, 2nd line
    sprintf (timeBuf, "Run Time = %4u", runTime);  // format run time for LCD
    lcd.print(timeBuf);
    Serial.println(runTime);
    if (runTime == 0) clearDisplay();  // Clear the display at the end of th run time 
  } 
  //
}
//
// functions that need to be defined
//
void EnablePushButton()  // interrupt service routine for enable push button debounce
{
  static unsigned long enable_last_interrupt_time = 0;  //static only initializes the 1st time the function is called
  unsigned long enable_interrupt_time = millis();
  if (enable_interrupt_time - enable_last_interrupt_time > 200)
  {
    enable = true;
  }
  enable_last_interrupt_time = enable_interrupt_time;
}
//
void TestPushButton()  // interrupt service routine for test push button debounce
{
  static unsigned long test_last_interrupt_time = 0;  //static only initializes the 1st time the function is called
  unsigned long test_interrupt_time = millis();
  if (test_interrupt_time - test_last_interrupt_time > 200)
  {
    test = true;
  }
  test_last_interrupt_time = test_interrupt_time;
}
//
void clearDisplay()
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x01);  // send the clear screen command
}
//
void setLCDCursor(byte cursor_position)
{
  lcd.write(0xFE);  // send the special command
  lcd.write(0x80);  // send the set cursor command
  lcd.write(cursor_position);  // send the cursor position
}
//
