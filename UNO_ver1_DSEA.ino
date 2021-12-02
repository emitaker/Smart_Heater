//A01750402
//Santiago Ortiz Suzarte
//ECU Arduino UNO ver. 1
//monitorea temperatura usando LM35 y enciende un ventilador si el valor excede un limite o lo apaga si es inferior a un limite
//Cuenta con display LCD para desplegar temperatura
//Modulo bluetooth HC-05 para enviar informacion a Arduino MEGA 

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <queue.h>
//#include <Wire.h>
//#include <LiquidCrystal.h> //libreria para configurar LCD con solo 2 pines
#include <SPI.h> //libreria SPI 
#include <mcp2515.h> //libreria para usar el modulo mcp2515

struct can_frame canMsg; //crea la trama de datos CAN (el mensaje que se va a enviar)

MCP2515 mcp2515(10); //pin donde esta conectado CS (Slave Select) 
//LiquidCrystal lcd(7,8,9,10,11,12); //crea objeto LCD 
               //rs,e,d4,d5,d6,d7
//QueueHandle_t queue_1; //queue que envía datos de sensor de temperatura LM35 a LCD
QueueHandle_t queue_2; //queue que envía datos de sensor de temperatura LM35 a tarea TaskFAN para determinar si se debe encender el ventilador
QueueHandle_t queue_3; //queue que envia datos de sensor de temperatura


//LiquidCrystal_I2C lcd(0x27,16,2); //crear objeto LiquidCrystal_I2C para controlar LCD
float inferior_limit = 10.0; //limite inferior a partir del cual se apaga ventilador
float superior_limit = 20.0; //limite superior a partir del cual se enciende ventilador
bool on_off = false; //indica si ventilador esta encendido o apagado




void setup() {
  // put your setup code here, to run once:
  SPI.begin(); //inicia comunicacion SPI
  mcp2515.reset(); //resetea mcp2515
  mcp2515.setBitrate(CAN_125KBPS,MCP_8MHZ); //establece bitrate
  mcp2515.setNormalMode(); //mcp2515 modo normal
//  queue_1 = xQueueCreate(5, sizeof(float)); //crea queue 1 con tamaño para 5 objetos de tipo float //OJO si no es usa y no se elimina se llena memoria y programa ni funciona
  //if (queue_1 == NULL) {
   //   Serial.println("No se pudo crear queue 1"); //Serial va a ser utilizada por bluetooth
  //  }

  queue_2 = xQueueCreate(5, sizeof(float));

  if (queue_2 == NULL) {
      Serial.println("No se pudo crear queue 2");
    }

  queue_3 = xQueueCreate(5, sizeof(float));

  if (queue_3 == NULL) {
      Serial.println("No se pudo crear queue 2");
    }
  Serial.begin(9600); //inciia comunicacion serial con terminal
  //lcd.init();//inicia display LCD
  //lcd.backlight(); //Activa backlight de LCD
  xTaskCreate(TaskReadTemp,"Leer temperatura",128, NULL, 1, NULL); //crear task que lee temperatura de LM35
  //xTaskCreate(TaskLCD,"Display LCD",128, NULL, 2, NULL); //task que muestra temperatura en display LCD
  xTaskCreate(TaskFAN,"Enciende ventilador",128, NULL, 1, NULL); //enciende ventilador si temperatura supera cierto valor
  xTaskCreate(TaskSendBT, "Manda datos por bluetooth",128,NULL,1,NULL); //manda datos de temperatura por bluetooth a Arduino MEGA
  xTaskCreate(TaskBUSCAN, "recibe mensajes de CAN BUS",128,NULL,1,NULL);
  vTaskStartScheduler();

}

void loop() {
  // put your main code here, to run repeatedly:

}



void TaskReadTemp (void * pvParameters) {
    int value;
    float current_temp;

    while(1){
        //Serial.println("Temp LM35"); //debug
        value = analogRead(A0); //lee valor enviado por sensor
        current_temp = (5.0*value*100.0)/1024.0;//(value/ 1023.0) * 5000; //temperatura actual en Millivolts
        //current_temp = current_temp/10; //pasa el valor a grados Celcius
        //Serial.println(current_temp );
        //lcd.clear();
        //lcd.setCursor(0,0);
        //lcd.print("sent to queue");
        //xQueueSend(queue_1, &current_temp, portMAX_DELAY); //envia informacion de sensor LM35 A queue_1
        xQueueSend(queue_2, &current_temp, portMAX_DELAY); //envia informacion de sensor LM35 a queue_2
        xQueueSend(queue_3, &current_temp, portMAX_DELAY); //envia informacion de sensor LM35 a queue_3
        
        
        vTaskDelay(1000/ portTICK_PERIOD_MS);
      }
  }

