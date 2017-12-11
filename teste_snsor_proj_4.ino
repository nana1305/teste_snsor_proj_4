#include <Wire.h>

#include <Keypad.h>
#define envia_mensagem 1
#define toca_buzzer 2

bool typing = false;//salvar estado da senha
String senhaDigitada;
String senha = "4321";
int buzzer = 11;
long ultimoEstadoPorta;
bool alarmeAtivado;
int sensorPorta = 9;



const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1', '2', '3'},
  {'4', '5', '6'},
  {'7', '8', '9'},
  {'*', '0', '#'}
};

byte rowPins[ROWS] = {5, 4, 3, A0}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {8, 7, 6}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );



void setup() {
  Serial.begin(9600);
  Wire.begin();

  pinMode(A0, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(sensorPorta, INPUT);

}

void requererEnvioSMS() {
  Wire.beginTransmission(8);
  Wire.write(envia_mensagem);
  Wire.endTransmission();
}

void loop() {
  int estadoPorta = digitalRead(sensorPorta);


  char key = keypad.getKey();


  if (key) {

    if (key == '7') {
      alarmeAtivado = true;
      Serial.println("Alarme ativado");
    }

    else if (key == '9') {
      alarmeAtivado = false;
      Serial.println("Alarme desativado");
    }
  }



  if (alarmeAtivado && (estadoPorta == 1)) {
    //preciso dar o start do millis aqui, ele precisa começar a contar os 30s a partir
    //daqui

    long now = millis();

    if (now % 30000  == 0) {
      Serial.println("Entrou no millis");

      if (senhaDigitada != senha) {

        Serial.println("Senha incorreta");


        Serial.println("30 Segundos");
        tone(buzzer, 3000);
        delay (500);
        noTone(buzzer);
        requererEnvioSMS();


        // enviaSMS("Sua casa foi invadida")
      }
    }
  }

  if (key && alarmeAtivado) {

    if (typing && key != '#') {
      senhaDigitada += key;
      Serial.println(senhaDigitada);
    }



    if (key == '*') {
      senhaDigitada = "";
      typing = true;
      Serial.println("Digitando senha...");
    }

    if (key == '#') {
      typing = false;
      Serial.println("Finalizado");


      if (senhaDigitada == senha)
      {
        alarmeAtivado = false;
        noTone (buzzer);
        Serial.println("Alarme desativado");
      }
    }
    // usou o + (mais) pq ambas as variaveis são do tipo String e Strings não somam,
    //elas apenas juntam

  }
}
/* else {
   Serial.println("Acesso negado");
   tone (buzzer, 2000);
  }

  Serial.println(key);
*/


