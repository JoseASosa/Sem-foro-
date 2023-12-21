#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

const int semaforoPrincipal_verde = 13;
const int semaforoPrincipal_amarillo = 12;
const int semaforoPrincipal_rojo = 11;

const int semaforoSecundario_verde = 10;
const int semaforoSecundario_amarillo = 9;
const int semaforoSecundario_rojo = 8;

const int pinDatosTemperatura = 2;

DHT dht(pinDatosTemperatura, DHT11);
LiquidCrystal_I2C lcd(0x27, 16, 2);

int tiempoVerdePrincipal = 10000;
int tiempoVerdeSecundario = 10000;
int tiempoAmarillo = 5000;
int tiempoParpadeoAmarillo = 700;
int tiempoTemporizador = 10000;  // 10 segundos

char temperaturaStr[6];  // Se reserva espacio para la cadena de temperatura

void setup() {
  Wire.begin();  // Iniciar la comunicación I2C
  Wire.setClock(10000);  // Establecer la velocidad de I2C a 10 kHz
  pinMode(semaforoPrincipal_verde, OUTPUT);
  pinMode(semaforoPrincipal_amarillo, OUTPUT);
  pinMode(semaforoPrincipal_rojo, OUTPUT);

  pinMode(semaforoSecundario_verde, OUTPUT);
  pinMode(semaforoSecundario_amarillo, OUTPUT);
  pinMode(semaforoSecundario_rojo, OUTPUT);

  Serial.begin(9600);
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  lcd.print("Control de Trafico");
}

void loop() {
  // Leer temperatura
  float temperatura = dht.readTemperature();

  // Formatear temperatura como cadena de caracteres
  dtostrf(temperatura, 4, 1, temperaturaStr);

  // Actualizar la pantalla LCD y el semáforo
  actualizarSemaforo();

  // Esperar un tiempo antes de la próxima lectura
  delay(600);
}

void actualizarSemaforo() {
  // Mostrar la temperatura en la pantalla LCD
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temperaturaStr);
  lcd.print(" C");

  // Cambio de verde a rojo en el semáforo principal
  digitalWrite(semaforoPrincipal_rojo, LOW);
  digitalWrite(semaforoSecundario_rojo, HIGH);
  digitalWrite(semaforoPrincipal_verde, HIGH);

  mostrarTemporizador(tiempoVerdePrincipal);

  digitalWrite(semaforoPrincipal_verde, LOW);

  for (int i = 0; i < 3; i++) {
    digitalWrite(semaforoPrincipal_amarillo, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Espere");
    delay(tiempoParpadeoAmarillo);
    digitalWrite(semaforoPrincipal_amarillo, LOW);
    delay(tiempoParpadeoAmarillo);
  }

  digitalWrite(semaforoPrincipal_rojo, HIGH);
  delay(1000);

  // Mostrar temporizador solo cuando cambie de rojo a verde en el semáforo principal
  digitalWrite(semaforoSecundario_rojo, LOW);
  digitalWrite(semaforoSecundario_verde, HIGH);

  mostrarTemporizador(tiempoVerdeSecundario);

  digitalWrite(semaforoSecundario_verde, LOW);

  for (int i = 0; i < 3; i++) {
    digitalWrite(semaforoSecundario_amarillo, HIGH);
    lcd.setCursor(0, 1);
    lcd.print("Espere");
    delay(tiempoParpadeoAmarillo);
    digitalWrite(semaforoSecundario_amarillo, LOW);
    delay(tiempoParpadeoAmarillo);
  }

  digitalWrite(semaforoSecundario_rojo, HIGH);
  delay(1000);
}

void mostrarTemporizador(int tiempo) {
  int tiempoRestante = tiempo / 1000;  // Convertir a segundos

  for (int i = 0; i < tiempo / 1000; i++) {
    lcd.setCursor(0, 1);
    lcd.print("Tiempo: ");
    lcd.print(tiempoRestante);
    lcd.print(" s");

    delay(1000);
    tiempoRestante--;
  }
}
