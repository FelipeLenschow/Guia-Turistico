void LerBotao() {
  Botao = analogRead(Button) / 100;
//  lcd.setCursor(0, 1);
//  lcd.print(Botao);
  if (lastBotao != Botao) {
    delay(30);
    if (Botao == analogRead(Button) / 100) {
      lastBotao = Botao;
      switch (Botao) {
        case 24:
          menu--;
          enter = 0;
          lcd.clear();
          break;
        case 12:
          enter++;;
          lcd.clear();
          break;
        case 10:
          menu++;
          enter = 0;
          lcd.clear();
          break;
      }
    }
  }
}



void Menu () {
  switch (menu % 6) {
    case 0:
      lcd.setCursor(0, 0);
      lcd.print("Print Sensores");

      if (enter % 3) {
        LerSensores();
        lcdPrintSensores();
      }
      break;


    case 1:
      lcd.setCursor(0, 0);
      lcd.print("Constantes PID");

      while (enter % 3) {
        if (WiFi.status() == WL_CONNECTED)
          ArduinoOTA.handle();
        Botao = analogRead(Button) / 100;
        WifiVerify();
        if (lastBotao != Botao) {
          delay(20);
          if (Botao == analogRead(Button) / 100) {
            lastBotao = Botao;

            switch (Botao) {

              case 24:
                if (enter % 3 == 1)
                  menuK--;
                else if (enter % 3 == 2)
                  switch (menuK % 3) {
                    case 0:
                      Kp -= 0.01;
                      break;
                    case 1:
                      Ki -= 0.01;
                      break;
                    case 2:
                      Kd -= 0.01;
                      break;
                  }
                else
                  menu--;
                break;

              case 12:
                enter++;
                break;
              case 10:

                if (enter % 3 == 1)
                  menuK++;
                else if (enter % 3 == 2)
                  switch (menuK % 3) {
                    case 0:
                      Kp += 0.01;
                      break;
                    case 1:
                      Ki += 0.01;
                      break;
                    case 2:
                      Kd += 0.01;
                      break;
                  }
                else
                  menu++;
                break;
            }
          }


          switch (menuK % 3) {
            case 0:
              lcd.setCursor(0, 1);
              lcd.print("Kp=");
              lcd.print(Kp);
              break;

            case 1:
              lcd.setCursor(0, 1);
              lcd.print("Ki=");
              lcd.print(Ki);
              break;

            case 2:
              lcd.setCursor(0, 1);
              lcd.print("Kd=");
              lcd.print(Kd);
              break;
          }
          lcd.setCursor(10, 1);
          lcd.print(enter);
        }
      }
      break;


    case 2:
      lcd.setCursor(0, 0);
      lcd.print("Run Forrest, Run");
      if (enter % 3) {
        delay(800);
        Run();
      }

      break;


    case 3:
      if (WiFi.status() == WL_CONNECTED) {
        lcd.setCursor(0, 0);
        lcd.print("Desconectar Wifi");
        if (enter % 3) {
          WifiDisconnect();
          enter = 0;
        }
      } else
        lcd.setCursor(0, 0);
      lcd.print("Conectar Wifi   ");
      if (enter % 3) {
        WiFi.begin(mySSID, myPASSWORD);
        enter = 0;
      }
      break;

    case 4:
      lcd.setCursor(0, 0);
      lcd.print("Calibrar Sensores");
      if (enter % 3) {
        WifiDisconnect();
        calibra_sensores();
        enter = 0;
      }

      break;

    case 5:
      lcd.setCursor(0, 0);
      lcd.print("AnalogRead");

      lcd.setCursor(0, 1);
      lcd.print(analogRead(A[5]));  lcd.print("   ");
      break;


  }

}