//void TaskLCD (void * pvParameters){ //
  //  float temp = 0;
   // while(1){
        //Serial.println("Task LCD");
     //   if (xQueueReceive(queue_1, &temp, portMAX_DELAY) == pdPASS) {
            //Serial.println("Mostrando en LCD");
            /*lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("Temp: ");
            lcd.setCursor(8,0);
            lcd.print(temp); */
            //Serial.println("llego aqui");

         //   if (on_off){
                //Serial.println("si llegamos?");
                /*lcd.setCursor(0,1);
                lcd.print("Fan state: ");
                lcd.setCursor(12,1);
                lcd.print("ON");*/
              
        //    } else {
                //Serial.println("si llego hasta aqui?");
               /* lcd.setCursor(0,1);
                lcd.print("Fan state: ");
                lcd.setCursor(12,1);
                lcd.print("OFF");*/
         //     } 
       //   }
    //  }
 // }


void TaskFAN ( void * pvParameters) {//enciende o apaga ventilador de acuerdo a la temperatura que mida el sensor LM35 
  float temp = 0;
  while(1){
      //Serial.println("ventilador");
      if (xQueueReceive(queue_2, &temp, portMAX_DELAY) == pdPASS) {
          //Serial.println(temp); 
          if (temp <= inferior_limit) { //revisa la temperatura es menor o igual a limite inferior para apagar ventilador
              //Serial.println("Apagando ventilador");
              on_off = LOW; //apaga ventilador
              
            }
          if (temp >= superior_limit){
              //Serial.println("Encendiendo ventilador"); 
              on_off = HIGH; //enciende ventilador
             
            
            }
          digitalWrite(4, on_off);
          
          
        
        }
    }
  }

void TaskSendBT (void * pvParameters) {//envia datos por bluetooth a Arduino MEGA 
        float temp; //floats se almacenan en 4 bytes
        //byte *byte_pointer = (byte*)&temp; //type casting para que byte_pointer sea un apuntador a un byte
        while(1){
            if (xQueueReceive(queue_3, &temp, portMAX_DELAY) == pdPASS) {
                // byte *byte_pointer = (byte*)&temp; //apuntador de tipo byte a variable tipo float temp (los float se guardan en 4 bytes)
                 //Serial.println(temp);
                 //Serial.print(*byte_pointer,HEX); //primer byte de temp
                 //Serial.write(*byte_pointer);
                  Serial.write((int)temp); //prototipo envia int 
                 //Serial.print((int)(temp));
                 //byte_pointer = byte_pointer+1;
                 //Serial.print(*byte_pointer,HEX); //segundo byte de temp 
                 //Serial.write(*byte_pointer);
                 //byte_pointer = byte_pointer+1;
                 //Serial.print(*byte_pointer,HEX); //tercer byte de temp 
                 //Serial.write(*byte_pointer);
                 //byte_pointer = byte_pointer+1;
                 //Serial.println(*byte_pointer,HEX);//cuarto byte de temp
                 //Serial.write(*byte_pointer);
                 //digitalWrite(7,HIGH); //LED indicador
                 
              
              }
          
            vTaskDelay(1000/ portTICK_PERIOD_MS);
          
          }  
  }

  void TaskBUSCAN (void * pvParameters) { //revisa si se han recibido datos del arduino NANO por comunicación por BUS CAN
        int coll_code = 254; //si obtuvieramos una medida asi del LM35 estariamos muertos
        while(1){
            
            if (mcp2515.readMessage(&canMsg) == MCP2515::ERROR_OK){ //¿se recibió un mensaje?
                 //float msg = canMsg.data[0];
                 //int code = canMsg.data[1];
                  //Serial.println("funciono "); //debug
                  //Serial.println(msg);//debug
                  Serial.write(coll_code); //indica que hubo una colision
                  digitalWrite(6,HIGH); //LED indicador
          
          } else {
                  digitalWrite(6,LOW);
            
            }

          vTaskDelay(100/portTICK_PERIOD_MS);
    
    }
  }
