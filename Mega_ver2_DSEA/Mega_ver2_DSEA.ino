//Emilio Campuzano Mejia
//Daniel Fuentes Castro
//Santiago Ortiz Suzarte
//Arduino MEGA 2560 ver. 2
//monitorea niveles de combustible y se encarga de recibir datos por bluetooth y CAN BUS para enviarlos a base de datos

#include <Arduino_FreeRTOS.h>
#include <semphr.h>
#include <LiquidCrystal.h>
#include <queue.h>


QueueHandle_t queue_1; //queue que almacena datos de sensor que mide nivel de agua
int LEDPins[10] = {34,36,38,40,42,44,46,48,33,35}; //Pines de la barra de LEDs 
LiquidCrystal lcd(7,8,9,10,11,12); //crea objeto LCD 
int inferior_limit = 10;
int superior_limit = 20;
 

void setup() {
  // put your setup code here, to run once:
  for (int i = 0; i < 10; i++){ //Configura barra de LEDs
      pinMode(LEDPins[i], OUTPUT);
    
    }
  
  Serial.begin(9600); //inciia comunicacion serial con PC
  Serial1.begin(9600); //comunicación con hc-05
  lcd.begin(16,2); //configura LCD para funcionar con 16 columnas y 2 filas
  queue_1 = xQueueCreate(5, sizeof(float)); //crea queue 1 con tamaño para 5 objetos de tipo float
  if (queue_1 == NULL) {
      Serial.println("No se pudo crear queue 1");
    }
  xTaskCreate(TaskGasLevel,"Nivel de combustible",128, NULL, 1, NULL); //revisa el nivel de combustible del vehiculo
  xTaskCreate(TaskLEDBar, "Grafica de LED",128,NULL,1,NULL); //usa LED Bar Graph para mostrar cuanto combustible hay disponible
  xTaskCreate(TaskBluetooth, "Recibir datos de bluetooth", 128, NULL, 2, NULL); //revisa si se ha mandado info de bluetooth de arduino UNO
  
  vTaskStartScheduler(); //inicia scheduler
  
  

}

void loop() {
  // put your main code here, to run repeatedly:

}


void TaskGasLevel (void * pvParameters) { //revisa el nivel de combustible del vehiulo
    int gas_level = 0; //nivel de combustible
    while(1){
        //digitalWrite(13,HIGH); //activa sensor
        //vTaskDelay(10/portTICK_PERIOD_MS); //espera 1 segundo
        gas_level = analogRead(A1); //lee señal que manda sensor
        //digitalWrite(13,LOW); //apaga sensor
        //sensor de gasolina va a permanecer encendido (+ conectado a 5V para obtener buenas mediiones)
        if (gas_level < 10){//si valor de combustile es mayor o igual a 10 significa que le tanque esta lleno
            gas_level = 0; 
          
        } else if (gas_level > 0 && gas_level < 100){
             gas_level = 0;
          
        } else if(gas_level >= 100 && gas_level < 200){
              gas_level = 1;
          
        } else if (gas_level >= 200 && gas_level < 300) {
              gas_level = 2;
           
        } else if (gas_level >= 300 && gas_level < 400){
              gas_level = 3;
              
        } else if (gas_level >= 400 && gas_level < 500){
              gas_level = 4;
          
        } else if (gas_level >= 500 && gas_level < 600){
              gas_level = 5;
        } else {
              gas_level = 5;  
        } 
        Serial.print("Nivel de combustible: ");
        Serial.println(gas_level); //muestra el nivel de combustible en el vehiculo
        xQueueSend(queue_1, &gas_level, portMAX_DELAY); //envia informacion de sensor LM35 A queue_1
        vTaskDelay(1000/portTICK_PERIOD_MS); //espera 1 segundo
      }
  }

