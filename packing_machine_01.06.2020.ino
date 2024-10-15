#include "HX711.h"           // Include the library for the HX711 load cell sensor
#include <LiquidCrystal_I2C.h> // Include the library for I2C LCD display

// Initialize the LCD with I2C address and dimensions (16 columns, 2 rows)
LiquidCrystal_I2C lcd(0x3f, 16, 2);
HX711 scale;  // Create an instance of the HX711 scale

unsigned long int timeOut, switchControlTimeOut;  // Variables for timing and timeouts

float readScales;  // Variable to store the reading from the scale
bool startSystem = false, stopSystem = true, manual = true;  // Flags for system state and modes
int scalesMode = 1, setScales;  // Variables to track scale modes and set weight

// Pin assignments for various inputs and outputs
const int LOADCELL_DOUT_PIN = 3;
const int LOADCELL_SCK_PIN = 2;
const int conveyorMotorR = 5;
const int conveyorMotorL = 6;
const int startButton = 7;
const int configButton = 8;
const int stopButton = 9;
const int scalesMotorR = 10;
const int scalesMotorL = 11;
const int topSwitch = 12;
const int bottomSwitch = 13;

// Function to configure the system's scale mode based on the selected mode
void configScale(int setScalesMode) {

  switch (setScalesMode) {
    case 1:
      manual = true;           // Set to manual mode
      setScales = 200;         // Set the target weight to 200 grams
      lcd.clear();             // Clear the LCD display
      lcd.setCursor(0, 0);     // Set cursor position
      lcd.print("Program Modu : 1");  // Display mode information
      lcd.setCursor(0, 1);     // Set cursor to second line
      lcd.print("Paket: 200 gr(M)");  // Display weight and mode type
      break;

    case 2:
      manual = false;          // Set to automatic mode
      setScales = 200;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Program Modu : 2");
      lcd.setCursor(0, 1);
      lcd.print("Paket: 200 gr(O)");
      break;

    case 3:
      manual = true;
      setScales = 400;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Program Modu : 3");
      lcd.setCursor(0, 1);
      lcd.print("Paket: 400 gr(M)");
      break;

    case 4:
      manual = false;
      setScales = 400;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Program Modu : 4");
      lcd.setCursor(0, 1);
      lcd.print("Paket: 400 gr(O)");
      break;

    case 5:
      manual = true;
      setScales = 500;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Program Modu : 5");
      lcd.setCursor(0, 1);
      lcd.print("Paket: 500 gr(M)");
      break;

    case 6:
      manual = false;
      setScales = 500;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Program Modu : 6");
      lcd.setCursor(0, 1);
      lcd.print("Paket: 500 gr(O)");
      break;

    case 7:
      manual = true;
      setScales = 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Program Modu : 7");
      lcd.setCursor(0, 1);
      lcd.print("Paket: 1000gr(M)");
      break;

    case 8:
      manual = false;
      setScales = 1000;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Program Modu : 8");
      lcd.setCursor(0, 1);
      lcd.print("Paket: 1000gr(O)");
      break;

    case 9:
      lcd.clear();
      lcd.setCursor(5, 0);
      lcd.print("TARTIM");  // "Weighing Mode"
      lcd.setCursor(6, 1);
      lcd.print("MODU");
      break;

    default:
      break;
  }
}

// Function to handle system failure: stop all motors and display an error
void systemFailed() {

  digitalWrite(conveyorMotorR, LOW);  // Stop conveyor motor (right)
  digitalWrite(conveyorMotorL, LOW);  // Stop conveyor motor (left)
  digitalWrite(scalesMotorR, LOW);    // Stop scale motor (right)
  digitalWrite(scalesMotorL, LOW);    // Stop scale motor (left)

  lcd.clear();
  lcd.setCursor(1, 0);
  lcd.print("MOTOR / SWITCH");  // Display "Motor / Switch Error"
  lcd.setCursor(4, 1);
  lcd.print("ARIZASI");
  delay(3000);  // Delay for error display
  while (1);    // Halt the system (infinite loop)
}

