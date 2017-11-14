#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266mDNS.h>
#include <SPI.h>
#include <SD.h>
#include <SoftwareSerial.h>
#include <TinyGPS.h>


const char* ssid = "EWHA-IOT";
const char* password = "dscho007";
char server[]="192.168.0.8";

TinyGPS gps;
SoftwareSerial ss(4, 5);

#define FREQUENCY 2400
File myFile;

double earthR = 6371;
double LatA = 0;
double LonA = 0;
double DistA = 0;//km 단위로 변경
double LatB = 0;
double LonB = 0;
double DistB = 0;
double LatC = 0;
double LonC = 0;
double DistC = 0;
int ii=0;

struct SCAN{
  String s;
  int r;
  };

 struct DISTANCE{
  String ssid; // ap 이름
  double distance;//km
  };


void sort(int n, struct SCAN arr[]){
  int i,j;
  struct SCAN temp;

  for(i=0;i<n;i++){
    for(j=0;j<n-1;j++){
      if(arr[i].r>arr[j].r)
      {
        temp=arr[i];
        arr[i]=arr[j];
        arr[j]=temp;
      }
    }  
  }
}


void bestClose(struct SCAN arr[], struct DISTANCE d[3]){
    Serial.println("------------------가장 가까운 3개--------------------------");
    Serial.print("First");
    Serial.print(" => ");
    Serial.print(arr[0].s);
    Serial.print(":");
    Serial.println(arr[0].r);
    Serial.print("Second");
    Serial.print(" => ");
    Serial.print(arr[1].s);
    Serial.print(":");
    Serial.println(arr[1].r);
    Serial.print("Third");
    Serial.print(" => ");
    Serial.print(arr[2].s);
    Serial.print(":");
    Serial.println(arr[2].r);
    Serial.println("-----------------------------------------------------------");

    delay(10);

    for(int i=0;i<3;++i){
      d[i].ssid=arr[i].s;
      d[i].distance = (pow(10,((27.55 - (20 * log10(FREQUENCY)) + abs(arr[i].r))/20.0))); //km로 환산
      Serial.print(d[i].ssid);
      Serial.print(" : ");
      Serial.println(d[i].distance,14);
      }
  }

//////////////////////////////////////////////////////////////////////////////////////////////////////

void match(int i,struct DISTANCE d[3]){ 

    //내가 설치해둔 ap의 이름과 위치에 대한 정보가 저장되어있다(차후 table을 따로 작성해서 파일을 불러오는 식으로 구성할 예정)

   if(d[i].ssid=="AP1"){
      if(i==0){
     LatA= 37.568577;
     LonA= 126.967857;
    }
    else if(i==1){
     LatB= 37.568577;
     LonB= 126.967857;
    }
    else if(i==2){
     LatC= 37.568577;
     LonC= 126.967857;
      }
    }

    else if(d[i].ssid=="AP2"){
     if(i==0){
     LatA= 37.568271;
     LonA= 126.964563;
    }
    else if(i==1){
     LatB= 37.568271;
     LonB= 126.964563;
    }
    else if(i==2){
     LatC= 37.568271;
     LonC= 126.964563;
      }
    }
   
    else if(d[i].ssid=="AP3"){
     if(i==0){
     LatA= 37.565581;
     LonA= 126.967046;
    }
    else if(i==1){
     LatB= 37.565581;
     LonB= 126.967046;
    }
    else if(i==2){
     LatC= 37.565581;
     LonC= 126.967046;
      }
    }

    else{
      Serial.println("No matching wifi name");
      }

  }
  
///////////////////////////////////////////////////////////////////////////////////////////////


  
void setup() {
  Serial.begin(115200);
  ss.begin(9600); // gps baud
  WiFi.begin(ssid, password);
  ////////////////////////////////////////////////////////////////
  
  pinMode(15, OUTPUT);
  digitalWrite(15, HIGH);   //15번 - sd카드
  Serial.print("Initializing SD card...");
  if (!SD.begin(15)) {
    Serial.println("initialization failed!");
    return;
  }

  Serial.println("initialization done.");
  
  //////////////////////////////////////////////////////////////

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.print("Connection : ");
  Serial.println(ssid);
  //////////////////////////////////////////////////////////////////////////////
  
  // Set WiFi to station mode and disconnect from an AP if it was previously connected
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  delay(100);
  Serial.println("Setup done");
}

