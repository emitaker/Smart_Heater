#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

//Crear el objeto lcd  dirección  0x27 y 16 columnas x 2 filas
LiquidCrystal_I2C lcd(0x27,16,2);

void setup() {
  // Inicializar el LCD
  lcd.init();
  
  //Encender la luz de fondo.
  lcd.backlight();
  
  // Escribimos el Mensaje en el LCD en una posición  central.
  lcd.setCursor(0, 0);
  lcd.print("La temperatura");
  lcd.setCursor(0, 1);
  lcd.print("es de: ");
  lcd.setCursor(13,1);
  lcd.print((char)223);
  lcd.setCursor(14,1);
  lcd.print("C");
}

void loop() {
  //desplazamos una posición a la izquierda
  delay(500);
}