// Function to tare the scale (reset weight)
void dara_alma() {

  scale.set_scale(2415.f);  // Set the scale factor
  scale.tare();             // Tare the scale (set to zero)

  while (!scale.is_ready());  // Wait until the scale is ready
  readScales = scale.get_units(1) * 10;  // Read the scale value and multiply by 10
  scale.power_down();         // Power down the scale for energy saving
  delay(500);                 // Short delay
  scale.power_up();           // Power the scale back up
}

// Function to start the system
void systemStart() {

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("SISTEM");         // Display "System"
  lcd.setCursor(2, 1);
  lcd.print("BASLATILIYOR");   // Display "Starting"
  
  analogWrite(scalesMotorR, 255);  // Start scale motor (right) at full speed
  switchControlTimeOut = millis(); // Store current time for timeout control
  
  // Wait until the top switch is pressed or timeout is reached
  while (!digitalRead(topSwitch) && !((millis() - switchControlTimeOut) > 3000));
  
  if ((millis() - switchControlTimeOut) > 3000) {
    systemFailed();  // Call failure function if timeout
  }
  
  delay(10);  // Small delay
  analogWrite(scalesMotorR, 0);  // Stop the scale motor
  
  dara_alma();  // Perform tare operation

  // If not in weighing mode, start the conveyor
  if (scalesMode != 9) {
    analogWrite(conveyorMotorR, 255);  // Start the conveyor motor (right) at full speed
  }
}

// Function to stop the system
void systemStop() {

  // Stop all motors
  digitalWrite(conveyorMotorR, LOW);
  digitalWrite(conveyorMotorL, LOW);
  digitalWrite(scalesMotorR, LOW);
  digitalWrite(scalesMotorL, LOW);

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("SISTEN");  // Display "System"
  lcd.setCursor(3, 1);
  lcd.print("DURDURULDU");  // Display "Stopped"
  delay(3000);
  configScale(scalesMode);  // Reconfigure the scale after stopping
}

// Setup function, runs once when the program starts
void setup() {

  scale.begin(LOADCELL_DOUT_PIN, LOADCELL_SCK_PIN);  // Initialize the load cell (scale)

  lcd.init();       // Initialize the LCD
  lcd.backlight();  // Turn on the LCD backlight

  delay(1000);      // Short delay before starting

  lcd.clear();
  lcd.setCursor(5, 0);
  lcd.print("SISTEM");  // Display "System"
  lcd.setCursor(4, 1);
  lcd.print("ACILIYOR");  // Display "Starting"
  delay(3000);
  configScale(scalesMode);  // Configure the system's mode

  scale.set_scale(2415.f);  // Set scale factor
  scale.tare();             // Tare the scale

  // Set pin modes for input buttons and switches
  pinMode(startButton, INPUT);
  pinMode(configButton, INPUT);
  pinMode(stopButton, INPUT);
  pinMode(topSwitch, INPUT);
  pinMode(bottomSwitch, INPUT);

  // Set pin modes for motor control outputs
  pinMode(conveyorMotorR, OUTPUT);
  pinMode(conveyorMotorL, OUTPUT);
  pinMode(scalesMotorR, OUTPUT);
  pinMode(scalesMotorL, OUTPUT);

  // Initialize motors to be off
  digitalWrite(conveyorMotorR, LOW);
  digitalWrite(conveyorMotorL, LOW);
  digitalWrite(scalesMotorR, LOW);
  digitalWrite(scalesMotorL, LOW);
}

