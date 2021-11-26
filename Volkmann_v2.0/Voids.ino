void Motor(signed int velocidade1, signed int velocidade2) {

  /*
    AIN1, 5
    AIN2, 6
    BIN1, 7
    BIN2, 8
  */
  //      GPIO.out_w1tc = (1 << AIN1);
  //      GPIO.out_w1ts = (1 << AIN2);


  if (velocidade1 > 0) {
    ledcWrite(5, velocidade1);
    ledcWrite(6, 0);
  } else {
    if (velocidade1 == 0) {
      ledcWrite(5, 0);
      ledcWrite(6, 0);
    } else {
      ledcWrite(5, 0);
      ledcWrite(6, -velocidade1);
    }
  }

  if (velocidade2 > 0) {
    ledcWrite(8, velocidade2);
    ledcWrite(7, 0);
  } else {
    if (velocidade2 == 0) {
      ledcWrite(8, 0);
      ledcWrite(7, 0);
    } else {
      ledcWrite(8, 0);
      ledcWrite(7, -velocidade2);
    }
  }

}



void LerSensores() {

  //  Sensores[0] = !(GPIO.in1.val >> A0) & 1;
  //
  //  Sensores[1] = !((GPIO.in1.val >> A1) & 1);
  //  Sensores[2] = !((GPIO.in1.val >> A2) & 1);
  //  Sensores[3] = !((GPIO.in1.val >> A3) & 1);
  //  Sensores[4] = !((GPIO.in >> A4) & 1);
  //  Sensores[5] = !((GPIO.in >> A5) & 1);
  //  Sensores[6] = !((GPIO.in >> A6) & 1);
  //  Sensores[7] = !((GPIO.in >> A7) & 1);
  //  Sensores[8] = !((GPIO.in >> A8) & 1);
  //
  //  Sensores[9] = !(GPIO.in1.val >> A9) & 1;

  for (int i = 0; i <= 10; i++)
    if (analogRead(A[i]) < cut_sensor[i])
      Sensores[i] = true;
    else
      Sensores[i] = false;

  int quantidade = 0;
  for (int i = 1; i < 9; i++)
    if (Sensores[i])
      //if (quantidade < 2)
      quantidade++;


  if (quantidade != 0)
    error = (Sensores[1] * 35 + Sensores[2] * 25 + Sensores[3] * 15 + Sensores[4] * 5 - Sensores[5] * 5 - Sensores[6] * 15 - Sensores[7] * 25 - Sensores[8] * 35) / quantidade;
  else {
    erroA = error;
    error = 1000;
    if (erroA != 1000)
      t_lastMove = millis();
  }
}

void lcdPrintSensores() {
  lcd.setCursor(2, 1);
  lcd.print(Sensores[9]);
  lcd.print(" ");
  lcd.print(Sensores[8]);
  lcd.print(Sensores[7]);
  lcd.print(Sensores[6]);
  lcd.print(Sensores[5]);
  lcd.print(Sensores[4]);
  lcd.print(Sensores[3]);
  lcd.print(Sensores[2]);
  lcd.print(Sensores[1]);
  lcd.print(" ");
  lcd.print(Sensores[0]);


  //  lcd.setCursor(0, 1);
  //  lcd.print(error);
  //  lcd.print("   ");
}

void calibra_sensores() {
  int Max_sensor[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, Min_sensor[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, sensores[10];
  Motor(50, 50);
  int t_calibra = millis();
  while (millis() - t_calibra < 2000) {
    for (int i = 0; i <= 10; i++) {
      sensores[i] = analogRead(A[i]);
      if (sensores[i] > Max_sensor[i])
        Max_sensor[i] = sensores[i];
      else if (sensores[i] < Min_sensor[i])
        Min_sensor[i] = sensores[i];
    }
  }
  for (int i = 0; i <= 10; i++)
    cut_sensor[i] = (Max_sensor[i] + Min_sensor[i]) * 0.2;
  Motor(0, 0);
  lcd.setCursor(0, 1);
  lcd.print(cut_sensor[5]); lcd.print(" ");

}


void Run() {
  WifiDisconnect();
  lcd.noBacklight();
  lcd.clear();
  while (analogRead(Button) / 100 < 2) {
    LerSensores();
    if (error != 1000) {
      P = error;
      I = I + ((error + lastError) / 2);
      D = error - lastError;
      //          lcd.setCursor(map(lastError, -35, 35, 0, 15), 1);
      //          lcd.print(" ");
      //          lcd.setCursor(map(error, -35, 35, 0, 15), 1);
      //          lcd.print("|");
      lastError = error;
      int motorspeed = P * Kp + I * Ki / 1000 + D * Kd / 10;
      Motor(constrain(basespeeda - motorspeed, 110, maxspeeda), constrain(basespeedb + motorspeed, 110, maxspeedb));
      //Motor(basespeeda - motorspeed, basespeedb + motorspeed);

    }
    else if (millis() - t_lastMove > 800) Motor(0, 0);
  }
  enter = 0;
  lcd.backlight();
}
