#include <SoftwareSerial.h>
#include <DFPlayer_Mini_Mp3.h>
 
//#include <MsTimer2.h>
#include <DS3231.h>
#include <SPI.h>
#include <SD.h>
 #include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#define PI 3.1415926535897932384626433832795
#define CYCLE_RADIUS 0.2 //meter
 
int sensor=4; //d2
int state = 1; //감지x 
int preState = 1; //이전상태
int count=0;
double distance;
 
//---------
File myFile;
 
DS3231 clock;
RTCDateTime dt;
RTCDateTime dt2;
RTCDateTime dt3;
 
const int chipSelect = 15;
 
String date = "";
String date_send = "";
String howManySpeedOver = "";
double d1 = 0.0;
double d2 = 0.0;
 
double instant_velocity;
double max_velocity;
int ii = 0;
 
int time_flag = 1;
int accum_time_hour = 0;
int accum_time_minute = 0;
int accum_time_sec = 0;
 
int start_hour = 0;
int start_minute = 0;
int start_second = 0;
 
int end_hour = 0;
int end_minute = 0;
int end_second = 0;
 
 
int start_hour2 = 0;
int start_minute2 = 0;
int start_second2 = 0;
 
int end_hour2 = 0;
int end_minute2 = 0;
int end_second2 = 0;
 
int accum_time_hour2 = 0;
int accum_time_minute2 = 0;
int accum_time_second2 = 0;
int sec_to_find_distance = 0;
 
 
const char* ssid = "EWHA-IOT";
const char* password = "dscho007";
char server[]="192.168.0.8";
String accum_time_string = "";
 
int over_speed = 0;
//--------------------------------------------------------------
const byte interruptPin1 = 5; //d1
const byte interruptPin2 = 14; //d5
const byte interruptPin3 = 12; //d6
const byte interruptPin4 = 13; //d7
 
volatile int saddleState = HIGH;
volatile int leftState = HIGH;
volatile int rightState = HIGH;
volatile int pedalState = HIGH;
 
int wheelCount=0;
long debounceDelay = 50;
volatile int timenow=0;
 
int pretime = 0;
int Wtimenow=0;
int Wpretime = 0;
 
int previousTime = 0;
int previousTime2 = 0;
int previousTime3 = 0;
int previousTime4 = 0;
 
int interval = 50;
int ride = 5000;
int rotation = 2000;
 
int i=0;
 
void find_velocity(){ //find max velocity and instant velocity (every 1sec)
    d2 = distance;
  instant_velocity = (d2 - d1);
  
  if(max_velocity < instant_velocity){
    max_velocity = instant_velocity;
  }
  
  d1 = d2;
}
 
void setup() {
  
  Serial.begin(9600);
  clock.begin(); 
  clock.setDateTime(__DATE__, __TIME__);
  WiFi.begin(ssid, password);  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connection : ");
  Serial.println(ssid);
/*
  MsTimer2::set(1000,find_velocity);
  MsTimer2::start();
  */
  
  dt = clock.getDateTime();
  date += String(dt.year);
  date += String(dt.month); 
  date += String(dt.day);
  date += ".txt";
 
  date_send += String(dt.year);
  date_send += String(dt.month); 
  date_send += String(dt.day);
  
  howManySpeedOver += String(dt.year);
  howManySpeedOver += String(dt.month); 
  howManySpeedOver += String(dt.day);
  howManySpeedOver += "SpeedOverCount.txt";
  
  Serial.println(date);
 
   //SD initialize (setup)
   Serial.print("Initializing SD card...");
   
  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    return;
  }
  
  Serial.println("initialization done.");
  
  pinMode(sensor, INPUT);
  pinMode(interruptPin1, INPUT_PULLUP);
  pinMode(interruptPin2, INPUT_PULLUP);
  pinMode(interruptPin3, INPUT_PULLUP);
  pinMode(interruptPin4, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(interruptPin1), saddleInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin2), leftInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin3), rightInterrupt, CHANGE);
  attachInterrupt(digitalPinToInterrupt(interruptPin4), pedalInterrupt, CHANGE);
 
//MP3 Player 모듈을 사용하기 위한 setup 
  mp3_set_serial (Serial);      // DFPlayer-mini mp3 module 시리얼 세팅
  delay(1);                     // 볼륨값 적용을 위한 delay
  mp3_set_volume (30);          // 볼륨조절 값 0~30
 
}
 
