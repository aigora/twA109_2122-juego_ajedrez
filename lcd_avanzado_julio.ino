
#include <LiquidCrystal_I2C.h>


LiquidCrystal_I2C lcd(0x27,16,2);


#define TIEMPO_JUEGO 30
#define LED_JUGADOR_1 5
#define LED_JUGADOR_2 6
#define PULSADOR_JUGADOR_1 8
#define PULSADOR_JUGADOR_2 9


#define LED_GANADOR 3


// Estados juego
#define DETENIDO 0
#define TURNO_JUGADOR_1 1
#define TURNO_JUGADOR_2 2
#define MOSTRANDO_GANADOR 3 


bool estadoPulJug1Prev=false;
bool estadoPulJug2Prev=false;
bool estadoLedGanador=false;


unsigned long contadorJugador1=TIEMPO_JUEGO;
unsigned long contadorJugador2=TIEMPO_JUEGO;
unsigned long tiempoReferencia=0;
unsigned int tiempoDePartida=0;

int Tiempo_Mov_Jug1=0;
int Tiempo_Mov_Jug2=0;
int modo=DETENIDO;
int tiempoGanador=0;
int Tiempo_Base_Jug1=0;
int Tiempo_Base_Jug2=0;


void setup() {
  // put your setup code h

lcd.init();
lcd.backlight();
lcd.clear();

   lcd.setCursor(0,0);
  lcd.print("JUGADOR 1:");
  lcd.print(contadorJugador1);
  lcd.setCursor(0,1);
  lcd.print("JUGADOR 2:");
  lcd.print(contadorJugador2);

 pinMode(LED_JUGADOR_1, OUTPUT);
  pinMode(LED_JUGADOR_2, OUTPUT);
  pinMode(LED_GANADOR, OUTPUT);
  
  pinMode(PULSADOR_JUGADOR_1, INPUT_PULLUP);
  pinMode(PULSADOR_JUGADOR_2, INPUT_PULLUP);


tiempoReferencia=millis();
  
 
delay(1000);



}

void loop() {
  // put your main code here, to run repeatedly:

 // 1. LEEMOS Y PROCESAMOS LAS ENTRADAS
  bool estadoPulJug1 = !digitalRead(PULSADOR_JUGADOR_1);
  bool estadoPulJug2 = !digitalRead(PULSADOR_JUGADOR_2);
 
  if (estadoPulJug1 == true && estadoPulJug1Prev == false) {
    if(modo==DETENIDO || modo == TURNO_JUGADOR_1){
      modo=TURNO_JUGADOR_2;
        digitalWrite(LED_JUGADOR_2,true);
        digitalWrite(LED_JUGADOR_1,false);
    }
    
  }
  if (estadoPulJug2 == true && estadoPulJug2Prev == false) {
    if(modo==DETENIDO || modo == TURNO_JUGADOR_2){
      modo=TURNO_JUGADOR_1;
        digitalWrite(LED_JUGADOR_1,true);
        digitalWrite(LED_JUGADOR_2,false);      
    }
    
  }

  // 2. EJECUTAMOS LAS TAREAS PERIODICAS
  unsigned long tiempoActual = millis();
  if ((tiempoActual - tiempoReferencia) >= 1000) {
    tiempoReferencia = tiempoActual;
  //En este punto del progrma Arduino esta cada 1 segundo
    
    //De acuerdo al modo descontamos el tiempo del jugardor
    //correspondiente 
    if(modo == TURNO_JUGADOR_1){
        tiempoDePartida++;
     Tiempo_Base_Jug1++;
     lcd.setCursor(13,0);
     lcd.print(Tiempo_Base_Jug1);
  Serial.println(Tiempo_Base_Jug2);
      Tiempo_Base_Jug2=0;
      contadorJugador1--;
      // Tiempo_Mov_Jug1=Tiempo_Base_Jug1-contadorJugador1;

      if(contadorJugador1>9)
      {
        //Mostramos el tiempo restante
        lcd.setCursor(10,0);
        lcd.print(contadorJugador1);
      }
      else{

        lcd.setCursor(10,0);
         lcd.print("0");
         lcd.print(contadorJugador1);
        
      }
      
       //Verificamos si hay algun ganador?
        if(contadorJugador1==0){
          contadorJugador1=TIEMPO_JUEGO;
          contadorJugador2=TIEMPO_JUEGO;
          modo=MOSTRANDO_GANADOR;
          lcd.setCursor(10,1);
          lcd.print("GANO");
          lcd.setCursor(10,0);
          lcd.print("     ");

          Serial.print("Tiempo de partida:");
          Serial.println(tiempoDePartida);
          tiempoDePartida=0;
          digitalWrite(LED_GANADOR,true);
          tiempoGanador=15;
        }
   
    }
    
    if(modo == TURNO_JUGADOR_2)
    {
        tiempoDePartida++;
        contadorJugador2--;

      Tiempo_Base_Jug2++;
     lcd.setCursor(13,1);
     lcd.print(Tiempo_Base_Jug2);
   Serial.println(Tiempo_Base_Jug1);
       Tiempo_Base_Jug1=0;


        if(contadorJugador2>9)
        {
        //Mostramos el tiempo restante
          lcd.setCursor(10,1);
         lcd.print(contadorJugador2);
        }
        else{
          lcd.setCursor(10,1);
          lcd.print("0");
          lcd.print(contadorJugador2);
        }

        
        if(contadorJugador2==0){
          contadorJugador1=TIEMPO_JUEGO;
          contadorJugador2=TIEMPO_JUEGO;
          modo=MOSTRANDO_GANADOR;   
          lcd.setCursor(10,0);
          lcd.print("GANO");
          lcd.setCursor(10,1);
          lcd.print("     ");

          Serial.print("Tiempo de partida:");
          Serial.println(tiempoDePartida);
          tiempoDePartida=0;

          digitalWrite(LED_GANADOR,true);
          tiempoGanador=5;
        }

    }
    
    if(modo == MOSTRANDO_GANADOR){
      tiempoGanador--;
      
      estadoLedGanador=!estadoLedGanador;
      digitalWrite(LED_GANADOR,estadoLedGanador);
     
      //Si paso el tiempo para mostrar el ganador
      //pasamos a DETENIDO
      if(tiempoGanador==0){
         modo=DETENIDO;   
         digitalWrite(LED_GANADOR,false);
         digitalWrite(LED_JUGADOR_1,false);
         digitalWrite(LED_JUGADOR_2,false);
         
         
         lcd.clear();
         lcd.setCursor(0,0);
         lcd.print("JUGADOR 1:");
         lcd.print(contadorJugador1);
         lcd.setCursor(0,1);
         lcd.print("JUGADOR 2:");
         lcd.print(contadorJugador2);
      }    
    }

    
  }

  //actualizamos el valor de nuestras variables de estado previo
  estadoPulJug1Prev = estadoPulJug1;
  estadoPulJug2Prev = estadoPulJug2;

  delay(10); // para alivianar la carga del navegador
}



















































































     
