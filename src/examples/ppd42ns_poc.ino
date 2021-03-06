/*
 Shinyei PPD42NS Particle Sensor

 using nodemcu-LoLin board

 specification: http://www.sca-shinyei.com/pdf/PPD42NS.pdf

 pin 1 (grey)  -> GND
 pin 3 (black) -> Vin
 pin 2 (green) -> Pin D5 / GPIO14
 pin 4 (white) -> Pin D6 / GPIO12
 pin 5 (red)   -> unused!

*/

int pin1 = 5;
int pin2 = 6;

boolean valP1 = HIGH;
boolean valP2 = HIGH;

unsigned long starttime;
unsigned long durationP1;
unsigned long durationP2;

boolean trigP1 = false;
boolean trigP2 = false;
unsigned long trigOnP1;
unsigned long trigOnP2;

unsigned long sampletime_ms = 15000;
unsigned long lowpulseoccupancyP1 = 0;
unsigned long lowpulseoccupancyP2 = 0;

void setup() {
  Serial.begin(9600);
  Serial.println("start PPD42NS");
  pinMode(pin1,INPUT);
  pinMode(pin2,INPUT);
  starttime = millis();
}

void loop() {
  float ratio = 0;
  float concentration = 0;

  valP1 = digitalRead(pin1);
  valP2 = digitalRead(pin2);

  if(valP1 == LOW && trigP1 == false){
    trigP1 = true;
    trigOnP1 = micros();
  }
  
  if (valP1 == HIGH && trigP1 == true){
    durationP1 = micros() - trigOnP1;
    lowpulseoccupancyP1 = lowpulseoccupancyP1 + durationP1;
    trigP1 = false;
  }
  
  if(valP2 == LOW && trigP2 == false){
    trigP2 = true;
    trigOnP2 = micros();
  }
  
  if (valP2 == HIGH && trigP2 == true){
    durationP2 = micros() - trigOnP2;
    lowpulseoccupancyP2 = lowpulseoccupancyP2 + durationP2;
    trigP2 = false;
  }

  if ((millis()-starttime) > sampletime_ms)
  {
    ratio = lowpulseoccupancyP1/(sampletime_ms*10.0);                 // int percentage 0 to 100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // spec sheet curve
    Serial.print("lowpulseoccupancyP1:");
    Serial.print(lowpulseoccupancyP1);
    Serial.print(" ratioP1:");
    Serial.print(ratio);
    Serial.print(" countP1:");
    Serial.println(concentration);

    ratio = lowpulseoccupancyP2/(sampletime_ms*10.0);
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62;

    Serial.print("lowpulseoccupancyP2:");
    Serial.print(lowpulseoccupancyP2);
    Serial.print(" ratioP2:");
    Serial.print(ratio);
    Serial.print(" countP2:");
    Serial.println(concentration);

    lowpulseoccupancyP1 = 0;
    lowpulseoccupancyP2 = 0;
    starttime = millis();
  }
}


