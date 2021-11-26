#include "OTA.h"
const char* mySSID = "LenHide";//"Mi 9t";//
const char* myPASSWORD = "01010101";

bool Sensores[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int cut_sensor[10];
int tempo = 0;
signed int error = 0, erroA = 0;
int t_lastMove = 0;
String currentLine = "";
bool t = 0;

#include <WiFi.h>
#include <WiFiUdp.h>
#include <HTTPClient.h>
HTTPClient http;
WiFiUDP UDP;
#define CHANNEL 7
WiFiServer server(80);

int Botao = 0;
int lastBotao = 0;
unsigned int menu = 1;
unsigned int menuK = 1;
int enter = 0;

float Kp = 30.0;
float Ki = 1.20;
float Kd = 15;
int P;
int I;
int D;
int lastError = 0;
const uint8_t maxspeeda = 220;
const uint8_t maxspeedb = 220;
const uint8_t basespeeda = 90;
const uint8_t basespeedb = 90;


#define BIN2 16
#define BIN1 17
#define AIN1 5
#define AIN2 18

#define Button 34//12
const byte A[10] = {39, 35, 32, 33, 25, 26, 27, 14, 12, 36};


//#include <Wire.h>
//#include <LiquidCrystal_I2C.h>
//LiquidCrystal_I2C lcd(0x27, 16, 2);


void setup() {
  //Motor(0, 0);
  lcd.init();
  Serial.begin(115200);
  Serial.println("Booting");
  setupOTA("Volkmann v2.0 ", mySSID, myPASSWORD);
  server.begin();

  pinMode(BIN2, OUTPUT);
  pinMode(BIN1, OUTPUT);
  pinMode(AIN1, OUTPUT);
  pinMode(AIN2, OUTPUT);
  pinMode(Button, INPUT_PULLUP);
  for (int i = 0; i < 10; i++)
    pinMode(A[i], OUTPUT);


  ledcSetup(5, 1000, 8);  //Atribuimos ao canal 5 a frequencia de 1000Hz com resolucao de 8bits.
  ledcAttachPin(AIN1, 5);  //Atribuimos o pino AIN1 ao canal 5.
  ledcSetup(6, 1000, 8);
  ledcAttachPin(AIN2, 6);
  ledcSetup(7, 1000, 8);
  ledcAttachPin(BIN1, 7);
  ledcSetup(8, 1000, 8);
  ledcAttachPin(BIN2, 8);


  Motor(0, 0);
  lcd.backlight(); // LIGA A ILUMINAÇÃO DO DISPLAY
  lcd.clear(); // LIMPA O DISPLAY
  lcd.print("Guia Turistico!");
  lcd.setCursor(0, 1);
  lcd.print(WiFi.localIP());
  delay(2000);
  lcd.clear();


}

void loop() {
  if (WiFi.status() == WL_CONNECTED)
    ArduinoOTA.handle();
  //WifiVerify();
  //if (WiFi.waitForConnectResult() != WL_CONNECTED)
  //  WiFi.begin(mySSID, myPASSWORD);

  LerBotao();
  Menu();
  //LerSensores();
  //lcdPrintSensores();

}
