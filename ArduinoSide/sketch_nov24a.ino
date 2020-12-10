//#include <Ethernet.h>
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include <SoftwareSerial.h>
#include <LedControl.h>

LedControl lc=LedControl(D2,D4,D3,D1);//DIN,CLK,CS  use nodemcu's vcc and gnd
SoftwareSerial BT(D5,D6);//RX/TX
int BTV;
int LV;

const char ssid[] = "";//Wi-Fi ID
const char pass[] = "";//Wi-Fi Pwd
const char* host = "";//Database IP
 
int id;
int id_transfer[11]={0,127,63,126,31,125,62,124,15,123,61};
String data;
  
void setup(){   
  Serial.begin(9600);   
  delay(300);             //Let system settle  
  WiFi.begin(ssid, pass);
  Serial.println("");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Wi-Fi ready...");  
  delay(700);             //Wait rest of 1000ms recommended delay before   
                          //accessing sensor 
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP"); 
//  }
	
	
	BT.begin(19200);  // HC-06 current bound rate (default 9600)
	Serial.println("Bluetooth ready"); 
	//led ctrl
	lc.shutdown(0,false);  // 關閉省電模式
    lc.setIntensity(0,8);  // 設定亮度為 8 (介於0~15之間)
    lc.clearDisplay(0);    // 清除螢幕 
             
}
  
void loop(){
	if (BT.available())
	{
      BTV=BT.read();
      Serial.println(BTV);
	    callsever(BTV);
	}
}
void callsever(int id_input)
{
	if(id_input==0)//-1
	{
		lc.clearDisplay(0);
		id=0;
	}
	for(int i=1;i<6;i++)
	{
		if(id_input==id_transfer[i])
		{
			id=i;
		}
	}
  Serial.println(id);
  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort)) {
  Serial.println("connection failed");
  return;
  }
  if(id!=0)
  {
     String data = "id=" + String(id);
     Serial.print("Requesting POST: ");
     // Send request to the server:
     client.println("POST /test/tesp_output.php HTTP/1.1");
     client.println("Host: "+String(host));
     client.println("Accept: */*");
     client.println("Content-Type: application/x-www-form-urlencoded");
     client.print("Content-Length: ");
     client.println(data.length());
     client.println();
     client.print(data);
    delay(10);
    String getstr="GET /test/"+String(id)+".json HTTP/1.1";
    client.println(getstr);
    client.println("Host: "+String(host));
    client.println("Connection: close\r\n\r\n"); 
    delay(10);
    Serial.println(getstr);
    String section="HEAD";
    //Serial.println("11111111111111111111111111111111111111111111111111111111111111111111111111111");
    while(client.available())
    {
      String line = client.readStringUntil('\r');
      //Serial.print(line);
      if(section == "HEAD" )  // HEAD
      {
          Serial.print( "." );
          if( line == "\n" )  // 空白行
          {
              section = "LENGTH";
          }
      }
      else if( section == "LENGTH" )
      {
          Serial.print( "." );
          if( line == "\n" )  // 空白行
          {
              section = "JSON";
          }
      }
      else if( section == "JSON" )    // print the good stuff
      {
          Serial.println( "?" );
          section = "END";
          String jsonStr = line.substring(1); // 給定一個從索引到尾的字串
          
          // 開始解析 JSON
          int size = jsonStr.length() + 1;
          //Serial.print("string length= "); Serial.println(size);
          char json[size];
          jsonStr.toCharArray(json, size);
          //Serial.println("XXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXXX");
          //Serial.println( json );
          //Serial.println("YYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYYY");
          StaticJsonBuffer<400> jsonBuffer;
          JsonObject& jsonParsed = jsonBuffer.parseObject(json);
          if (!jsonParsed.success())
          {
              Serial.println("parseObject() failed");
              return;
          }
          int id=jsonParsed["feeds"][0]["id"];
          const char *lamp= jsonParsed["feeds"][0]["lamp"];
          int Time=jsonParsed["feeds"][0]["time"];

          //Serial.println("-- Decoding / Parsing --");
          Serial.print( "ID : " ); Serial.println( id );
          Serial.print( "lamp : " ); Serial.println( lamp );
          Serial.print( "time : " ); Serial.println( Time );
          //break;
		  
		  //shoosing lamp
		for(int u=0;u<64;u++)
		{
        int row=u/8;
        int column=u%8;
			  if(lamp[u]=='1')
			  {
				  
				  lc.setLed(0,row,column,true);
			  }
        else
        {
          lc.setLed(0,row,column,false);
        }
		  }
       }
    }
    //Serial.println("222222222222222222222222222222222222222222222222222222222222222222222222222");
    Serial.println();
    Serial.println("closing connection");
    client.stop();
  }
}	
