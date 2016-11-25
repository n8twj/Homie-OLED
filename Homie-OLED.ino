#include <Homie.h>
#include <U8g2lib.h>

#define PIN_LED D0

U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(U8G2_R0, /* clock=*/ D6, /* data=*/ D5, /* reset=*/ U8X8_PIN_NONE); 

HomieNode displayNode("display", "display");
AsyncMqttClient mqttClient;

bool displayHandler(HomieRange range, String value) {
  display.clearBuffer(); 
  display.setFont(u8g2_font_ncenB08_tf);
  display.drawStr(0,24,"Flowering Room");
  display.setFont(u8g2_font_profont22_mf ); // choose a suitable font
  display.drawStr(13,45,"74F - 55%");  // write something to the internal memory
  display.setFont(u8g2_font_ncenB08_tf);
  display.drawStr(0,55, value.c_str() );
  display.sendBuffer();          // transfer internal memory to the display
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("** Publish received **");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("payload: ");
  Serial.println(payload);
}

void onHomieEvent(const HomieEvent& event) {
  switch(event.type) {
     case HomieEventType::MQTT_CONNECTED:
       // this gets called by homie
       uint16_t packetIdSub = mqttClient.subscribe("test/message", 0);
       Serial.print("Subscribing at QoS 0, packetId: ");
       Serial.println(packetIdSub); // packetId: 0

       packetIdSub = mqttClient.subscribe("test/message2", 0);
       Serial.print("Subscribing at QoS 0, packetId: "); 
       Serial.println(packetIdSub);  // packetId: 0

       mqttClient.onMessage(onMqttMessage); // never gets called
        
       break;    
  }
}

void setup() {
  Serial.begin(115200);
  display.begin();
  display.clearBuffer();

  Homie_setFirmware("oled-display", "0.0.1");
  Homie.setLedPin(PIN_LED, LOW);
  Homie.onEvent(onHomieEvent); 
  
//displayNode.advertise("temperature").settable(displayTempHandler);
//displayNode.advertise("humidity").settable(displayHumidityHandler);
  displayNode.advertise("message").settable(displayHandler);  
  mqttClient = Homie.getMqttClient();
  
  Homie.setup();
}

void loop() {
   Homie.loop();
}