void loop() {
 
  
  //안장에 앉은 경우==========================================================================
  if(saddleState == LOW) {
    
    //-----------------
    if(time_flag==1){ //앉자마자 시간을 체크한다!!! (앉기 시작한 시간)
      dt = clock.getDateTime();
      
      start_hour = dt.hour;
      start_minute = dt.minute;
      start_second = dt.second;
      time_flag = 0;
    }
    //Serial.println("saddle");
    
    //---------------------------------------------------------
    state = digitalRead(sensor);  //읽음  
    
    if(state != preState){
      if(state == 0) {
        count++;  //white에서 black으로 넘어가는 경우에만 count를 1씩 더해준다
        
        //--------------------------------------------------------------
        Wtimenow = millis();
        
        //바퀴가 회전중일 때(자전거를 타는중) 손잡이를 잡지 않는 경우. 안내음성
        if(Wtimenow - Wpretime < 2000 && (leftState == HIGH || rightState == HIGH)) {
          //Serial.println("Hold the handle!!");
          i=1;
          if(i==1){
            mp3_play (2); //0002손잡이.mp3 파일 플레이
            i=0;
          }
          
          //2초가 지나면 멈춘것으로 판단
          if(millis() - previousTime4 > rotation){
            previousTime4 = millis();
            Wtimenow = 20000;
            Wpretime = 0;
          }
        }
      } 
      else if(state == 1){
        Wpretime = Wtimenow;
        previousTime4 = millis();
      }
      
      //-----------------------------------------------------------
      preState = state; //prestate를 state로 변경
    }
    
     //Serial.println(count);
    //count가 3 증가하면 한 바퀴를 주행한 것으로 간주
    if(count%3==0){
      
      //회전수 출력------------------------------------------------------
      wheelCount=count/3;
      if(millis() - previousTime > interval) { 
        previousTime = millis();
        Serial.print("wheelCount = ");
        Serial.println(wheelCount);
      }
      
      //거리구하기----------------------------------------------
      distance = 2*CYCLE_RADIUS *PI * (count/3);
    }
    else if(count%3==1) distance = 2*CYCLE_RADIUS *PI * (count/3 + (float)1/3);
    else if(count%3==2) distance = 2*CYCLE_RADIUS *PI * (count/3 + (float)2/3);
  } 
  
  else if(saddleState == HIGH) {
    //우선은 일어서자마자 시간을 체크해서 더한다******* (앉아있던 시간을 더하는 개념)
    
    if(time_flag==0){
      
      dt = clock.getDateTime();
      end_hour = dt.hour;
      end_minute = dt.minute;
      end_second = dt.second; 
 
      if(accum_time_sec + end_second - start_second > 59){
        accum_time_minute += 1;
        accum_time_sec = accum_time_sec + end_second - start_second - 60;
      }
      
      if(accum_time_minute + end_minute - start_minute > 59){
        accum_time_hour += 1;
        accum_time_minute = accum_time_minute + end_minute - start_minute - 60;
      }
 
      accum_time_hour += end_hour - start_hour;
      time_flag = 1;
    }
    
   //손잡이 양쪽과 페달을 밟은 경우->페달회전측정으로 변경(마그네틱센서 이용)--------------
   if(leftState == LOW && rightState == LOW) {
    //magnetic센서가 0->1일 때 시간을 측정하고 다음 시간측정까지의 간격이 5초보다 짧으면 자전거를 타고 있는 것으로 인식. 회전 수와 거리를 측정한다.
    
    if(pedalState == HIGH) {
      if(timenow - pretime < 5000) {
        //그 다음에 새로 시각을 받아서 서서주행하는 시간의 시작시간으로 설정
        if(time_flag==1){
          dt = clock.getDateTime();
          start_hour = dt.hour;
          start_minute = dt.minute;
          start_second = dt.second; 
          
          time_flag = 0;
        }
        
        //----------------------------------------------------------------
        //Serial.println("pedal");
        //일어서서 자전거를 타는 경우 안내음성
        //Serial.println("Sit down please.");
        
        i=1;
        if(i==1){
            mp3_play (3); //0003일어서서.mp3 파일 플레이
            i=0;
        }
          
        //------------------------------------------------------
        state = digitalRead(sensor);  //읽음  
        
        if(state != preState){
          if(state == 0)  count++;  //white에서 black으로 넘어가는 경우에만 count를 1씩 더해준다
            preState = state; //prestate를 state로 변경
        }
 
        //count가 3 증가하면 한 바퀴를 주행한 것으로 간주
        if(count%3==0){
          
          //회전수 출력-----------------------------------------------------
          wheelCount=count/3;
          
          if(millis() - previousTime2 > interval) { 
            previousTime2 = millis();
            Serial.print("wheelCount = ");
            Serial.println(wheelCount);
          } 
          
          //거리구하기-----------------------------------------------
          distance = 2*CYCLE_RADIUS *PI * (count/3);
        }
        else if(count%3==1) distance = 2*CYCLE_RADIUS *PI * (count/3 + (float)1/3);
        else if(count%3==2) distance = 2*CYCLE_RADIUS *PI * (count/3 + (float)2/3);
        
        // HIGH인 상태로 5초가 지나면 멈춘것으로 판단----------------------------
        if(millis() - previousTime3 > ride) { 
          previousTime3 = millis();  
          timenow = 50000;
          pretime = 0;
          Serial.println("stop---------------------");
        }
      }
    }
    else if(pedalState == LOW) {
      pretime = timenow;
      previousTime2 = millis();
    }
   }
  }//saddlestate
 
  //@@@@@@@@@@@@@@@@@@@@@writing distance to sd card@@@@@@@@@@@@@@@@@@@@@@@
  myFile = SD.open(date, FILE_WRITE);
  
  if(myFile){
    //총 주행 시간, 최대속력, 거리,
 
    myFile.print(accum_time_hour);
    myFile.print(":");
    myFile.print(accum_time_minute);
    myFile.print(":");
    myFile.print(accum_time_sec);
    myFile.print(", ");
    myFile.print(max_velocity);
    myFile.print(", ");
    myFile.print(distance);
    myFile.println(", ");
    myFile.close();
  }
  else{
    Serial.println("Error opening text for writing.");
  }
 
 WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(server, httpPort)) {
    return;
  }
 
 if (client.connect(server, httpPort)) { // Http 요청 헤더
  
  myFile = SD.open(date);
  
  if(myFile){
    
    while(myFile.available()){
      String line = myFile.readStringUntil(',');
 
      if(ii%3==0){
        accum_time_string = line;
      }
      else if(ii%3==1){
        max_velocity = line.toFloat();
      }
      else if(ii%3==2){
        distance = line.toFloat();
      }
      ii++;
    }
    myFile.close();
  }
  client.print("GET /insert_driveinfo.php");//여기서
  client.print("date=");
  client.print(date_send);
  client.print("accum_time=");
  client.print(accum_time_string);
  client.print("max_velocity=");
  client.print(max_velocity);
  client.print("distance=");
  client.print(distance);//여기까지는 PHP에서 받기로한 데이터들이다
  client.println(" HTTP/1.1");
  client.println("Host: 192.168.0.8"); //요청을 보낼 서버의 주소
  client.println("Content-Type: application/x-www-form-urlencoded");
  client.println("Connection: close");
  client.println();
  client.println(); 
  client.stop();
  Serial.println("OK");
 }