void TaskLEDBar (void * pvParameters){//se encarga de los indicadores de combustible
    int level = 0; //nivel de combustible
    while(1){
        Serial.println("Encendiendo barra de LEDs");//debug
        if (xQueueReceive(queue_1,&level,portMAX_DELAY) ==pdPASS){
            switch (level) {
                case 0:
                    Serial.println("Precaución Combustible bajo");
                    lcd.clear();
                    lcd.setCursor(0,0);
                    lcd.print("SIN COMBUSTIBLE");
                    digitalWrite(22, HIGH); //led que indica que hay poco combustible
                    digitalWrite(24,HIGH); //enciende alarma or combustible agotado
                    digitalWrite(LEDPins[0],LOW);
                    digitalWrite(LEDPins[1],LOW);
                    digitalWrite(LEDPins[2],LOW);
                    digitalWrite(LEDPins[3],LOW);
                    digitalWrite(LEDPins[4],LOW);
                    digitalWrite(LEDPins[5],LOW);
                    digitalWrite(LEDPins[6],LOW);
                    digitalWrite(LEDPins[7],LOW);
                    digitalWrite(LEDPins[8],LOW);
                    digitalWrite(LEDPins[9],LOW);
                    break;
                case 1:
                    Serial.println("level = 1");
                    digitalWrite(22, LOW); //led que indica que hay poco combustible
                    digitalWrite(24,LOW); //enciende alarma or combustible agotado
                    digitalWrite(LEDPins[0],HIGH);
                    digitalWrite(LEDPins[1],HIGH);
                    digitalWrite(LEDPins[2],LOW);
                    digitalWrite(LEDPins[3],LOW);
                    digitalWrite(LEDPins[4],LOW);
                    digitalWrite(LEDPins[5],LOW);
                    digitalWrite(LEDPins[6],LOW);
                    digitalWrite(LEDPins[7],LOW);
                    digitalWrite(LEDPins[8],LOW);
                    digitalWrite(LEDPins[9],LOW);
                    break;
                    
                 case 2:
                    Serial.println("level = 2");
                    digitalWrite(22, LOW); //led que indica que hay poco combustible
                    digitalWrite(24,LOW); //enciende alarma or combustible agotado
                    digitalWrite(LEDPins[0],HIGH);
                    digitalWrite(LEDPins[1],HIGH);
                    digitalWrite(LEDPins[2],HIGH);
                    digitalWrite(LEDPins[3],HIGH);
                    digitalWrite(LEDPins[4],LOW);
                    digitalWrite(LEDPins[5],LOW);
                    digitalWrite(LEDPins[6],LOW);
                    digitalWrite(LEDPins[7],LOW);
                    digitalWrite(LEDPins[8],LOW);
                    digitalWrite(LEDPins[9],LOW);
                    break;
                    
                 case 3:
                    Serial.println("level = 3");
                    digitalWrite(22, LOW); //led que indica que hay poco combustible
                    digitalWrite(24,LOW); //enciende alarma or combustible agotado
                    digitalWrite(LEDPins[0],HIGH);
                    digitalWrite(LEDPins[1],HIGH);
                    digitalWrite(LEDPins[2],HIGH);
                    digitalWrite(LEDPins[3],HIGH);
                    digitalWrite(LEDPins[4],HIGH);
                    digitalWrite(LEDPins[5],HIGH);
                    digitalWrite(LEDPins[6],LOW);
                    digitalWrite(LEDPins[7],LOW);
                    digitalWrite(LEDPins[8],LOW);
                    digitalWrite(LEDPins[9],LOW);
                    break;
                    
                 case 4:
                    Serial.println("level = 4");
                    digitalWrite(22, LOW); //led que indica que hay poco combustible
                    digitalWrite(24,LOW); //enciende alarma or combustible agotado
                    digitalWrite(LEDPins[0],HIGH);
                    digitalWrite(LEDPins[1],HIGH);
                    digitalWrite(LEDPins[2],HIGH);
                    digitalWrite(LEDPins[3],HIGH);
                    digitalWrite(LEDPins[4],HIGH);
                    digitalWrite(LEDPins[5],HIGH);
                    digitalWrite(LEDPins[6],HIGH);
                    digitalWrite(LEDPins[7],HIGH);
                    digitalWrite(LEDPins[8],LOW);
                    digitalWrite(LEDPins[9],LOW);
                    break;
                    
                 case 5:
                    Serial.print("level = 5");
                    Serial.println("Tanque lleno");
                    digitalWrite(22, LOW); //led que indica que hay poco combustible
                    digitalWrite(24,LOW); //enciende alarma or combustible agotado
                    digitalWrite(LEDPins[0],HIGH);
                    digitalWrite(LEDPins[1],HIGH);
                    digitalWrite(LEDPins[2],HIGH);
                    digitalWrite(LEDPins[3],HIGH);
                    digitalWrite(LEDPins[4],HIGH);
                    digitalWrite(LEDPins[5],HIGH);
                    digitalWrite(LEDPins[6],HIGH);
                    digitalWrite(LEDPins[7],HIGH);
                    digitalWrite(LEDPins[8],HIGH);
                    digitalWrite(LEDPins[9],HIGH);
                    break; 
              }    
          }
      }
  }

void TaskBluetooth (void * pvParameters) { //revisa si se han recibido datos de temperatura por bluetooth 
         //float bt_data = 0;
         //byte *byte_pointer = (byte*)&bt_data; //type casting para apuntador de float a byte
         int temp_bt_data = 0; //por ahora para facilitar comunicacion el dato va a ser de tipo int
         Serial.println("Revisando si se han recbido datos por bt...");
         
         while(1){
              if (Serial1.available()>0){ //se han recibido datos?
                  Serial.println("recibido");
                  //byte_pointer = Serial1.read();  //asigna valor que se envio a variable
                  //Serial.readBytes(byte_pointer,4);
                  
                  temp_bt_data = Serial1.read();
                  if (temp_bt_data == 254){
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Colision!!!");
                      Serial.flush();
                      vTaskDelay(500/portTICK_PERIOD_MS);
                    
                    
                    
                  } else{
                      Serial.print("Se han recibido datos por bt!: "); //debug
                      /*Serial.print(*byte_pointer,HEX); //debug
                      byte_pointer = byte_pointer+1;
                      Serial.print(*byte_pointer,HEX); //debug
                      byte_pointer = byte_pointer+1;
                      Serial.print(*byte_pointer,HEX); //debug
                      byte_pointer = byte_pointer+1;
                      Serial.println(*byte_pointer,HEX); //debug
                      */
                      Serial.print(temp_bt_data);
                      digitalWrite(26,HIGH); //LED que indica que se recibieron datos
                      lcd.clear();
                      lcd.setCursor(0,0);
                      lcd.print("Temp: ");
                      lcd.setCursor(8,0);
                      lcd.print(temp_bt_data);

                    if (temp_bt_data <= inferior_limit) { //revisa la temperatura es menor o igual a limite inferior para apagar ventilador
                      //Serial.println("Apagando ventilador");
                       lcd.setCursor(0,1);
                       lcd.print("Fan status: ");
                       lcd.setCursor(12,1);
                       lcd.print("OFF");
              
                    }
                    if (temp_bt_data >= superior_limit){
                    //Serial.println("Encendiendo ventilador"); 
                        lcd.setCursor(0,1);
                        lcd.print("Fan state: ");
                        lcd.setCursor(12,1);
                        lcd.print("ON");
                
                    }
                  } 
              }
                    
                   

               
          vTaskDelay(500/portTICK_PERIOD_MS);  

  }
}
