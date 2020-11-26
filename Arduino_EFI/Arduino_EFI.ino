unsigned long run_time;
float mili_to_hrs = 2.7777777777777776 * pow(10,-7);
float mili_to_min = 0.0000166667;
float rev;

int iny_output;
float throtle_input;
int spark_input;

int spark_pulse;
bool spark_sensor;
int spark_input_simulator;

bool alternate_in_out = true;

float tiempo_inicio;
float tiempo_final;
float variacion_tiempo;
float variacion_tiempo_inyeccion;
float variacion_tiempo_inyeccion_alternativo;

void setup() {
  Serial.begin(9600);
  pinMode(iny_output, OUTPUT);
  pinMode(spark_input, INPUT);
  pinMode(throtle_input, INPUT);
  
}

void loop() {
  run_time = millis();

  spark_input_simulator = random(0,5); // simula una vuelta del motor
  
  if (spark_input_simulator > 3){ // deberia ser el input de la bujia digitalRead(spark_input)
    spark_sensor = true;
  }
  else{
    spark_sensor = false;
  }
  
  if (spark_sensor == true){
    spark_pulse += 1; // Contaria las veces que hay chispa
    RevCalc();
    Serial.println(rev);
  }
}


float RevCalc(){
  if (alternate_in_out == true){
    tiempo_inicio = run_time;
    variacion_tiempo = tiempo_inicio - tiempo_final;
    variacion_tiempo_inyeccion = variacion_tiempo;
    alternate_in_out = false;
    TiempoInyeccion();
  }
  else if (alternate_in_out == false){
    tiempo_final = run_time;
    variacion_tiempo = tiempo_final - tiempo_inicio;
    variacion_tiempo_inyeccion_alternativo;
    alternate_in_out = true;
  }
  
  rev = (1/variacion_tiempo)/mili_to_hrs; // Calcula las revoluciones segun la variacion de milisegundos en dos vueltas
  return rev;
}

void TiempoInyeccion(){
  variacion_tiempo_inyeccion = variacion_tiempo_inyeccion/2; // Tiempo valvula de aire abierta
  Serial.println(variacion_tiempo_inyeccion);
}
