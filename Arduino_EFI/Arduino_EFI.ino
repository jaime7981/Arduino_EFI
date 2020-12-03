unsigned long run_time;
float mili_to_hrs = 2.7777777777777776 * pow(10,-7);
float mili_to_min = 0.0000166667;
int volumen_cilindro; // Calcular volumen del cilindro
int densidad_aire; // Calcular densidad de aire segun presion atmosferica
int fuelgrams_per_mili = mili_to_min*0.001; // Calcular los gramos de bencina por minuto segun inyector

int revs[7] = {500,1000,1500,2000,2500,3000,3500};
int throttle[7] = {0,20,40,50,60,80,100};
double efi_map[7][7] = {{17.2, 16.8, 15.5, 14.8, 13.8, 13.0, 12.2},
                        {17.0, 16.5, 15.0, 14.0, 13.4, 13.0, 12.4},
                        {16.8, 16.0, 14.6, 14.2, 13.6, 13.2, 12.6},
                        {16.6, 15.8, 14.8, 14.4, 13.8, 13.4, 12.8},
                        {16.4, 15.5, 15.0, 14.6, 14.0, 13.6, 13.0},
                        {16.2, 15.6, 15.2, 14.8, 14.2, 13.8, 13.2},
                        {16.0, 15.8, 15.5, 15.1, 14.6, 14.0, 13.5}};

float rev;

// Agregar pines
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

double afr;
int air; // masa aire
float fuel;

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
    alternate_in_out = false;
  }
  else if (alternate_in_out == false){
    tiempo_final = run_time;
    variacion_tiempo = tiempo_final - tiempo_inicio;
    variacion_tiempo_inyeccion = variacion_tiempo/4; // cuando inyectar la bencina en la segunda vuelta
    CalcularInyeccion(); // cuanta bencina inyectar
    alternate_in_out = true;
  }
  
  rev = (1/variacion_tiempo)/mili_to_hrs; // Calcula las revoluciones segun la variacion de milisegundos
  return rev;
}

double AFR(){
  for (int i = 0; i < 6; i++){
    if ((rev <= revs[i]) and (200 < rev)){
      for (int a = 0; a < 6; a++){
        if (throtle_input <= throttle[a]){ // deberia ser el input del acelerador digitalRead(throttle_input)
          return efi_map[6-i][a];
        }
      }
    }
  }
  return 0;
}

void CalcularInyeccion(){
  afr = AFR();
  if (afr < 19 and afr > 10){
    air = volumen_cilindro*densidad_aire; // masa aire
    fuel = air/afr;
    Serial.println(fuel);
    PulsosInyector(fuel);
  }
  else{
    digitalWrite(iny_output,LOW);
  }
}

void PulsosInyector(float fuelgrams_to_inyect){ // Esta funcion deberia inyectar cierta cantidad de ml de bencina segun se necesite
  digitalWrite(iny_output,HIGH);
  delay(fuelgrams_to_inyect/fuelgrams_per_mili); // Tiempo de apertura (arreglar)
  digitalWrite(iny_output,LOW);
}