// TE2004B Diseño de sistemas emebebidos avanzados
// Código del funcionamiento del sistema Smart_Heater
// Profesores:
// Luis Humberto Paniagua Sabines
// Ricardo Acevedo Ávila
// Alumnos:
// Emilio Campuzano Mejía -  A01378948
// Santiago Ortiz Suzarte -  A01750402
// Daniel Fuentes Castro  -  A01750425


//-----Declaración de pines y librerías a utilizar -----------------------------------------//
#include <Wire.h> //Libreria para comunicarse con dispositivos por bus I2C 
#include <LiquidCrystal_I2C.h> //Library of the LCD with I2C
#include <LedControl.h>    //Librería para controlar la matriz de leds 8x8
#define BUZZER_ACTIVO 49 //Buzzer Activo
#define TEMP 0 //Sensor de Temperatura LM35 conectado a A0
#define DIN 51 // DIN de la matriz de leds 8x8
#define CLK 52 // CLK de la matriz de leds 8x8
#define CS  53 // CS de la matriz de leds 8x8

//-----Declaración de variable globales y creación de objetos -------------------------------//

//Se crea el objeto lcd  dirección  0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2); //Puede ser la dirección 0x27 ó 0x3F

//Se crea el objeto de la matriz de leds 8x8
LedControl lc=LedControl(DIN,CLK,CS,1); //(DIN, CLK, CS, No.)

//------ Arrays para las figuras de la matriz de leds 8x8 -----------------------------------//
byte carita_feliz[8]= { // array que contiene todos los elementos de las
  B00111100,            // filas necesarias por mostrar una carita feliz
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};

byte cruz[8]= { // array que contiene todos los elementos de las
  B10000001,    // filas necesarias por mostrar una cruz
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001
};

byte parpadeo[8]= { // array que contiene todos los elementos de las
  B01111110,        // filas necesarias por mostrar un parpadeo
  B10111101,
  B11011011,
  B11100111,
  B11100111,
  B11011011,
  B10111101,
  B01111110
};

//------ Variables del LM35 -----------------------------------------------------------------//
float value; // Variable para almacenar el valor obtenido del LM35 (0 a 1023) 
float tempC; // Variable para almacenar el valor de la temperatura en centígrados

//------ Variables del Buzzer Activo --------------------------------------------------------//
float sinVal; //Variable que almacena la amplitud de la señal sinusuidal
int toneVal; //Variable que almacena la frecuencia del tono que se genera

void setup() {
  // Configuramos el puerto serial a 9600 bps
  Serial.begin(9600);

  // Inicializamos el LCD
  lcd.init();
  
  //Encendemos la luz de fondo del LCD
  lcd.backlight();

  //Configuración de la matriz de leds 8x8
  lc.shutdown(0,false);     // enciende la matriz
  lc.setIntensity(0,4);     // establece brillo
  lc.clearDisplay(0);     // blanquea matriz

  //Configuramos el PinMode
  pinMode(BUZZER_ACTIVO, OUTPUT);
}

void loop() {
//-------- Instrucciones del LM35 -----------------------------------------------------------//

  // Con analogRead leemos el sensor, es importante recordar que es un valor de 0 a 1023
  value = analogRead(TEMP); 
   
  // Calculamos la temperatura con la fórmula
  // Voltaje=(Valor analogread * 5V)/1023
  // Valor de temperatura = Voltaje * 100
  tempC = (5.0 * value * 100.0)/1023.0; 
 
  // Envia el dato al puerto serial
  Serial.print(tempC);
  Serial.print("°C");
  // Salto de línea
  Serial.print("\n");
  
  // Esperamos un tiempo para repetir el loop
  delay(1000);

//-------- Instrucciones del LCD con I2C ------------------------------------------------------//

  //Colocamos el cursor en la primera posición de la primera fila
  lcd.setCursor(0, 0);
  lcd.print("La temperatura");
  //Colocamos el cursor en la primera posición de la segunda fila
  lcd.setCursor(0, 1);
  lcd.print("es de: ");
  //Colocamos el cursor en la 7° posición de la segunda fila
  //Aquí se manda el dato de la temperatura del LM35 al LCD
  lcd.setCursor(7,1);
  lcd.print(tempC,2);// 2 decimales
  //Colocamos el cursor en la 13° posición de la segunda fila
  lcd.setCursor(12,1);
  lcd.print((char)223);// (char)223 --> "°"
  //Colocamos el cursor en la 14° posición de la segunda fila
  lcd.setCursor(13,1);
  lcd.print("C");

  delay(200);

//-------- Instrucciones del Buzzer Activo ----------------------------------------------------//
  if(tempC >= 25){
    for (int i=0; i<180; i++){

      //Convertir grados a radianes para obtener el valor.
      sinVal = (sin(i*(3.1412/180)));
      //Generamos una frecuecia del valor de sin
      toneVal = 2000 + int((sinVal*1000));
      tone(BUZZER_ACTIVO, toneVal);
      delay(10);
    }
    //Mostrar la figura de la cruz y parpadeo
    mostrar(cruz);// llama funcion mostrar
    delay(200); 
    mostrar(parpadeo);// llama funcion mostrar
    delay(200); 
    mostrar(cruz);// llama funcion mostrar
    delay(200);
  }
  else{
    noTone(BUZZER_ACTIVO);
    //Mostrar la figura de la carita feliz
    mostrar(carita_feliz);  //llama funcion mostrar
    delay(10);            
  }  
}

//---- Creación de la función para mostrar las figuras de la matriz de leds 8x8 ------------//
void mostrar (byte character []){
  int i = 0;
  for (i=0; i<8; i++){
    lc.setRow(0,i,character[i]);
  }
}