// Main loop function, runs continuously
void loop() {

  timeOut = millis();  // Store current time for timeout control

  // Wait until the scale is ready, or handle sensor error if timeout occurs
  while (!scale.is_ready()) {
    if ((millis() - timeOut) > 10000) {
      startSystem = false;
      systemStop();  // Stop the system if the scale is not ready in time
      lcd.clear();
      lcd.setCursor(1, 0);
      lcd.print("SENSOR ARIZASI");  // Display "Sensor Error"
      while (!scale.is_ready());    // Wait until the sensor is ready
      configScale(scalesMode);      // Reconfigure the scale mode
    }
  }

  // Read the scale value and multiply by 10
  readScales = scale.get_units(1) * 10;
  
  // Ensure no negative weight is displayed
  if (readScales < 0) {
    readScales = 0.0;
  }

  // If the system is started, display active status and handle weighing logic
  if (startSystem) {

    lcd.clear();
    lcd.setCursor(2, 0);
    lcd.print("SISTEM AKTIF");  // Display "System Active"

    // Display the current weight on the LCD based on the mode
    if (scalesMode == 9) {
      lcd.setCursor(0, 1);
      lcd.print("TARTIM = ");
      lcd.setCursor(9, 1);
      lcd.print(String((int)readScales) + " gr");
    } else {
      if (manual) {
        lcd.setCursor(0, 1);
        lcd.print(String((int)readScales));
        lcd.setCursor(4, 1);
        lcd.print("gr/" + String(setScales) + "gr(M)");  // Display weight in manual mode
      } else {
        lcd.setCursor(0, 1);
        lcd.print(String((int)readScales));
        lcd.setCursor(4, 1);
        lcd.print("gr/" + String(setScales) + "gr(O)");  // Display weight in automatic mode
      }
    }

    // Motor control logic based on the current weight and mode
    if (scalesMode == 1 || scalesMode == 2) {
      if (readScales < 100.0) {
        analogWrite(conveyorMotorR, 255);  // Run the conveyor at full speed
      } else if (readScales >= 100.0 && readScales < 200.0) {
        analogWrite(conveyorMotorR, 100);  // Slow down the conveyor
      } else {
        analogWrite(conveyorMotorR, 0);  // Stop the conveyor
        delay(100);
        if (manual) {
          while (!digitalRead(configButton));  // Wait for config button press
          delay(10);
        }
        analogWrite(scalesMotorL, 200);  // Start the scale motor (left)
        switchControlTimeOut = millis();
        while (!digitalRead(bottomSwitch) && !((millis() - switchControlTimeOut) > 3000));
        if ((millis() - switchControlTimeOut) > 3000) {
          systemFailed();
        }
        delay(10);
        analogWrite(scalesMotorL, 0);  // Stop the motor
        delay(500);
        analogWrite(scalesMotorR, 200);  // Move scale back up
        switchControlTimeOut = millis();
        while (!digitalRead(topSwitch) && !((millis() - switchControlTimeOut) > 3000));
        if ((millis() - switchControlTimeOut) > 3000) {
          systemFailed();
        }
        delay(10);
        analogWrite(scalesMotorR, 0);  // Stop the motor
        delay(100);

        dara_alma();  // Tare the scale

        analogWrite(conveyorMotorR, 255);  // Restart the conveyor
      }
    }

    // Similar logic for other scale modes (3-8) with adjusted weight limits and motor control...

  }

  // Handle mode change with the config button
  if (digitalRead(configButton) && !startSystem) {
    delay(50);
    scalesMode++;
    while (digitalRead(configButton));
    if (scalesMode == 10) {
      scalesMode = 1;  // Reset mode if it exceeds limit
    }
    configScale(scalesMode);  // Reconfigure the scale mode
  }

  // Start the system when the start button is pressed
  if (digitalRead(startButton) && !startSystem) {
    delay(10);
    stopSystem = false;
    startSystem = true;
    systemStart();  // Start the system
  }

  // Stop the system when the stop button is pressed
  if (digitalRead(stopButton) && !stopSystem) {
    delay(10);
    stopSystem = true;
    startSystem = false;
    systemStop();  // Stop the system
  }

}
