// Declaracion de variables globales
float tempC; // Variable para almacenar el valor obtenido del sensor (0 a 1023)
float value;
int pinLM35 = 0; // Variable del pin de entrada del sensor (A0)
 
void setup() {
  // Configuramos el puerto serial a 9600 bps
  Serial.begin(9600);
 
}
 
void loop() {
  // Con analogRead leemos el sensor, es importante recordar que es un valor de 0 a 1023
  value = analogRead(pinLM35); 
   
  // Calculamos la temperatura con la fórmula
  tempC = (5.0 * value * 100.0)/1023.0; 
 
  // Envia el dato al puerto serial
  Serial.print(tempC);
  Serial.print("°C");
  // Salto de línea
  Serial.print("\n");
  
  // Esperamos un tiempo para repetir el loop
  delay(1000);
}195.5
