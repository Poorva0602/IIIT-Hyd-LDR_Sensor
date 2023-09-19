#include <ESP8266HTTPClient.h>
#include <ESP8266WiFi.h>

#define MAIN_SSID "RAHUL4G"
#define MAIN_PASS "poorva2000"
#define CSE_IP      "192.168.29.61"
#define CSE_PORT    8000
#define HTTPS     false
#define OM2M_ORGIN    "admin:admin"
#define OM2M_MN     "/~/mn-cse/mn-name/"
#define OM2M_AE_0     "LAMP_0"
#define OM2M_AE_1     "LAMP_1"
#define OM2M_DATA_CONT  "DATA"
#define LISTENER_PORT  8000

WiFiServer listener(LISTENER_PORT);
HTTPClient http; 

#define LED1 D4
#define LED2 A0

void setup()
{
   Serial.begin(115200);
   delay(5000);
   Serial.println("Connecting to "+ String()+ MAIN_SSID);
    WiFi.begin(MAIN_SSID, MAIN_PASS);
    delay(5000);


   
   listener.begin(LISTENER_PORT);
   Serial.println("ESP listener started");
   Serial.println(" Ip Address is" + WiFi.localIP());
   //Lamp-0 Object 
   String server="http://" + String() + CSE_IP + ":" + String() + CSE_PORT + String()+OM2M_MN;
   http.begin(server + String() +OM2M_AE_0 + "/" + OM2M_DATA_CONT + "/");
   http.addHeader("X-M2M-Origin", OM2M_ORGIN);
   http.addHeader("Content-Type", "application/json;ty=23");
   http.addHeader("Content-Length", "100");
   String req_data = String() + "{\"m2m:sub\": {"
     + "\"rn\": \"led_sub_test\""+","
     + "\"nu\": \"" + "http://"+WiFi.localIP().toString()+":"+LISTENER_PORT + "\","
     + "\"nct\": \"2\""
     + "}}";
   int x=http.POST(req_data);
   Serial.println(x);
   http.end();
   //Lamp-1 Object
   http.begin(server + String() +OM2M_AE_1 + "/" + OM2M_DATA_CONT + "/");
   http.addHeader("X-M2M-Origin", OM2M_ORGIN);
   http.addHeader("Content-Type", "application/json;ty=23");
   http.addHeader("Content-Length", "100");
   req_data = String() + "{\"m2m:sub\": {"  
     + "\"rn\": \"led_sub_test\""+","
     + "\"nu\": \"" + "http://"+WiFi.localIP().toString()+":"+LISTENER_PORT + "\","
     + "\"nct\": \"2\""
     + "}}";
   x =http.POST(req_data);
   Serial.println(x);
   http.end();
     pinMode(LED1,OUTPUT);
     pinMode(LED2,OUTPUT);
}
void loop(){
listener.begin(LISTENER_PORT);
WiFiClient client = listener.available();
 String w=client.readString();
Serial.println(w);
int a=w.indexOf("false");
int b=w.indexOf("true");
int c=w.indexOf("LAMP_0");
int d=w.indexOf("LAMP_1");

if(b>0)
{
  if(c>0)
  {
  digitalWrite(LED1,HIGH);
  Serial.println("Light-1 ON");
  }
  else if(d>0)
  {  digitalWrite(LED2,HIGH);
    Serial.println("Light-2 ON");
  }
}
else if(a>0)
{
  if(c>0)
  {
  digitalWrite(LED1,LOW);
  Serial.println("Light-1 OFF");
  }
  else if(d>0)
  {  digitalWrite(LED2,LOW);
    Serial.println("Light-2 OFF");
  }
}
client.flush();
// Send HTTP response to the client
String s = "HTTP/1.1 200 OK\r\n";
client.print(s);
delay(10);
}
