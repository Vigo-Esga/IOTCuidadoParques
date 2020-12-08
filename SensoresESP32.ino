#include <WiFi.h>
#include <time.h>
#include "DHT.h"
#include <ArduinoJson.h>
#include <HTTPClient.h>
#define DHTTYPE DHT11
char jsonOutput[128];
const char* ssid = "Vigod";
const char* password =  "ACSICNARF8";
uint8_t DHTPin = 5; 
DHT dht(DHTPin, DHTTYPE);  
float Temperatura;
float Humedad;
uint8_t sensor = 33;
uint8_t UVsensorIn = 32;
void setup() {
delay(10);
  Serial.begin(115200);
  pinMode(UVsensorIn, INPUT);
  pinMode(DHTPin, INPUT);
  dht.begin();  
  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
    delay(500);
    Serial.print(".");
  }

  Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());

  configTime(0, 0, "pool.ntp.org", "time.nist.gov");
  setenv("TZ", "EST5EDT,M3.2.0,M11.1.0", 1);
}

void loop() {
  int uvLevel = averageAnalogRead(UVsensorIn);
  float outputVoltage = 3.95 * uvLevel/4095;
  float uvIntensity = mapfloat(outputVoltage, 0.99, 2.9, 0.0, 15.0);
  Temperatura = dht.readTemperature(); 
  Humedad = dht.readHumidity();
  uint8_t v_sensor = analogRead(sensor);
  if(WiFi.status()== WL_CONNECTED){
  Serial.println("Conectado y listo para enviar a Web Services");
  Json_Datos("Humedad",Humedad,84);
  Json_Datos("Temperatura",Temperatura,84);
  Json_Datos("Ruido",v_sensor,84);
  Json_Datos("Radiacion",uvIntensity,84);
  Serial.println("La Temperatura es: ");
  Serial.print(Temperatura);
  Serial.println(" °");
  Serial.println("La Humedad es: ");
  Serial.print(Humedad);
  Serial.println(" %");
  Serial.println("El ruido es: ");
  Serial.println(v_sensor);
  Serial.print(" UV Intensidad: ");
  Serial.print(uvIntensity);
  Serial.print(" mW/cm^2"); 
  Serial.println(); 
  Serial.println("La fecha y hora es: ");
  Serial.println(get_time());
  }

  delay(2000);

}
void Json_Datos(String Nombre,float Sensor,int IdParque){

  HTTPClient client;
    client.begin("http://smartcityhyo.tk/api/Sensores/Insertar_Sensores.php");      
    client.addHeader("Content-Type", "application/json"); 
    const size_t CAPACITY = JSON_OBJECT_SIZE(4);
    StaticJsonDocument<CAPACITY> doc;
    JsonObject object = doc.to<JsonObject>();
    object["SE_Tipo"] = Nombre;
    object["ID_Parque"] = IdParque;
    object["SE_Nivel"] = Sensor;
    object["SE_Fecha"] = get_time();
    serializeJson(doc, jsonOutput);
    int httpCode = client.POST(String(jsonOutput));
    if(httpCode > 0){    
    String payload = client.getString();
    Serial.println("\nStatuscode: "+ String(httpCode));
    Serial.println(payload);
    }
      else{
     Serial.println("Error on HTTP request");
    }
}


String get_time(){
  time_t now;
  time(&now);
  char time_output[30];
  // See http://www.cplusplus.com/reference/ctime/strftime/ for strftime functions
  strftime(time_output, 30, "%Y-%m-%d %T", localtime(&now)); 
  return String(time_output); // returns Sat 20-Apr-19 12:31:45
}
//Takes an average of readings on a given pin
//Returns the average
int averageAnalogRead(int pinToRead)
{
  byte numberOfReadings = 8;
  unsigned int runningValue = 0; 
 
  for(int x = 0 ; x < numberOfReadings ; x++)
    runningValue += analogRead(pinToRead);
  runningValue /= numberOfReadings;
 
  return(runningValue);  
 
}
 
 
float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
{
  return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}
