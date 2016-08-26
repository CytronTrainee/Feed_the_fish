#include <ESPert.h>
#include <Servo.h>

ESPert espert;
Servo myservo;

const char* mqtt_server = "mqtt.espert.io";

//*** PUSH MSG TO PHONE
const char *host = "www.espert.io";
const int httpPort = 80;
const char *path = "/MySmartphone/send?key=xxxxxxxxxxxxxxxx&message=Feed%20the%20FiSh"; //xxx is the key for phone from espert.io

int currentSwitch = true;
String outTopic = "ESPert/" + String(espert.info.getChipId()) + "/FEED";
String inTopic = "ESPert/" + String(espert.info.getChipId()) + "/FEED";


void callback(char* topic, byte* payload, unsigned int length) 
{
String strPayload = String((char*)payload).substring(0, length);
espert.println("Receive: " + strPayload);

myservo.attach(14); //set servo to pin14
int pos ;
int y;

  if (espert.json.init(strPayload)) {
    if (espert.json.containsKey("cmd")) {
      String value = espert.json.get("cmd");

      
  if (value == "1") 
  {
        espert.led.on();    //TURN ON LED
        espert.println("LED: On");
        
  //*** SEND MESSAGE
    espert.println("Connecting...");
    espert.println(">>" + espert.wifi.getHTTP(host, path) + "<<");      //SEND "FEED THE FISH"
    espert.oled.println("Feeding");
    delay(2000);
    

  //*** initiate servo
    for (y = 0; y <2; y++)
    {
      for (pos = 0; pos <= 180; pos += 8)  // goes from 0 degrees to 180 degrees
      {myservo.write(pos);                  // tell servo to go to position in variable 'pos'
      delay(50);}   
    
      for (pos = 180; pos >= 0; pos -= 8)  // goes from 180 degrees to 0 degrees
      {myservo.write(pos);                  // tell servo to go to position in variable 'pos'
       delay(50);}
    }
     
  //*** SEND MESSAGE
    const char *path = "/MySmartphone/send?key=xxxxxxxxxxxxxxxx&message=DONE"; //xxx is the key for phone from espert.io
    espert.println("Connecting...");
    espert.println(">>" + espert.wifi.getHTTP(host, path) + "<<");      //SEND "DONE"
    espert.oled.println("D O N E");
    delay(2000);

 espert.led.off();  //TURN OFF LED
 espert.println("LED: Off");
 delay(3000);
 espert.oled.clear();
  }

    
    }
  }
}

void setup() {
  espert.init();
  espert.mqtt.init(mqtt_server, 1883, callback);
  espert.oled.init();
  delay(2000);
  espert.oled.clear();
  espert.oled.println(espert.info.getId());
  espert.oled.println();

myservo.write(0);  //SET THE INITIAL POSITION FOR SERVO

  int mode = espert.wifi.init();

  if (mode == ESPERT_WIFI_MODE_CONNECT)
  {
    espert.println(">>> WiFi mode: connected.");
    espert.oled.println("WiFi: connected.");
    espert.oled.print("IP..: ");
    espert.oled.println(espert.wifi.getLocalIP());
  } 
  else if (mode == ESPERT_WIFI_MODE_DISCONNECT)
  {
    espert.println(">>> WiFi mode: disconnected.");
    espert.oled.println("WiFi: not connected.");
  }
}

void loop() {
  
 if (espert.mqtt.connect()) 
  {
    espert.println("MQTT: Connected");
    espert.oled.println("MQTT: Connected.");
    espert.println("MQTT: Out Topic " + outTopic);
    espert.mqtt.subscribe(inTopic);
    Serial.println("MQTT: Subscribed " + inTopic);
  }
    
}
