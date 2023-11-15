

void connectToPeripheral() {
  unsigned long processStartTime = millis();
  numPeripherals = 0;

  while (numPeripherals < MAX_PERIPHERALS) {
    // Check if the entire process has exceeded 10 seconds
    // if (millis() - processStartTime > 10000) {
    //   Serial.println("Connection process timed out after 10 seconds.");
    //   return;  // Exit the function if more than 10 seconds have passed
    // }

    Serial.println("- Discovering peripheral device...");

    unsigned long scanStartTime = millis();
    while (millis() - scanStartTime < 3000) {
      BLE.scanForUuid(deviceServiceUuid);
      peripheral = BLE.available();
      if (digitalRead(BTN_LEFT) == LOW) {
        inFeetScreen = false;
        inMainMenu = true;
        Serial.println("Stopped scanning");
        numPeripherals = 1;
      }

      if (peripheral) {
        break;  // Exit the while loop if a peripheral is found
      }
    }

    if (peripheral) {
      Serial.println("* Peripheral device found!");
      Serial.print("* Device MAC address: ");
      Serial.println(peripheral.address());
      Serial.print("* Device name: ");
      Serial.println(peripheral.localName());
      Serial.print("* Advertised service UUID: ");
      Serial.println(peripheral.advertisedServiceUuid());
      Serial.println(" ");
      BLE.stopScan();
      controlPeripheral(peripheral);
      delay(500);
      broadcastUart(UARTMessage);
    } else {
      Serial.println("* No peripheral device found within scan period.");
      // Optionally, you can add a short delay before the next scan
      delay(1000);
    }
  }
}



void controlPeripheral(BLEDevice peripheral) {
  Serial.println("- Connecting to peripheral device...");

  if (peripheral.connect()) {
    Serial.println("* Connected to peripheral device!");
    Serial.println(" ");

  } else {
    Serial.println("* Connection to peripheral device failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }

  Serial.println("- Discovering peripheral device attributes...");
  if (peripheral.discoverAttributes()) {
    Serial.println("* Peripheral device attributes discovered!");
    Serial.println(" ");
    Serial.print("Successfully connected to: ");
    Serial.println(peripheral.localName());
    receiveCharacteristic = peripheral.characteristic(receiveCharacteristicUuid);
    Serial.println("OK1");

    if (receiveCharacteristic) {
      isReceiveCharacteristicDiscovered = true;
      Serial.println("OK2");

      if (receiveCharacteristic.canSubscribe()) {
        Serial.println("OK3");
        receiveCharacteristic.subscribe();
        receiveCharacteristic.setEventHandler(BLEUpdated, notificationCallback);
        Serial.println("* Subscribed to receive characteristic notifications!");
      } else {
        Serial.println("* Unable to subscribe to notifications.");
      }
    } else {
      Serial.println("* Receive characteristic not found!");
      peripheral.disconnect();
      return;
    }

    if (numPeripherals < MAX_PERIPHERALS) {
      peripherals[numPeripherals] = peripheral;
      wasConnected[numPeripherals] = true;
      numPeripherals++;
    } else {
      Serial.println("Already at max peripherals, cannot connect to another.");
    }
    printCurrentConnections();

  } else {
    Serial.println("* Peripheral device attributes discovery failed!");
    Serial.println(" ");
    peripheral.disconnect();
    return;
  }
}

void disconnectFromPeripheral() {
  if (peripheral && peripheral.connected()) {
    peripheral.disconnect();
    Serial.println("Peripheral has been manually disconnected!");
    delay(1000);
  }
}

void disconnectFromPeripheral1() {
  if (numPeripherals > 0) {
    peripheral = peripherals[0];
    if (peripheral.connected()) {
      String peripheralName = peripheral.localName();
      String peripheralAddress = peripheral.address();
      peripheral.disconnect();
      Serial.println("Peripheral at index 0 has been manually disconnected!");
      Serial.print("Peripheral Name: ");
      Serial.println(peripheralName);
      Serial.print("Peripheral Address: ");
      Serial.println(peripheralAddress);
      delay(1000);

      numPeripherals--;
      for (int j = 0; j < numPeripherals; j++) {
        peripherals[j] = peripherals[j + 1];
        wasConnected[j] = wasConnected[j + 1];
      }
      printCurrentConnections();
    }
  } else {
    Serial.println("No connected peripherals to disconnect.");
  }
}

void printCurrentConnections() {
  Serial.println("Current connected devices: ");
  for (int i = 0; i < numPeripherals; i++) {
    if (peripherals[i].connected()) {
      Serial.print("Peripheral ");
      Serial.print(i + 1);
      Serial.print(" | ");
      Serial.print("Name: ");
      Serial.print(peripherals[i].localName());
      Serial.print(" | ");
      Serial.print(" MAC Address: ");
      Serial.println(peripherals[i].address());
    }
  }
}

void broadcastUart(String UARTMessage) {
  for (int i = 0; i < numPeripherals; i++) {
    if (peripherals[i].connected()) {
      BLECharacteristic uartCharacteristic = peripherals[i].characteristic(deviceServiceCharacteristicUuid);
      if (uartCharacteristic.canWrite()) {
        uartCharacteristic.writeValue(UARTMessage.c_str());
      }
    }
  }
}
String messageBuffer = "";  // a buffer to store fragmented received messages

void notificationCallback(BLEDevice central, BLECharacteristic characteristic) {
  const uint8_t* data = characteristic.value();
  String response = String((char*)data);

  // Extract the first line from the response
  int indexOfNewline = response.indexOf('\n');  // Find the position of the first newline character
  String firstLine = (indexOfNewline != -1) ? response.substring(0, indexOfNewline) : response;

  firstLine.trim();  // Remove any whitespace

  // Remove # from UARTMessage
  String cleanedUARTMessage = UARTMessage;
  cleanedUARTMessage.replace("#", "");

  // Debugging: print lengths
  Serial.print("Received first line: ");
  Serial.println(firstLine);
  Serial.print("Received length: ");
  Serial.println(firstLine.length());
  Serial.print("Expected first line: ");
  Serial.println(cleanedUARTMessage);
  Serial.print("Expected length: ");
  Serial.println(cleanedUARTMessage.length());

  // Check if the first line matches your cleaned expected data
  if (firstLine == cleanedUARTMessage) {
    // Handle the logic when the first line matches
    Serial.println("OK");

  } else {
    // Handle the logic when the first line doesn't match
    Serial.println("NOT OK");
  }
}
