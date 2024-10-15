

# Automated Weighing and Packaging System

## Overview
This project implements an **Automated Weighing and Packaging System** using a load cell sensor (HX711), a conveyor belt controlled by motors, and a LiquidCrystal I2C display for user interaction. The system allows you to weigh and package items in various predefined weight modes, either manually or automatically.

### Key Features:
- **Weighing and Packaging Modes**: The system has multiple modes that handle packages weighing 200g, 400g, 500g, and 1000g in both manual and automatic modes.
- **Manual and Automatic Modes**: Users can switch between manual and automatic operations. In manual mode, the user must confirm actions via buttons; in automatic mode, the system operates independently.
- **LCD Display**: The system provides real-time feedback and operational statuses on an I2C-based LCD display.
- **Error Handling**: If the system encounters a motor or switch error, it stops all operations and displays an error message.
- **Taring (Zeroing) the Scale**: The system automatically tares the scale at appropriate times, ensuring accurate weight measurements.

## Hardware Components
- **HX711 Load Cell**: Used to weigh items.
- **I2C LCD Display**: Displays operational modes and system status.
- **Motors**: Control the conveyor belt and scale movements.
- **Buttons**: Used to start, stop, and configure the system.
- **Switches**: Detect top and bottom positions for scale movement control.
  
## Pin Connections
The following table summarizes the pin connections used in this project:

| Component            | Pin on Microcontroller |
|----------------------|------------------------|
| HX711 DOUT (Data)     | Pin 3                  |
| HX711 SCK (Clock)     | Pin 2                  |
| Conveyor Motor Right  | Pin 5                  |
| Conveyor Motor Left   | Pin 6                  |
| Start Button          | Pin 7                  |
| Config Button         | Pin 8                  |
| Stop Button           | Pin 9                  |
| Scale Motor Right     | Pin 10                 |
| Scale Motor Left      | Pin 11                 |
| Top Switch            | Pin 12                 |
| Bottom Switch         | Pin 13                 |

## Installation and Setup

### Prerequisites:
- **Arduino IDE**: Install the latest version of the Arduino IDE.
- **HX711 and LiquidCrystal_I2C Libraries**: Install the necessary libraries for the HX711 load cell and I2C LCD display via the Arduino Library Manager.
  
### Steps to Run:
1. **Connect the Hardware**: Connect the HX711 load cell, I2C LCD, motors, buttons, and switches according to the pin mappings mentioned above.
2. **Upload the Code**: Load the provided code into the Arduino IDE and upload it to the microcontroller.
3. **Power the System**: Ensure the microcontroller and motors are powered appropriately.
4. **Operational Modes**: Upon powering up, the system will display the current mode and allow you to configure and start weighing operations.

### Running the System:
- **Start**: Press the start button to begin the weighing process.
- **Mode Selection**: Use the config button to switch between different weight modes (200g, 400g, 500g, and 1000g).
- **Stop**: Press the stop button to halt the system.
- **Weighing and Packaging**: The system will weigh items and control the conveyor motors based on the selected mode.

### Error Handling:
If any motor or switch fails during operation, the system will stop and display an error on the LCD. The system can only resume when the error is fixed and the process is restarted.

## Code Explanation

### Main Components:
1. **HX711 Load Cell**: The load cell is used to weigh items. It provides weight readings that are multiplied by 10 for easier display and interpretation.
   
2. **I2C LCD Display**: The display is used to show the current system mode, errors, and real-time weight information.

3. **Motor Control**: The system uses motors to control the movement of the conveyor belt and the scale. These motors are controlled based on the current weight and mode.

4. **Taring**: The `dara_alma()` function handles the process of taring (zeroing) the scale to ensure accurate measurements.

5. **Modes**: The system has 9 modes that control the package weights and system operations. Modes 1–8 handle package weights of 200g, 400g, 500g, and 1000g in both manual and automatic modes. Mode 9 is used solely for weighing purposes without packaging.

6. **System Failure**: If any error occurs, the system halts all operations, and an error message is displayed.

### Code Structure:
- **`setup()`**: Initializes the LCD, load cell, motors, and buttons. It also configures the initial scale mode.
- **`loop()`**: Continuously checks for button presses, manages the weighing process, and handles errors and timeouts.

## Future Enhancements:
- **Additional Modes**: Add more package weight modes or allow custom weight entry.
- **Remote Monitoring**: Integrate a communication module to allow remote monitoring of the system status.
- **Data Logging**: Implement a system to log the weight data to an SD card or database for record-keeping.

## License
This project is licensed under the MIT License.

## Contact
For any questions or further information, feel free to reach out:
- **LinkedIn**: [Muhammed Kemal Yağcı](https://www.linkedin.com/in/muhammed-yagci/)

