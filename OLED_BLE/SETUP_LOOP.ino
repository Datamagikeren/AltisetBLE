void setup() {
  Serial.begin(9600);
  while (!Serial)
    ;
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);

  tft.begin();
  tft.fillScreen(BLACK);
  tft.setTextSize(1.4);   // You can change this value for a different text size
  tft.setCursor(10, 57);  // Center the text. Adjust this value for your liking
  if (!BLE.begin()) {
    Serial.println("* Starting Bluetooth® Low Energy module failed!");
    while (1)
      ;
  }

  BLE.setLocalName("Nano 33 BLE (Central)");
  BLE.advertise();

  Serial.println("Arduino Nano 33 BLE Sense (Central Device)");
  Serial.println(" ");
  drawMenu();
}

void loop() {
  
  
  if (inMainMenu) {
    if (digitalRead(BTN_UP) == LOW) {
      currentOption--;
      if (currentOption < 0) {
        currentOption = 2;
      }
      Serial.println(currentOption);
      drawMenu();
    } else if (digitalRead(BTN_DOWN) == LOW) {
      currentOption++;
      if (currentOption > 2) {
        currentOption = 0;
      }
      Serial.println(currentOption);
      drawMenu();
    } else if (digitalRead(BTN_RIGHT) == LOW) {
      if (currentOption == 0) { // "Set Offset" option is selected
        Serial.println("Right buttn clicked");
        inMainMenu = false;
        inFeetScreen = true;
        displayFeetScreen();
      }
    }
    // delay(10);  // Simple debouncing
  } else if (inFeetScreen) {
     if (digitalRead(BTN_UP) == LOW) {
      feet += 5;
      displayFeetScreen();
      delay(50); // Simple debouncing
    } 
    else if (digitalRead(BTN_DOWN) == LOW) {
      feet -= 5;
      displayFeetScreen();
      delay(50); // Simple debouncing
    }
    connectToPeripheral();
    delay(500); // You might want to adjust this delay
    broadcastUart(UARTMessage);
    delay(500);
    
    
    // Check if the left button is clicked to return to the main menu
    if (digitalRead(BTN_LEFT) == LOW) {
      inFeetScreen = false;
      inMainMenu = true;
      feetScreenInitialized = false;  // Reset the flag
      Serial.println("Stopped scanning");
      tft.fillScreen(BLACK);
      drawMenu();
    }
  }
}

