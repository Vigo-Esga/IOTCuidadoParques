#include <ArduinoJson.h>
#include <HTTPClient.h>
#include <WiFi.h>
const char* ssid = "Vigod";
const char* password =  "ACSICNARF8";
char jsonOutput[128]; 
int relay = 15;
void setup() {

  Serial.begin(115200);
  pinMode(relay, OUTPUT);
  WiFi.begin(ssid, password);
  Serial.print("Conectando...");
  while (WiFi.status() != WL_CONNECTED) { //Check for the connection
        Serial.print(".");
    delay(500);
  }
    Serial.print("Conectado con éxito, mi IP es: ");
  Serial.println(WiFi.localIP());
}

void loop() {
 if(WiFi.status()== WL_CONNECTED){
 // long rnd = random(54, 82);
  HTTPClient client;
  client.begin("http://smartcityhyo.tk/api/Activar/Buscar_Act.php?Act_id=1");  
  int httpCode = client.GET();
  if(httpCode > 0){ 
    String payload = client.getString();
    Serial.println("\nStatuscode: "+ String(httpCode));
    Serial.println(payload);
    char json[500];
    payload.toCharArray(json, 500);
    StaticJsonDocument<200> doc;
    deserializeJson(doc, json);
    int idp = doc["Act_Valor"];
    Serial.println(String(idp));
        if(idp == 1){
      Serial.println("Riego Activado");
      digitalWrite(relay, LOW);
      }
    else if(idp == 2){
      Serial.println("Riego Desactivado");
      digitalWrite(relay, HIGH);
      }
      else if(idp == 3){
    Riego();
      }else{
      Serial.println("No es el parque correcto");
      }
    client.end();

  }
 }
 else
 {
  Serial.println("Conexion perdida");
 }
 delay(500);
 }
void Riego(){
// put your main code here, to run repeatedly:
  int lectura = analogRead(33);

  Serial.print("La lectura es: ");
  Serial.println(lectura);

  // lecturas entre 1000 - 1023
  if (lectura >= 4095){
    Serial.println(">> EL SENSOR ESTA DECONECTADO O FUERA DEL SUELO <<");
    digitalWrite(relay, HIGH);  
    Serial.println("Relay no accionado");  
    } 
   else if (lectura <4095 && lectura >= 2400){
    Serial.println(">> EL SUELO ESTA SECO <<");
    digitalWrite(relay, LOW); // envia señal alta al relay
    Serial.println("Relay accionado");
   }
   else if (lectura < 2400 && lectura >= 1500){
    Serial.println(">> EL SUELO ESTA HUMEDO <<");
    digitalWrite(relay, HIGH);  
    Serial.println("Relay no accionado");
   }
   else if (lectura < 1500){
    Serial.println(">> EL SENSOR ESTA PRACTICAMENTE EN AGUA <<");
    digitalWrite(relay, HIGH);  
    Serial.println("Relay no accionado");
   }
   delay(1000);

   //Convirtiendo a Porcentaje
   int lecturaPorcentaje = map(lectura, 4095, 0, 0, 100);

   Serial.print("La Humedad es del: ");
   Serial.print(lecturaPorcentaje);
   Serial.println("%");
}
