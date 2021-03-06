//connecting to nodeMCU
//MPU9250: vcc-5v, gnd - G, scl - D1, sda - D2, ad0 - 3v
//sd: vcc-5V, gnd-G, cs-D8, sck-D5, mosi-D7, miso-D6
//RTC: vcc-5v, gnd - G, scl - D1, sda - D2
 
#include "Wire.h"
// I2Cdev and MPU9250 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU9250.h"
// class default I2C address is 0x68
// specific I2C addresses may be passed as a parameter here
// AD0 low = 0x68 (default for InvenSense evaluation board)
// AD0 high = 0x69
 
#include <DS3231.h>
#include <SPI.h>
#include <SD.h>
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
 
 
MPU9250 accelgyro;
I2Cdev I2C_M;
 
uint8_t buffer_m[6];
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
 
float heading;
float tiltheading;
 
float Axyz[3];
float Gxyz[3];
float Mxyz[3];
 
#define sample_num_mdate 5000
volatile float mx_sample[3];
volatile float my_sample[3];
volatile float mz_sample[3];
 
static float mx_centre = 0;
static float my_centre = 0;
static float mz_centre = 0;
 
volatile int mx_max = 0;
volatile int my_max = 0;
volatile int mz_max = 0;
 
volatile int mx_min = 0;
volatile int my_min = 0;
volatile int mz_min = 0;
 
float temperature;
float pressure;
float atm;
float altitude;
 
/////////////////////////////
 
const char* ssid = "EWHA-IOT";
const char* password = "dscho007";
char server[]="192.168.0.8";
 
 
const int chipSelect = 15;
File myFile;
int check_fall_count = 0;
int check_fall_flag = 0;
int fall_down = 1;
int iii=0;
int ii=0;
String date = "f";
int final_fall_count = 0;
String fall_down_time = "";
DS3231 clock;
RTCDateTime dt;
/////////////////////////////
 
void setup()
{
  // join I2C bus (I2Cdev library doesn't do this automatically)
  Wire.begin();
  // initialize serial communication
  // (38400 chosen because it works as well at 8MHz as it does at 16MHz, but
  // it's really up to you depending on your project)
  
  Serial.begin(9600);
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.print("Connection : ");
  Serial.println(ssid);
  
  // initialize device
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
  // verify connection
  Serial.println("Testing device connections...");
  Serial.println(accelgyro.testConnection() ? "MPU9250 connection successful" : "MPU9250 connection failed");
  delay(1500);
  Serial.println(" ");
 
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);   //15번 - sd카드
  
  //SD initialize (setup)
  Serial.print("Initializing SD card...");
 
  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    return;
  }
  Serial.println("initialization done.");
  // open the file. note that only one file can be open at a time,
  // so you have to close this one before opening another.
  //SD initialization done (setup)
  
  clock.begin();
  clock.setDateTime(__DATE__, __TIME__);
  dt = clock.getDateTime();
  
  date += String(dt.year);
  date += String(dt.month); 
  date += String(dt.day);
  date += ".txt";
}
 
