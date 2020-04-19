#define ID 3
#define TERMPIN A0 //пин термистора
#define TERMRES 10000 //сопротивление при номинальной температуре
#define NOMTEMP 25 //номинальная температура
#define B 3380 //коэффициент для упрощенной формулы Стейнхарта-Харта 3693
#define RESIST 10000// сопротивление второго резистора
#define NUMSAMP 10 // количество усредняемых значений
#define BUZZPIN 3 // пищалка

int timer = 0;


float getTemp(){
  int samples[NUMSAMP];
  for (int i = 0; i < NUMSAMP; i++){
    samples[i] = analogRead(TERMPIN);
    delay(10);
  }

  int tVal = 0;// значение с термистора
  for (int i = 0; i < NUMSAMP; i++){
    tVal += samples[i];
  }
  tVal /= NUMSAMP;
  float tRes = RESIST/((1023.00/tVal) - 1);
  float steinhart = 1/(NOMTEMP+273.15) + 1.0/B * log(tRes/TERMRES);
  float temp = 1.0/steinhart;
  return(temp-273.15);
}
void setup() {
  Serial.begin(9600);
  timer = millis();
  pinMode(3, OUTPUT);
  analogReference(EXTERNAL);
}

void loop() {
  if(millis() - timer >= 14400000){
    timer = millis();
    Serial.println(getTemp());
  }
  if(Serial.available()){
    char buff = Serial.read();
    switch (buff){
      case 'T':
        Serial.println(getTemp());
        break;
      case 'R':
        Serial.println(ID);
        break;
    }
  }
  delay(1000);
}
