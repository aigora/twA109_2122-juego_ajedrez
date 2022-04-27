

int PULSADOR=3;
int LED1=2;
int LED2=4;
int estado1=LOW;
int estado2=LOW;



void setup()
{
  Serial.begin(9600);
  pinMode(PULSADOR,INPUT);
  pinMode(LED1, OUTPUT);
  pinMode(LED2,OUTPUT);
  digitalWrite(LED1, LOW);
  digitalWrite(LED2, LOW);
}

void loop()  {
 
  while(digitalRead(PULSADOR)==LOW);
  estado1 = digitalRead(LED1);
  digitalWrite(LED1, !estado1);
  estado2 = digitalRead(LED2);
  digitalWrite(LED2, !estado2);
  while(digitalRead(PULSADOR)==HIGH);
}














  




  
