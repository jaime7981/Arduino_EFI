unsigned long run_time;
float rev;

int iny_output;

float throtle_input;

int spark_input;
int spark_pulse;
bool spark_sensor;

float AF_ratio = 14;

void setup() {
  Serial.begin(9600);
  pinMode(iny_output, OUTPUT);
  pinMode(spark_input, INPUT);
  pinMode(throtle_input, INPUT);
  
}

void loop() {
  run_time = millis();
  
  if (digitalRead(spark_input) > 10){
    spark_sensor = true;
  }
  
  if (spark_sensor == true){
    spark_pulse += 1;
  } 
  spark_sensor = false;
  Serial.println(RevCalc());

}

float RevCalc(){
  float new_time = run_time % 1000;
  if (new_time == 0){
    spark_pulse = 0;
  }
  rev = 3600 * (spark_pulse / new_time);
  return rev;
}
