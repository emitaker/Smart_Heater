#define PULSADOR 2
#define BUZZER_ACTIVO 8

float sinVal; //Variable que almacena la amplitud de la señal sinusuidal
int toneVal; //Variable que almacena la frecuencia del tono que se genera
boolean stateAlarm; //Variable de tipo bool que indica si la alarma está encendida o apagada

void setup (){
  pinMode (PULSADOR, INPUT_PULLUP);
  pinMode (BUZZER_ACTIVO, OUTPUT);
}

void loop (){
  if (digitalRead(PULSADOR) == LOW){
    stateAlarm =! stateAlarm;
    delay(300);
  }

  if(stateAlarm == 1){
    for (int i=0; i<180; i++){

      //Convertir grados a radianes para obtener el valor.
      sinVal = (sin(i*(3.1412/180)));
      //Generamos una frecuecia del valor de sin
      toneVal = 2000 + int((sinVal*1000));
      tone(BUZZER_ACTIVO, toneVal);
      delay(2);

        if(digitalRead(PULSADOR) == LOW){
          stateAlarm =! stateAlarm;
          delay(300);
        }
    }
  }

  else{
    noTone(BUZZER_ACTIVO);
  }
}
