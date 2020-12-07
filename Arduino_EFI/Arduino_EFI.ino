unsigned long run_time;
float mili_to_hrs = 2.7777777777777776 * pow(10,-7);
float mili_to_min = 0.0000166667;
float volumen_cilindro = 443.319215; // volumen del cilindro en cm3
float densidad_aire = 0.00100845492; // densidad de aire segun presion atmosferica en gr/cm3
float gramos_aire = volumen_cilindro*densidad_aire;
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
int iny_output = 2;

float throtle_input = A1;
float throttle_read;

int spark_input = A2;
float spark_read;

float spark_pulse;
bool spark_sensor;
bool one_spark = true;
int spark_input_simulator;

bool alternate_in_out = true;

float tiempo_inicio;
float tiempo_final;
float variacion_tiempo;
float variacion_tiempo_inyeccion;
float variacion_tiempo_inyeccion_alternativo;

double afr;
double fuel;

void setup() {
  Serial.begin(9600);
  pinMode(iny_output, OUTPUT);
  pinMode(spark_input, INPUT);
  pinMode(throtle_input, INPUT);
  
}

void loop() {
  run_time = millis();
  throttle_read = analogRead(throtle_input); // posicion del acelerador (Es logaritmica)
  throttle_read = 100*throttle_read/1024;
  
  spark_read = analogRead(spark_input); // Simula Revoluciones
  spark_read = spark_read/(1024);
  spark_pulse += spark_read;

  //Serial.print(sin(spark_pulse/50));Serial.print(" / ");Serial.print(one_spark); Serial.print(" / ");Serial.println(spark_sensor);

  //spark_input_simulator = random(0,5); // simula una vuelta del motor
  
  if (sin(spark_pulse)  > 0.99){ // deberia ser el input de la bujia digitalRead(spark_input)
    if (one_spark == true and spark_sensor == false){
      one_spark = false;
      spark_sensor = true;
    }
    else if (one_spark == false){
      spark_sensor = false;
    }
  }
  else{
    spark_sensor = false;
    one_spark = true;
  }
  
  if (spark_sensor == true){
    RevCalc();
    Serial.print(rev);Serial.print(" / "); Serial.print(throttle_read); 
    Serial.print(" / ");Serial.print(fuel*1000);Serial.print(" / "); Serial.println(afr);
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
  
  rev = (1/variacion_tiempo)/(mili_to_hrs*10); // Calcula las revoluciones segun la variacion de milisegundos (Arreglar para el motor)
  return rev;
}

double AFR(){
  for (int i = 0; i < 7; i++){
    if ((rev <= revs[i]) and (200 < rev)){
      for (int a = 0; a < 7; a++){
        if (throttle_read <= throttle[a]){ // deberia ser el input del acelerador digitalRead(throttle_input)
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
    fuel = gramos_aire/afr;
    //PulsosInyector(fuel);
  }
  else{
    digitalWrite(iny_output,LOW);
  }
}

void PulsosInyector(float fuelgrams_to_inyect){ // Esta funcion deberia inyectar cierta cantidad de ml de bencina segun se necesite
  digitalWrite(iny_output,HIGH);
  //delay(fuelgrams_to_inyect/fuelgrams_per_mili); // Tiempo de apertura (arreglar)
  digitalWrite(iny_output,LOW);
}
