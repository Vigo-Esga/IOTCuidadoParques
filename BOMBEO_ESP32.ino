int relay = 15;
void setup() {
  // put your setup code here, to run once:
  pinMode(relay, OUTPUT); // Configurar relay como salida o OUTPUT
  //Serial.begin(9600); // Abrir el puerto serie a la velocidad de 9600bps para trasnmicion de datos.
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  int lectura = analogRead(4);

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
  // int lecturaPorcentaje = map(lectura, 1023, 0, 0, 100);

  // Serial.print("La Humedad es del: ");
   //Serial.print(lecturaPorcentaje);
   //Serial.println("%");
}
