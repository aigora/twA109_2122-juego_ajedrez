
int PULSADOR=3;
int LED1=2;
int LED2=4;
int LED3=5;
int estado1=LOW;
int estado2=LOW;
int estado3=LOW;
String mensaje_entrada;
String mensaje_salida;

void setup()
{
  Serial.begin(9600);
  
  pinMode(PULSADOR,INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2,OUTPUT);
  pinMode(LED3,OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
  digitalWrite(LED3,LOW);
}

void loop()  {
 

   if(Serial.available()>0)
   {
    mensaje_entrada=Serial.readStringUntil('\0');

  
   
      while(digitalRead(PULSADOR)==LOW);
     estado1 = digitalRead(LED1);
      digitalWrite(LED1, !estado1);
     estado2 = digitalRead(LED2);
     digitalWrite(LED2, !estado2);
     while(digitalRead(PULSADOR)==HIGH);
    
   }

 
  if(mensaje_entrada=="BLANCO")
  {
     digitalWrite(LED1,HIGH);
     digitalWrite(LED2,LOW);  
     digitalWrite(LED3,LOW);
  }
  
  else  if(mensaje_entrada=="NEGRO")
  {
     digitalWrite(LED1,LOW);
     digitalWrite(LED2,HIGH);  
     digitalWrite(LED3,LOW);
     
  }

  else if(mensaje_entrada=="FIN")
   {
      digitalWrite(LED1,LOW);
      digitalWrite(LED2,LOW);
      digitalWrite(LED3,HIGH);


   }

   

    delay(500);
  
}