void loop() {

   WiFiClient client;
  const int httpPort = 80;
  
  if (!client.connect(server, httpPort)) {
    return;
  }
 
  // WiFi.scanNetworks will return the number of networks found
  int n = WiFi.scanNetworks();
  int count=0;
  char ch1[6]="AP";
  struct SCAN scan[n];
  struct DISTANCE d[3]; // 여기 저장된 wifi 이름과 distance(반지름)를 이용하여 삼각측량 시작
  double lat;
  double lon;
  double r_lat;
  double r_lon;

  //////////////////////////////////////////////////////////////////////////////////////// gps 센서용 변수 선언
  
  bool newData = false;
  unsigned long chars;
  unsigned short sentences, failed;
  unsigned short save_sentences;
  int year;
  byte month, day, hour, minute, second, hundredths;

//////////////////////////////////////////////////////////////////////////////////////////위성으로부터 받은 경도,위도,시간 값 받아오기

for (unsigned long start = millis(); millis() - start < 1000;)
  {
    while (ss.available())
    {
      char c = ss.read();
      if (gps.encode(c)) 
        newData = true;
    }
  }

  if (newData)
  {
    float flat, flon;
    unsigned long age;
    gps.f_get_position(&flat, &flon, &age);
    gps.crack_datetime(&year,&month,&day,&hour,&minute,&second,&hundredths);
    Serial.print("Date: ");
    Serial.print(month, DEC);
    Serial.print("/");
    Serial.print(day, DEC);
    Serial.print("/");
    Serial.println(year);
    Serial.print("Time: ");
    Serial.print(hour+9, DEC);
    Serial.print(":");
    Serial.print(minute, DEC);
    Serial.print(":");
    Serial.print(second, DEC);
    Serial.print(".");
    Serial.println(hundredths, DEC);
    Serial.println("");


    
    Serial.print("latitude : ");
    lat=(flat == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flat);
    Serial.println(lat, 13);
    Serial.print("longitude : ");
    lon=(flon == TinyGPS::GPS_INVALID_F_ANGLE ? 0.0 : flon);
    Serial.println(lon, 13);
  
  }

  save_sentences=sentences;
  
  gps.stats(&chars, &sentences, &failed);

  /*  
  Serial.print(" CHARS=");
  Serial.print(chars);
  Serial.print(" SENTENCES=");
  Serial.print(sentences);
  Serial.print(" CSUM ERR=");
  Serial.println(failed);
  
  if (chars == 0)
    Serial.println("** No characters received from GPS: check wiring **");
  */


/////////////////////////////////////////////////////////////////////////////////////// 삼각측량 코드 (gps 센서가 작동이 불가한 상황)

  if(sentences==save_sentences){ //gps센서가 위성을 감지하지 못하는 상황
    Serial.println("Wifi scan start");

    Serial.println("scan done");
    if (n == 0)
    Serial.println("no networks found");
    else
   {
    Serial.print(n);
    Serial.println(" networks found");  

    
    for (int i = 0; i < n; ++i)
    {
      char ch2[30];
      String s=WiFi.SSID(i);
      s.toCharArray(ch2,30);
      
      if(!strncmp(ch1,ch2,2)){
      scan[count].s=WiFi.SSID(i);
      scan[count].r=WiFi.RSSI(i);
      Serial.print("ssid: ");
      Serial.println(scan[count].s);
      Serial.print("rssi: ");
      Serial.println(scan[count].r);
      
      count++;
      }
      delay(10);
    } 
  }

  //////////////////////////
  
  sort(count,scan);
  bestClose(scan,d);

  //////////////////////////


    
    DistA =(d[0].distance/1000);//km 단위로 변경
   
    DistB =(d[1].distance/1000);
   
    DistC =(d[2].distance/1000);

   match(0,d);
   match(1,d);
   match(2,d);

   Serial.print("match latitude A = ");
   Serial.println(LatA,14);
   Serial.print("match lontitude A = ");
   Serial.println(LonA,14);
   Serial.print("match distance A = ");
   Serial.println(DistA,14);
   Serial.print("match latitude B = ");
   Serial.println(LatB,14);
   Serial.print("match lontitude B = ");
   Serial.println(LonB,14);
   Serial.print("match latitude C = ");
   Serial.println(LatC,14);
   Serial.print("match lontitude C = ");
   Serial.println(LonC,14);

    double xA = earthR *(cos(radians(LatA)) * cos(radians(LonA)));
    double yA = earthR *(cos(radians(LatA)) * sin(radians(LonA)));
    double zA = earthR *(sin(radians(LatA)));

    double xB = earthR *(cos(radians(LatB)) * cos(radians(LonB)));
    double yB = earthR *(cos(radians(LatB)) * sin(radians(LonB)));
    double zB = earthR *(sin(radians(LatB)));

    double xC = earthR *(cos(radians(LatC)) * cos(radians(LonC)));
    double yC = earthR *(cos(radians(LatC)) * sin(radians(LonC)));
    double zC = earthR *(sin(radians(LatC)));

    double P1[3] = {xA, yA, zA}; //P1 = numpy.array([xA, yA, zA])
    double P2[3] = {xB, yB, zB}; //P2 = numpy.array([xB, yB, zB])
    double P3[3] = {xC, yC, zC}; //P3 = numpy.array([xC, yC, zC])

    double S1[3]={xB-xA, yB-yA, zB-zA}; //P2-P1
    double S2[3]={xC-xA, yC-yA, zC-zA}; //P3-P1

    double normS1=(sqrt(pow(S1[0],2)+pow(S1[1],2)+pow(S1[2],2)));// d
    double normS2=(sqrt(pow(S2[0],2)+pow(S2[1],2)+pow(S2[2],2)));

    //from wikipedia
    //transform  get circle 1 at origin
    //transform  get circle 2 on x axis

    double ex[3] = {(S1[0]/normS1), (S1[1]/normS1), (S1[2]/normS1)};  //ex=(P2 - P1)/(numpy.linalg.norm(P2 - P1))


    double i=ex[0]*S2[0]+ex[1]*S2[1]+ex[2]*S2[2];//벡터의 내적


    double M[3]={(ex[0]*i), (ex[1]*i), (ex[2]*i)}; //i*ex

    double S3[3]={S2[0]-M[0], S2[1]-M[1],S2[2]-M[2]};//P3 - P1 - i*ex
    double normS3=(sqrt(pow(S3[0],2)+pow(S3[1],2)+pow(S3[2],2)));// 벡터의 크기

    double ey[3]= {(S3[0]/normS3), (S3[1]/normS3), (S3[2]/normS3)}; //ey = (P3 - P1 - i*ex)/(numpy.linalg.norm(P3 - P1 - i*ex))
    double ez[3]={(ex[1]*ey[2])-(ey[1]*ex[2]),-((ex[0]*ey[2])-(ey[0]*ex[2])),(ex[0]*ey[1])-(ey[0]*ex[1]) };
     //ez = numpy.cross(ex,ey) 외적

    double j=ey[0]*S2[0]+ey[1]*S2[1]+ey[2]*S2[2];//벡터의 내적

    

    double x = (pow(DistA,2) - pow(DistB,2) + pow(normS1,2))/(2*normS1);
    double y = ((pow(DistA,2) - pow(DistC,2) + pow(i,2) + pow(j,2))/(2*j)) - ((i/j)*x);
    // only one case shown here
    double z=sqrt(abs(pow(DistA,2) - pow(x,2) - pow(y,2))); //z = numpy.sqrt(pow(DistA,2) - pow(x,2) - pow(y,2))
  
    
    
    double triPt[3] = {P1[0] + x*ex[0] + y*ey[0] + z*ez[0], 
                      P1[1] + x*ex[1] + y*ey[1] + z*ez[1], 
                      P1[2] + x*ex[2] + y*ey[2] + z*ez[2]};

    //convert back  lat/long from ECEF
    //convert  degrees
    lat = degrees(asin(triPt[2] / earthR));
    lon = degrees(atan2(triPt[1],triPt[0]));
    
    Serial.print("latitude : ");
    Serial.println(lat,13);
    Serial.print("longitude : ");
    Serial.println(lon,13);

    }

    
/////////////////////////////////////////////////////////////////////////////////// SD카드에 아이의 위치 입력

   myFile = SD.open("Location", FILE_WRITE);
   if (myFile) {
    
    myFile.print("Latitude : ");   
    myFile.println(lat);
    myFile.print("Longitude : ");  
    myFile.println(lon); 
    myFile.close();
    
  } 
  else {
  
  // if the file didn't open, print an error:
  Serial.println("error opening text for writing");
 
  }
 
 //////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  Serial.println("");

  if (client.connect(server, httpPort)) { // Http 요청 헤더
     
  /////////////////////////////////////////////////////////////////////////////////// SD카드로부터 위치값을 다시 불러옴
   
   myFile = SD.open("Location");
  
   if(myFile){
    
    while(myFile.available()){
      String line = myFile.readStringUntil(',');
      
      if(ii%2==1){
        r_lat = line.toInt();
      }
      else if(ii%2==0){
        r_lon = line.toInt();
      }
      ii++;
    }
    myFile.close();
  }
 
    client.print("GET /insert_location.php");
    client.print("latitude=");
    client.print(r_lat);
    client.print("longitude=");
    client.print(r_lon);
    client.println(" HTTP/1.1");
    client.println("Host: 192.168.0.8"); //요청을 보낼 서버의 주소
    client.println("Content-Type: application/x-www-form-urlencoded");
    client.println("Connection: close");
    client.println();
    client.println(); 
    client.stop();
    Serial.println("OK");
 }

 

  //////////////////////////////와이파이 위도,경도 초기화  
  LatA = 0;
  LonA = 0;
  DistA = 0;
  LatB = 0;
  LonB = 0;
  DistB = 0;
  LatC = 0;
  LonC = 0; 
  DistC = 0;
  
  delay(5000);
}
