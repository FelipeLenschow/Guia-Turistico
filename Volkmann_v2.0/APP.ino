void WifiVerify() {
  WiFiClient client = server.available();
  if (client) {
    client.println("<!DOCTYPE html><html><head><title>Page Title</title></head><body><h1>My First Heading</h1><p>My first paragraph.</p></body></html>");
    //client.println("ok"); //Algum client.print é obrigatorio
    currentLine = "";
    t = 1;
    while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        if (c != '\r')
          currentLine += c;
      }
    }
    Serial.print(currentLine);
    client.stop();
  }

  if (t)
    ExtrairInfo();
}

void ExtrairInfo() {



  if (currentLine.substring(5, 14) == "DESLIGAAA") {
    Motor(0, 0);
    ESP.restart();
  }

  else if (currentLine.substring(5, 6) == "A") {
    menu--;
    enter = 0;
    lcd.clear();
    lcd.noBacklight();
    delay(100);
    lcd.backlight();
  }

  else if (currentLine.substring(5, 6) == "B") {
    enter++;;
    lcd.clear();
  }

  else if (currentLine.substring(5, 6) == "C") {
    menu++;
    enter = 0;
    lcd.clear();
  }

  else if (currentLine.substring(5, 6) == "M") {
  }

  else if (currentLine.substring(5, 6) == "m") {
  }

  t = 0;
}