void loop()
{
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(server, httpPort)) {
    return;
  }
 
  
  dt = clock.getDateTime();
  
  getAccel_Data();
  getGyro_Data();
  getCompassDate_calibrated(); // compass data has been calibrated here
  getHeading(); //before we use this function we should run 'getCompassDate_calibrated()' frist, so that we can get calibrated data ,then we can get correct angle .
  getTiltHeading();
 
   if(check_fall_flag == 0){
    
    if((Gxyz[2]<= -70 || Gxyz[2]>= 70)){
      Serial.println("let's check if it's fell down");
      check_fall_flag = 1;
      delay(3000);
    }
    else{
      //Serial.print(iii++);
      //Serial.println("driving");
    }
  }
 
  if(check_fall_flag == 1){
    
    getGyro_Data();
    Serial.print("@@@@@@ ");
    Serial.print(Gxyz[0]);
    Serial.print(",");
    Serial.print(Gxyz[1]);
    Serial.print(",");
    Serial.println(Gxyz[2]); 
  
    if((Gxyz[0]<=4 && Gxyz[0]>= -4) && (Gxyz[1]<=4 && Gxyz[1]>= -4) && (Gxyz[2]<=4 && Gxyz[2]>= -4)){
      //Serial.println("check fall count ++");
      check_fall_count++;
    }
    
    else{
      check_fall_flag = 0; check_fall_count = 0;
    }
    
    if(check_fall_count == 3){
      
        Serial.print(dt.year);   
        Serial.print("-");
        Serial.print(dt.month);  
        Serial.print("-");
        Serial.print(dt.day);    
        Serial.print(" ");
        Serial.print(dt.hour);   
        Serial.print(":");
        Serial.print(dt.minute); 
        Serial.print(":");
        Serial.print(dt.second); 
        Serial.println("");
 
          //날짜, 시간을 sd card로 전송할 수 있어야 한다.
        //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ writing date & clock to file @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@
        myFile = SD.open(date, FILE_WRITE);
 
        if (myFile) {
 
          myFile.print(dt.hour);   
          myFile.print(":");
          myFile.print(dt.minute); 
          myFile.print(":");
          myFile.print(dt.second); 
          myFile.print(", ");
          myFile.print(fall_down); 
          myFile.println(",");
          myFile.close();
        } 
 
        else {
          // if the file didn't open, print an error:
          Serial.println("error opening text for writing");
        }
        
        //@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ done writing time & fall count to file @@@@@@@@@@@@@@@@@@@@@@@@@@
        Serial.println("bicycle has fell down :(");
        fall_down++;
    }
  }
   
  delay(500);
 
 
   if (client.connect(server, httpPort)) { // Http 요청 헤더
        //reading from sd card
          //@@@@@@@@@@@@@@@@@@reading from sd card@@@@@@@@@@@@@@@
      myFile = SD.open(date);
      
      if(myFile){
        
        while(myFile.available()){
          String line = myFile.readStringUntil(',');
          
          if(ii%2==1){
            fall_down_time = line;
          }
          else if(ii%2==0){
            final_fall_count = line.toInt();
          }
          ii++;
        }
        myFile.close();
      }
     client.print("GET /insert_falldown.php");//여기서
     client.print("fall_down_time=");
     client.print(fall_down_time );
     client.print("final_fall_count=");
     client.print(final_fall_count);//여기까지는 PHP에서 받기로한 데이터들이다
     client.println(" HTTP/1.1");
     client.println("Host: 192.168.0.8"); //요청을 보낼 서버의 주소
     client.println("Content-Type: application/x-www-form-urlencoded");
     client.println("Connection: close");
     client.println();
     client.println(); 
     client.stop();
     Serial.println("OK");
 }
  
}
 
void getHeading(void)
{
heading = 180 * atan2(Mxyz[1], Mxyz[0]) / PI;
if (heading < 0) heading += 360;
}
 
void getTiltHeading(void)
{
float pitch = asin(-Axyz[0]);
float roll = asin(Axyz[1] / cos(pitch));
 
float xh = Mxyz[0] * cos(pitch) + Mxyz[2] * sin(pitch);
float yh = Mxyz[0] * sin(roll) * sin(pitch) + Mxyz[1] * cos(roll) - Mxyz[2] * sin(roll) * cos(pitch);
float zh = -Mxyz[0] * cos(roll) * sin(pitch) + Mxyz[1] * sin(roll) + Mxyz[2] * cos(roll) * cos(pitch);
 
tiltheading = 180 * atan2(yh, xh) / PI;
if (yh < 0) tiltheading += 360;
 
}
 
