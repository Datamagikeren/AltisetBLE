void drawMenu() {
  // If there was a previously highlighted option, draw a black box over it to "deselect" it
  if (prevOption != -1) {
    tft.drawRect(5, 15 + prevOption * 20, 110, 18, BLACK);
  }

  // Draw a CYAN box around the currently selected option
  tft.drawRect(5, 15 + currentOption * 20, 110, 18, CYAN);

  for (int i = 0; i < 3; i++) {
    tft.setCursor(10, 20 + i * 21);
    tft.setTextColor(WHITE, BLACK);
    printOption(i);
  }

  prevOption = currentOption;
}

void printOption(int option) {
  switch (option) {
    case 0:
      tft.print("Set Offset   ");  // Added spaces to ensure it overwrites longer sub-options
      break;
    case 1:
      tft.print("Shut Down   ");
      break;
    case 2:
      tft.print("Run Test   ");
      break;
  }
}

void displayFeetScreen() {
  if (!feetScreenInitialized) {
    tft.fillScreen(BLACK);  // Clear the screen
    tft.setCursor(10, 57);  // Position to display the "Feet: " text
    tft.print("Feet: ");
    feetScreenInitialized = true;
  }

  // Determine the width required to overwrite the previous feet value, and clear that space
  tft.fillRect(55, 57, 50, 9, BLACK);  // This clears the previous value; adjust the numbers if needed

  // Now, print the updated feet value
  tft.setCursor(55, 57); // Adjust this as per where "Feet: " ends
  tft.print(feet);
}



