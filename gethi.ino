#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ArduinoJson.h>
#include "DHTesp.h"
#include <TridentTD_LineNotify.h>
#include <BlynkSimpleEsp8266.h>
 
const char* ssid = "Meda's Home_2.4G";               //กำหนด SSID
const char* password = "Boy@1234";     //กำหนด Password
#define LINE_TOKEN  "glIjdd9BL6Efsmb7GiVoreY0Eat22WeIcaK5YcRjFgD"
 String serverName = "http://www.qualityceramic.co.th";
const int ledPin =  LED_BUILTIN;// the number of the LED pin
//bool statuson= HIGH;
//bool statusoff= LOW;
bool statuson= HIGH;
bool statusoff= LOW;
BlynkTimer timer;
WiFiClient client;
 String mac;
  HTTPClient http;  //Declare an object of class HTTPClient
DHTesp dht;
 
  
void setup () {
  pinMode(12, OUTPUT);
  pinMode(14, OUTPUT);
  pinMode(13, OUTPUT);
  pinMode(15, OUTPUT);
  digitalWrite(12,statusoff );
  digitalWrite(13,statusoff );
  digitalWrite(14,statusoff );
  digitalWrite(15,statusoff );
 dht.setup(16, DHTesp::DHT22); 
   LINE.setToken(LINE_TOKEN);
  Serial.begin(9600);
 
  WiFi.begin(ssid, password);
  
  while (WiFi.status() != WL_CONNECTED) {

 
    delay(5000);
    Serial.print("Connecting..");
 
  }

 Serial.print("Connected");
  mac=WiFi.macAddress();
   LINE.notify(mac);
 timer.setInterval(1000,getport);
 timer.setInterval(60000,portsend);
 
}



void getdata(int portid){

//delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  String linetext;

 String  serverparth = serverName +"/boy/?opt=1&mac="+mac+"&port="+portid;
// String  serverparth = serverName +"/?mac="+mac+"&port="+portid+"&sensor="+(float)temperature+"&type=1";
 
http.begin(client,serverparth);
 
    // Serial.println(serverparth + http.getString()); 
    int httpCode = http.GET();                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
    
      
      
        const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters

      
      int id1 = root["0"]["port"]; // 1
      const char* mode1 = root["0"]["mode"]; // "Leanne Graham"
    int status1= root["0"]["status"]; 
    String namedev= root["0"]["name"]; // "Bret"
       String mode_name= root["0"]["modename"]; // "Bret"
   
      // Output to serial monitor
       Serial.print("Port :");
      Serial.println(id1);
      Serial.print("Mode:");
      Serial.println(mode1);
      Serial.print("Status:");
      Serial.println(status1);
     Serial.println(serverparth + http.getString()); 


if(status1==1){

  if(digitalRead(portid)!=statuson){


linetext=portid+" "+namedev+"  "+mode_name+" Port "+(String)id1+" on Temp="+temperature+"HD="+humidity;

LINE.notify(linetext);
    
  }


  
              Serial.println(status1);
  digitalWrite(id1,statuson );
  
     }else if(status1==0){


        if(digitalRead(portid)!=statusoff){

linetext=portid+" "+namedev+"  "+mode_name+" Port "+(String)id1+" off Temp="+temperature+"HD="+humidity;

LINE.notify(linetext);
    
  }
  
 Serial.println(status1);
     
   digitalWrite(id1,statusoff );
    }


  
 
    http.end();   //Close connection
     
 // }
 
//Send a request every 30 seconds

    }
}



void getport(){

getdata(15);
getdata(14);
getdata(12);
getdata(13);
  delay(3000);  
}




void portdata(int portid){

//delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();
  String linetext;

 String  serverparth = serverName +"/boy/?opt=1&mac="+mac+"&port="+portid;
// String  serverparth = serverName +"/?mac="+mac+"&port="+portid+"&sensor="+(float)temperature+"&type=1";
 
http.begin(client,serverparth);
 
    // Serial.println(serverparth + http.getString()); 
    int httpCode = http.GET();                                  //Send the request
 
    if (httpCode > 0) { //Check the returning code
 
    
      
      
        const size_t bufferSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(5) + JSON_OBJECT_SIZE(8) + 370;
      DynamicJsonBuffer jsonBuffer(bufferSize);
      JsonObject& root = jsonBuffer.parseObject(http.getString());
      // Parameters

      
      int id1 = root["0"]["port"]; // 1
      const char* mode1 = root["0"]["mode"]; // "Leanne Graham"
    int status1= root["0"]["status"]; 
    String namedev= root["0"]["name"]; // "Bret"
       String mode_name= root["0"]["modename"]; // "Bret"
   
      // Output to serial monitor
       Serial.print("Port :");
      Serial.println(id1);
      Serial.print("Mode:");
      Serial.println(mode1);
      Serial.print("Status:");
      Serial.println(status1);
     Serial.println(serverparth + http.getString()); 


if(status1==1){

  if(digitalRead(portid)!=statuson){


linetext=namedev+"  "+mode_name+" Port "+(String)id1+" on Temp="+temperature+"HD="+humidity;

LINE.notify(linetext);
   
  }
     Serial.println(status1);
  digitalWrite(id1,statuson );
  
     }else if(status1==0){


        if(digitalRead(portid)!=statusoff){

linetext=namedev+"  "+mode_name+" Port "+(String)id1+" off Temp="+temperature+"HD="+humidity;

LINE.notify(linetext);
    
  }
  
 Serial.println(status1);
     
   digitalWrite(id1,statusoff );
    }


   delay(1000); 
  serverparth = serverName +"/boy/?opt=2&temp="+temperature+"&hd="+humidity+"&mac="+mac;

http.begin(client,serverparth);
  http.GET();  
Serial.println(serverparth); 


bool portstatus=digitalRead(portid);
   
   serverparth = serverName +"/boy/?opt=3&port="+portid+"&status="+portstatus+"&mac="+mac;

http.begin(client,serverparth);
 http.GET();  
Serial.println(serverparth); 
   
 
    http.end();   //Close connection
     
 // }
 
//Send a request every 30 seconds

    }
}





void portsend(){

portdata(12);

  
}













 
void loop() {
  timer.run(); // run BlynkTimer
//getport();
  //  delay(3000);    
 
}
