#include <Homie.h>
#include <U8g2lib.h>

#define PIN_LED D0

U8G2_SSD1306_128X64_NONAME_F_SW_I2C display(U8G2_R0, /* clock=*/ D6, /* data=*/ D5, /* reset=*/ U8X8_PIN_NONE); 

HomieNode displayNode("display", "display");
AsyncMqttClient& mqttClient = Homie.getMqttClient();

bool displayHandler(HomieRange range, String value) {
  display.clearBuffer(); 
  display.setFont(u8g2_font_ncenB08_tf);
  display.drawStr(0,24,"Flowering Room");
  display.setFont(u8g2_font_profont22_mf );  // choose a suitable font
  display.drawStr(13,45,"74F - 55%");        // write something to the internal memory
  display.setFont(u8g2_font_ncenB08_tf);
  display.drawStr(0,55, value.c_str() );
  display.sendBuffer();                      // transfer internal memory to the display
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("** Publish received **");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("payload: ");
  Serial.println(payload);
 // display.drawStr(0,55, "" );
  display.drawStr(0,55, payload);
  display.sendBuffer(); 
}

void onHomieEvent(const HomieEvent& event) {
  switch(event.type) {
   case HomieEventType::MQTT_CONNECTED:
    uint16_t packetIdSub = mqttClient.subscribe("sensors/3/temperature", 0);
    Serial.print("Subscribing at QoS 0, packetId: ");
    Serial.println(packetIdSub);
    break;    
  }
}

void setup() {
  Serial.begin(115200);
  display.begin();
  display.clearBuffer();
  display.setFont(u8g2_font_ncenB08_tf);

  Homie_setFirmware("oled-display", "0.0.1");
  Homie.setLedPin(PIN_LED, LOW);
  Homie.onEvent(onHomieEvent); 
  mqttClient.onMessage(onMqttMessage);
   
//displayNode.advertise("temperature").settable(displayTempHandler);
//displayNode.advertise("humidity").settable(displayHumidityHandler);
  displayNode.advertise("message").settable(displayHandler);  

  Homie.setup();
}

void loop() {
   Homie.loop();
}
