



#include <SoftwareSerial.h>
SoftwareSerial espSerial =  SoftwareSerial(2,3);      // arduino RX pin=2  arduino TX pin=3    connect the arduino RX pin to esp8266 module TX pin   -  connect the arduino TX pin to esp8266 module RX pin



int A,B;


String apiKey = "IBD8KYFEM1V22HWL";     // replace with your channel's thingspeak WRITE API key

String ssid="OSToto_E447" ; // Wifi network SSID
String password ="1234567890";  // Wifi network password

boolean DEBUG=true;

//======================================================================== showResponce
void showResponse(int waitTime){
    long t=millis();
    char c;
    while (t+waitTime>millis()){
      if (espSerial.available()){
        c=espSerial.read();
        if (DEBUG) Serial.print(c);
      }
    }
                   
}

//========================================================================
boolean thingSpeakWrite(float value1, float value2){
  String cmd = "AT+CIPSTART=\"TCP\",\"";                  // TCP connection
  cmd += "184.106.153.149";                               // api.thingspeak.com
  cmd += "\",80";
  espSerial.println(cmd);
  if (DEBUG) Serial.println(cmd);
  if(espSerial.find("Error")){
    if (DEBUG) Serial.println("AT+CIPSTART error");
    return false;
  }
  
  
  String getStr = "GET /update?api_key=";   // prepare GET string
  getStr += apiKey;
  
  getStr +="&field1=";
  getStr += String(value1);
  getStr +="&field2=";
  getStr += String(value2);
  // getStr +="&field3=";
  // getStr += String(value3);
  // ...
  getStr += "\r\n\r\n";

  // send data length
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());
  espSerial.println(cmd);
  if (DEBUG)  Serial.println(cmd);
  
  delay(100);
  if(espSerial.find(">")){
    espSerial.print(getStr);
    if (DEBUG)  Serial.print(getStr);
  }
  else{
    espSerial.println("AT+CIPCLOSE");
    // alert user
    if (DEBUG)   Serial.println("AT+CIPCLOSE");
    return false;
  }
  return true;
}
//================================================================================ setup
void setup() {                
 DEBUG=true;           // enable debug serial
  Serial.begin(115200); 
  
  //dht.begin();          // Start DHT sensor
  
  espSerial.begin(115200);  
  

  espSerial.println("AT+CWMODE=1");   // set esp8266 as client
  showResponse(1000);

  espSerial.println("AT+CWJAP=\""+ssid+"\",\""+password+"\"");  // set your home router SSID and password
  showResponse(5000);

   if (DEBUG)  Serial.println("Setup completed");
}


// ====================================================================== loop


 void loop()
{
  A = analogRead(0);       // read analog input pin 0
  Serial.println("MQ7");
  Serial.print(A, DEC);  // prints the value read
  Serial.println("%");
    Serial.println();
  B = analogRead(1); 
    Serial.println("MQ135");
    Serial.print(B, DEC);  // prints the value read
  Serial.println("%");
   // Serial.println("dig");
  //Serial.print(limit, DEC); 
//Serial.println();
           thingSpeakWrite(A,B);                                      // Write values to thingspeak
      
  
    
  // thingspeak needs 15 sec delay between updates,     
  delay(5000);  
}




