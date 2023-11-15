#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
#include <ArduinoBLE.h>
// #include <nrf_nvic.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128  // Change this to 96 for 1.27" OLED.

// #define SCLK_PIN 13
// #define MOSI_PIN 11
#define DC_PIN 11
#define CS_PIN 7
#define RST_PIN 10

#define BLACK 0x0000
#define BLUE 0x001F
#define RED 0xF800
#define GREEN 0x07E0
#define CYAN 0x07FF
#define MAGENTA 0xF81F
#define YELLOW 0xFFE0
#define WHITE 0xFFFF

Adafruit_SSD1351 tft = Adafruit_SSD1351(128, 128, &SPI, CS_PIN, DC_PIN, RST_PIN);
// Adafruit_SSD1351 tft = Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, CS_PIN, DC_PIN, MOSI_PIN, SCLK_PIN, RST_PIN);


const char* deviceServiceUuid = "6E400001-B5A3-F393-E0A9-E50E24DCCA9E";
const char* deviceServiceCharacteristicUuid = "6E400002-B5A3-F393-E0A9-E50E24DCCA9E";
const char* receiveCharacteristicUuid = "6E400003-B5A3-F393-E0A9-E50E24DCCA9E";  // Add this line

BLECharacteristic receiveCharacteristic;
BLEDevice peripheral;

const int BTN_LEFT = 2;
const int BTN_UP = 3;
const int BTN_DOWN = 4;
const int BTN_RIGHT = 5;

int feet = 500;
int numPeripherals = 0;

String UARTMessage = "W-17-00000-00000-00000-00200#";

bool isReceiveCharacteristicDiscovered = false;


uint16_t currentTextColor = WHITE; // Default to WHITE

bool inMainMenu = true;  // Starts in the main menu by default

int currentOption = 0;  // Start with Option 1 selected
int prevOption = -1;     // To keep track of previously selected option
int currentSubOption = 0;
int prevSubOption = -1;  // This will store the previous submenu option

bool inFeetScreen = true;

bool isScanning = false;

const int MAX_PERIPHERALS = 1;
BLEDevice peripherals[MAX_PERIPHERALS];

bool wasConnected[MAX_PERIPHERALS] = { false };

void connectToPeripheral();
void controlPeripheral(BLEDevice peripheral);
void notificationCallback(BLEDevice central, BLECharacteristic characteristic);
void broadcastUart(String UARTMessage);

bool feetScreenInitialized = false;