void Mxyz_init_calibrated ()
{
Serial.println(F("Before using 9DOF,we need to calibrate the compass frist,It will takes about 2 minutes."));
Serial.print(" ");
 
Serial.println(F("During calibratting ,you should rotate and turn the 9DOF all the time within 2 minutes."));
Serial.print(" ");
 
Serial.println(F("If you are ready ,please sent a command data 'ready' to start sample and calibrate."));
 
while (!Serial.find("ready"));
 
Serial.println(" ");
Serial.println("ready");
Serial.println("Sample starting......");
Serial.println("waiting ......");
 
get_calibration_Data ();
 
Serial.println(" ");
Serial.println("compass calibration parameter ");
Serial.print(mx_centre);
Serial.print(" ");
Serial.print(my_centre);
Serial.print(" ");
Serial.println(mz_centre);
Serial.println(" ");
}
 
 
void get_calibration_Data ()
  {
  for (int i = 0; i < sample_num_mdate; i++)
  {
  get_one_sample_date_mxyz();
  
  /*
   Serial.print(mx_sample[2]);
   Serial.print(" ");
   Serial.print(my_sample[2]); //you can see the sample data here .
   Serial.print(" ");
   Serial.println(mz_sample[2]);
   */
  if (mx_sample[2] >= mx_sample[1])mx_sample[1] = mx_sample[2];
  if (my_sample[2] >= my_sample[1])my_sample[1] = my_sample[2]; //find max value
  if (mz_sample[2] >= mz_sample[1])mz_sample[1] = mz_sample[2];
  
  if (mx_sample[2] <= mx_sample[0])mx_sample[0] = mx_sample[2];
  if (my_sample[2] <= my_sample[0])my_sample[0] = my_sample[2]; //find min value
  if (mz_sample[2] <= mz_sample[0])mz_sample[0] = mz_sample[2];
  }
  
  mx_max = mx_sample[1];
  my_max = my_sample[1];
  mz_max = mz_sample[1];
  
  mx_min = mx_sample[0];
  my_min = my_sample[0];
  mz_min = mz_sample[0];
  
  mx_centre = (mx_max + mx_min) / 2;
  my_centre = (my_max + my_min) / 2;
  mz_centre = (mz_max + mz_min) / 2;
}
 
void get_one_sample_date_mxyz()
{
  getCompass_Data();
  
  mx_sample[2] = Mxyz[0];
  my_sample[2] = Mxyz[1];
  mz_sample[2] = Mxyz[2];
}
 
void getAccel_Data(void)
{
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  
  Axyz[0] = (double) ax / 16384;
  Axyz[1] = (double) ay / 16384;
  Axyz[2] = (double) az / 16384;
}
 
void getGyro_Data(void)
{
  accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  
  Gxyz[0] = (double) gx * 250 / 32768;
  Gxyz[1] = (double) gy * 250 / 32768;
  Gxyz[2] = (double) gz * 250 / 32768;
}
 
void getCompass_Data(void)
{
  I2C_M.writeByte(MPU9150_RA_MAG_ADDRESS, 0x0A, 0x01); //enable the magnetometer
  delay(10);
  
  I2C_M.readBytes(MPU9150_RA_MAG_ADDRESS, MPU9150_RA_MAG_XOUT_L, 6, buffer_m);
  mx = ((int16_t)(buffer_m[1]) << 8) | buffer_m[0] ;
  my = ((int16_t)(buffer_m[3]) << 8) | buffer_m[2] ;
  mz = ((int16_t)(buffer_m[5]) << 8) | buffer_m[4] ;
  
  Mxyz[0] = (double) mx * 1200 / 4096;
  Mxyz[1] = (double) my * 1200 / 4096;
  Mxyz[2] = (double) mz * 1200 / 4096;
}
 
void getCompassDate_calibrated ()
{
  getCompass_Data();
  Mxyz[0] = Mxyz[0] - mx_centre;
  Mxyz[1] = Mxyz[1] - my_centre;
  Mxyz[2] = Mxyz[2] - mz_centre;
}

