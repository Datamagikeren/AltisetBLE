// void connectToPeripheral() {
//     unsigned long processStartTime = millis();
//     Serial.println("- Discovering peripheral device...");
//     unsigned long scanStartTime = millis();

//     while (millis() - scanStartTime < 10000) {
      
//         BLE.scanForUuid(deviceServiceUuid);
//         Serial.println("Scanning");
//         peripheral = BLE.available();
        

//         if (peripheral) {
//             break;  // Exit the while loop if a peripheral is found
//         }

//         // Check if the left button is clicked to return to the main menu
//         if (digitalRead(BTN_LEFT) == LOW) {
//             BLE.stopScan();
//             inFeetScreen = false;
//             inMainMenu = true;
//             drawMenu();
//             return;  // Exit the function early
//         }

//         // Add a short delay for button debounce and to not overwhelm the BLE scan
//         delay(50);
//     }

//     if (peripheral) {
//         Serial.println("* Peripheral device found!");
//         Serial.print("* Device MAC address: ");
//         Serial.println(peripheral.address());
//         Serial.print("* Device name: ");
//         Serial.println(peripheral.localName());
//         Serial.print("* Advertised service UUID: ");
//         Serial.println(peripheral.advertisedServiceUuid());
//         Serial.println(" ");
//         BLE.stopScan();
//         controlPeripheral(peripheral);
//     } else {
//         Serial.println("* No peripheral device found within scan period.");
//         // Optionally, you can add a short delay before the next scan
//         delay(1000);
//     }
// }


// void controlPeripheral(BLEDevice peripheral) {

//   Serial.println("- Connecting to peripheral device...");

//   if (peripheral.connect()) {
//     Serial.println("* Connected to peripheral device!");
//     Serial.println(" ");

//   } else {
//     Serial.println("* Connection to peripheral device failed!");
//     Serial.println(" ");
//     return;
//   }
  
//   Serial.println("- Discovering peripheral device attributes...");
//   if (peripheral.discoverAttributes()) {
//     Serial.println("* Peripheral device attributes discovered!");
//     Serial.println(" ");
//     Serial.print("Successfully connected to: ");
//     Serial.println(peripheral.localName());
//     receiveCharacteristic = peripheral.characteristic(receiveCharacteristicUuid);
//     Serial.println("OK1");

//     if (receiveCharacteristic) {
//       isReceiveCharacteristicDiscovered = true;
//       Serial.println("OK2");

//       if (receiveCharacteristic.canSubscribe()) {
//         Serial.println("OK3");
//         receiveCharacteristic.subscribe();
//         receiveCharacteristic.setEventHandler(BLEUpdated, notificationCallback);
//         Serial.println("* Subscribed to receive characteristic notifications!");
//       } else {
//         Serial.println("* Unable to subscribe to notifications.");
//       }
//     } else {
//       Serial.println("* Receive characteristic not found!");
//       peripheral.disconnect();
//       return;
//     }
//   } else {
//     Serial.println("* Peripheral device attributes discovery failed!");
//     Serial.println(" ");
//     peripheral.disconnect();
//     return;
//   }

  
// }
// String messageBuffer = "";  // a buffer to store fragmented received messages
// void notificationCallback(BLEDevice central, BLECharacteristic characteristic) {
//   const uint8_t* data = characteristic.value();
//   String response = String((char*)data);

//   // Extract the first line from the response
//   int indexOfNewline = response.indexOf('\n');  // Find the position of the first newline character
//   String firstLine = (indexOfNewline != -1) ? response.substring(0, indexOfNewline) : response;

//   firstLine.trim();  // Remove any whitespace

//   // Remove # from UARTMessage
//   String cleanedUARTMessage = UARTMessage;
//   cleanedUARTMessage.replace("#", "");

//   // Debugging: print lengths
//   Serial.print("Received first line: ");
//   Serial.println(firstLine);
//   Serial.print("Received length: ");
//   Serial.println(firstLine.length());
//   Serial.print("Expected first line: ");
//   Serial.println(cleanedUARTMessage);
//   Serial.print("Expected length: ");
//   Serial.println(cleanedUARTMessage.length());

//   // Check if the first line matches your cleaned expected data
//   if (firstLine == cleanedUARTMessage) {
//     // Handle the logic when the first line matches
//     Serial.println("OK");
//   } else {
//     // Handle the logic when the first line doesn't match
//     Serial.println("NOT OK");
//   }
// }

// void broadcastUart(String UARTMessage) {
//     if (peripheral.connected()) {
//       BLECharacteristic uartCharacteristic = peripheral.characteristic(deviceServiceCharacteristicUuid);
//       if (uartCharacteristic.canWrite()) {
//         uartCharacteristic.writeValue(UARTMessage.c_str());
//       }
//     }
// }
