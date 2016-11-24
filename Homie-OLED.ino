#include <Homie.h>
#include <U8g2lib.h>

#define PIN_LED D0

//U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D6, /* data=*/ D5, /* reset=*/ U8X8_PIN_NONE);
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D6, /* data=*/ D5, /* reset=*/ U8X8_PIN_NONE); 


HomieNode displayNode("oled", "oled");

AsyncMqttClient mqttClient;

bool displayHandler(HomieRange range, String value) {
  u8g2.clearBuffer(); 
  u8g2.drawStr(0,25, value.c_str() );
  u8g2.sendBuffer();
}

void onMqttConnect(bool sessionPresent) {
  uint16_t packetIdSub = mqttClient.subscribe("devices/0fc9bdef/temperature/Fahrenheit", 2);
  Serial.print("Subscribing at QoS 2, packetId: ");
  Serial.println(packetIdSub);
}

void onMqttMessage(char* topic, char* payload, AsyncMqttClientMessageProperties properties, size_t len, size_t index, size_t total) {
  Serial.println("** Publish received **");
  Serial.print("  topic: ");
  Serial.println(topic);
  Serial.print("  qos: ");
  Serial.println(properties.qos);
  Serial.print("  dup: ");
  Serial.println(properties.dup);
  Serial.print("  retain: ");
  Serial.println(properties.retain);
  Serial.print("  len: ");
  Serial.println(len);
  Serial.print("  index: ");
  Serial.println(index);
  Serial.print("  total: ");
  Serial.println(total);
  Serial.print("  payload: ");
  Serial.println(payload);
}

void setup() {
  Serial.begin(115200);
  u8g2.begin();
  u8g2.clearBuffer(); // clear the internal memory
  u8g2.setFont(u8g2_font_victoriabold8_8r); // choose a suitable font
  u8g2.drawStr(0,10,"Temp/Humidity");  // write something to the internal memory
//  u8g2.setFont(u8g2_font_ncenB18_tf); // choose a suitable font
//  u8g2.drawStr(0,10,"80F");  // write something to the internal memory
//  u8g2.drawStr(0,20,"60%");  // write something to the internal memory
  u8g2.sendBuffer();          // transfer internal memory to the display

  Homie_setFirmware("oled-display", "0.0.1");
  Homie.setLedPin(PIN_LED, HIGH);

  displayNode.advertise("display").settable(displayHandler);

  mqttClient = Homie.getMqttClient();
  mqttClient.onConnect(onMqttConnect);

  Homie.setup();
}

void loop() {
   Homie.loop();
}
