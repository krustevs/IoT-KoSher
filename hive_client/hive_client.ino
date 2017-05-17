
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <ArduinoJson.h>


// Update these with values suitable for your network.

#define SSID              "!!! YOUR WIFI HERE !!!"
#define PASSWORD          "!!! YOUR WIFI PASSWORD HERE !!!"

#define BBT               "mqtt.beebotte.com"                       // Domain name of Beebotte MQTT service
#define TOKEN             "token:!!! YOUR BEEBOTTE TOKEN HERE !!!"  // Set your channel token here
#define CHANNEL           "!!! YOUR BEEBOTTE CHANNEL HERE !!!"      // Replace with your channel name
#define RES_TEMPERATURE   "!!! YOUR RESOURCE HERE !!!"              // Replace with your resource name


WiFiClient espClient;
PubSubClient client(espClient);


void setup_wifi() {
  delay(10);

  Serial.printf("\nConnecting to %s\n", SSID);

  WiFi.begin(SSID, PASSWORD);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.printf("\nWiFi connected with IP:\n");
  Serial.println(WiFi.localIP());
}



const char * generateID()
{
  const char chars[] = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ1234567890";
  char id[17];
  randomSeed(analogRead(0));
  int i = 0;
  for (i = 0; i < sizeof(id) - 1; i++) {
    id[i] = chars[random(sizeof(chars))];
  }
  id[sizeof(id) - 1] = '\0';

  return id;
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    Serial.printf("Connecting to Beebotte...\n");

    if (client.connect(generateID(), TOKEN, "")) {
      Serial.printf("Success!\n");

    } else {
      Serial.printf("Failed: rc=%d try again in 5 seconds\n", client.state());
      delay(5000);
    }
  }
}

// publishes data to the specified resource
void publish(const char* resource, float data)
{
  StaticJsonBuffer<128> jsonOutBuffer;
  JsonObject& root = jsonOutBuffer.createObject();
  root["channel"] = CHANNEL;
  root["resource"] = resource;
  root["write"] = true;
  root["data"] = data;

  // Now print the JSON into a char buffer
  char buffer[128];
  root.printTo(buffer, sizeof(buffer));

  // Create the topic to publish to
  char topic[64];
  sprintf(topic, "%s/%s", CHANNEL, resource);

  // Now publish the char buffer to Beebotte
  Serial.printf("Publishing to topic %s: %s\n", topic, buffer);
  client.publish(topic, buffer);
}

// Mock function to simulate sensors.
// TODO: Replace with actual implementation
float read_sensors () {

  return random(20);
}


void setup() {

  Serial.begin(115200);
  setup_wifi();

  client.setServer(BBT, 1883);
  reconnect();

}

void loop() {
  client.loop();

  if (!client.connected()) {
    reconnect();
  }
  
  float data = read_sensors();

  publish(RES_TEMPERATURE, data);

  delay(2000);
}
