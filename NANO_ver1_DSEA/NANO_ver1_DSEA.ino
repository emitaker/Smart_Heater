//Santiago Ortiz Suzarte
//Daniel Fuentes Castro
//Emilio Campuzano Mejia

//Practica CAN
//Codigo para Arduino emisor

#include <SPI.h> //libreria SPI 
#include <mcp2515.h> //libreria para usar el modulo mcp2515

struct can_frame canMsg; //crea la trama de datos CAN (el mensaje que se va a enviar)

MCP2515 mcp2515(10); //pin donde esta conectado CS (Slave Select) 

const int triggerPin = 5; //pin trigger de sensor ultrasonico
const int echoPin = 4; //pin echo de sensor ultrasonico

int LED = 7;

unsigned long ultrasonicDelay = 10; //retraso de 10 microsegundos que utiliza sensor ultrasonico, de esta forma se evita usar delay()
unsigned long prev_us_delay = 0; //retraso anterior de sensor ultrasonico

float prev_distance = 0;
float current_distance = 0;

bool onStartUp = true; //indica si el sistema esta iniciando
bool first_meassure = true; //identifica si es la primera medicion que se realiza, sirve para 
float temp_distance; //variable temporal que almacena distancia que se acaba de medir

int us_stage = 1; //el funcionamiento del sensor ultrasonico se puede dividir en 2, cuando se envía un pulso en alto al pin de trigger o cuando se envía un pulso en bajo

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600); //útil para debuggeo
  pinMode(triggerPin, OUTPUT); //PIN TRIGGER como salida
  pinMode(echoPin, INPUT); //pin echo como entrada
  pinMode(LED,OUTPUT);
  SPI.begin(); //inicia comunicación SPI

  mcp2515.reset();
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ);
  mcp2515.setNormalMode();
  

}

void loop() {
  // put your main code here, to run repeatedly:
  
  unsigned long current_us_delay = micros(); //iniciar variable que se encarga de monitorear si han pasado 10 microsegundos de retraso

  if (onStartUp) { //¿Esta iniciando el sistema? esto es util para emitir pulso inicial en bajo del sensor
      digitalWrite(triggerPin, LOW); //se emite pulso inicial en bajo
      onStartUp = false; //ya ha iniciado el sistema, la variable se vuelve falsa
    } 

  
  
  if (current_us_delay - prev_us_delay >= ultrasonicDelay){ //¿han transurrido 10 microsegundos?
    if (us_stage == 1) { //emite pulso en alto y espera 10 microsegundos
          us_stage += 1; //us_stage = 2
          digitalWrite(triggerPin, HIGH);
          
      } else if (us_stage == 2) { //transcurrieron 10 microsegundos desde que se emitió pulso en alto
          us_stage -= 1; //us_stage = 1
          digitalWrite(triggerPin, LOW); //emite pulso en bajo
          //calcula distancia
          if (first_meassure){ //¿es la primera medicion que realiza el sensor?
              current_distance = getDistance(); //llama funcion para medir distancia de sensor ultrasonico
              prev_distance = current_distance; //prev distance ayuda a solo tomar medidas cuando sea necesario, cuando la diferencia entre medidas sea muy significativa
              first_meassure = false;  //ya se tomo la primera medida de dstancia
            
         }else {
              temp_distance = getDistance(); //no va a tomar otra medida de distancia si no hay variacion de al menos 1 cm entre la distancia que se acaba de medir y la anterior
              if (abs(temp_distance - prev_distance) >= 1.0){ //current_distance solo cambia si hay una variacion de 1 cm en medidas de sensor, puede cambiarse la sensibilidad
                    current_distance = temp_distance;                    
                    prev_distance = current_distance; 
                    Serial.println("Cambio"); //util para debuggeo
                     
                }  
            }
          
      }
    
    }

   if (current_distance >= 8.0) { //la distancia medida por el sensor es mayor a 8cm  
                        Serial.print("Distancia: ");
                        Serial.print(current_distance);
                        Serial.println(" Todo bien"); //debug, indica que no hay ninguna colision
                        digitalWrite(LED,LOW);
                    } else if (current_distance < 3.0){ //Hubo una colision (valor de 3cm medido por sensor se interpreta como colision
                        Serial.print("Menor: ");
                        Serial.print(current_distance);
                        Serial.println(" Enviado");
                        //float code = -1;
                        canMsg.can_id = 0x036; //id de mensaje CAN
                        canMsg.can_dlc = 2; //Longitud de mensaje CAN
                        canMsg.data[0] = current_distance;
                        canMsg.data[1] = 0x10; //0x10 = "colision" 
                        mcp2515.sendMessage(&canMsg); //envia el mensaje
                        digitalWrite(LED,HIGH); //enciende LED que advierte de colision
                    }

                        
}



float getDistance(){//calcula distancia que ha medido el sensor ultrasonico
      float duration;
      float distance;

      duration = pulseIn(echoPin, HIGH); 
      distance = duration * 0.034/2;

      return distance;
          
  
  
  }
