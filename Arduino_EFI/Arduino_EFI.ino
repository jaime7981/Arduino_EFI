unsigned long run_time;
float rev;

int iny_output;

float throtle_input;

int spark_input;
int spark_pulse;
bool spark_sensor;

float AF_ratio = 14;

bool alternate_in_out = true;

float tiempo_inicio;
float tiempo_final;
float variacion_tiempo;

void setup() {
  Serial.begin(9600);
  pinMode(iny_output, OUTPUT);
  pinMode(spark_input, INPUT);
  pinMode(throtle_input, INPUT);
  
}

void loop() {
  run_time = millis();
  
  if (TrueToFalse(alternate_in_out) == true){
    tiempo_inicio = run_time;
  }
  else if (TrueToFalse(alternate_in_out) == false){
    tiempo_final = run_time;
    variacion_tiempo = tiempo_final - tiempo_inicio;
  }

  if (alternate_in_out == true){
    Serial.print(3600 * 4/variacion_tiempo);
  }
  Serial.println(alternate_in_out);
  
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

bool TrueToFalse(bool valor){
  if (valor == true){
    return false;
  }
  if (valor == false){
    return true;
  }
}