//과속한 경우 안내음성
  if(instant_velocity > 20000){
        over_speed++;
        i=1;
        if(i==1){
            mp3_play (1); //0001과속.mp3 파일 플레이
            i=0;
        }
 
          myFile = SD.open(howManySpeedOver, FILE_WRITE);
          dt3 = clock.getDateTime();
          if(myFile){
            myFile.print(dt3.hour);
            myFile.print(":");
            myFile.print(dt3.minute);
            myFile.print(":");
            myFile.print(dt3.second);
            myFile.print(", ");
            myFile.print(over_speed);
            myFile.println(", ");
            myFile.close();
          }
          else{
            Serial.println("Error opening text for writing.");
          }
 
  }
}//loop
 
//인터럽트 함수-------------------------------------------------------------
void saddleInterrupt() {
  int reading1 = digitalRead(interruptPin1);
  if(reading1 != saddleState) {
    saddleState = reading1;
  }
} 
void leftInterrupt() {
  int reading2 = digitalRead(interruptPin2);
  if(reading2 != leftState) {
    leftState = reading2;
  }
} 
void rightInterrupt() {
  int reading3 = digitalRead(interruptPin3);
  if(reading3 !=  rightState) {
    rightState = reading3;
  }
} 
void pedalInterrupt() {
  int reading4 = digitalRead(interruptPin4);
  if(reading4 != pedalState) {
    pedalState = reading4;
    if(pedalState == HIGH) {
      timenow = millis();
    }
  }
} 
