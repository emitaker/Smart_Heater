#include <LedControl.h>    // incluye libreria LedControl
#define DIN 51 // DIN de la matriz de leds 8x8
#define CLK 52 // CLK de la matriz de leds 8x8
#define CS  53 // CS de la matriz de leds 8x8

LedControl lc=LedControl(DIN, CLK, CS, 1); // crea objeto (DIN, CLK, CS)

byte carita_feliz[8]= {       // array que contiene todos los elementos de las
  B00111100,                 // filas necesarias por mostrar una carita feliz
  B01000010,
  B10100101,
  B10000001,
  B10100101,
  B10011001,
  B01000010,
  B00111100
};

byte cruz[8]= {       // array que contiene todos los elementos de las
  B10000001,          // filas necesarias por mostrar una cruz
  B01000010,
  B00100100,
  B00011000,
  B00011000,
  B00100100,
  B01000010,
  B10000001
};

byte parpadeo[8]= {       // array que contiene todos los elementos de las
  B01111110,          // filas necesarias por mostrar una cruz
  B10111101,
  B11011011,
  B11100111,
  B11100111,
  B11011011,
  B10111101,
  B01111110
};


void setup() {
  lc.shutdown(0,false);     // enciende la matriz
  lc.setIntensity(0,4);     // establece brillo
  lc.clearDisplay(0);     // blanquea matriz
}

void loop(){
  mostrar(carita_feliz);      // llama funcion mostrar
  delay(1000);              // demora
  mostrar(cruz);// llama funcion mostrar
  delay(200); 
  mostrar(parpadeo);
  delay(200); 
  mostrar(cruz);
  delay(200);              // demora
}

void mostrar (byte character []){
  int i = 0;
  for (i=0; i<8; i++){
    lc.setRow(0,i,character[i]);
  }
}
