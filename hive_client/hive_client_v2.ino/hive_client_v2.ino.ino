
#include <SPI.h>
#include <ESP8266WiFi.h>
#include <ArduinoJson.h>
#include <ArduinoHttpClient.h>

#define SER_NUM "asd"

// Update these with values suitable for your network.

#define SSID              "FMI-AIR-316"
#define PASSWORD          ""

#define HOST              "10.110.200.210"
#define PORT              3000


WiFiClient espClient;
HttpClient client = HttpClient(espClient, HOST, PORT);

String response;
int statusCode = 0;


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

void setup() {

  Serial.begin(115200);
 
  setup_wifi();

}

float read_sensors () {
  return random(42);
}

// publishes data to the specified resource
void publish(float data)
{
  StaticJsonBuffer<128> jsonOutBuffer;
  JsonObject& root = jsonOutBuffer.createObject();
  root["device"] = SER_NUM;
  root["temperature"] = data;

  // Now print the JSON into a char buffer
  char buffer[128];
  root.printTo(buffer, sizeof(buffer));

  String contentType = "application/json";

  client.post("/hive_data", contentType, buffer);

    // read the status code and body of the response
  statusCode = client.responseStatusCode();
  response = client.responseBody();

  Serial.print("Status code: ");
  Serial.println(statusCode);
  Serial.print("Response: ");
  Serial.println(response);

}

void loop() {
  
  float data = read_sensors();
  publish(data);

  delay(2000);
}
