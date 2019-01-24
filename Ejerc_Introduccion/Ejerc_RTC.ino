int horas;
int minutos;
int segundos
void setup() {
  // put your setup code here, to run once:

}

void loop() {
  // put your main code here, to run repeatedly:
  if(segundos == 60){
    segundos = 0;
    minutos++
  }
  if(minutos == 60){
    minutos = 0;
    horas++;
  }
  segundos++;
  delay(1000);
}
