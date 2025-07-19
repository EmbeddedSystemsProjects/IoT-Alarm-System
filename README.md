# STM32-Smart-Alarm

Smart alarm system based on STM32 as the main microcontroller, interfaced with ESP32 for WiFi communications.

## Introduction

This project is a burglar alarm system designed for home and office security. It combines PIN-based authentication, movement detection, and remote notifications through:

- Authentication via keypad and PIN code  
- Detection of door/window openings and motion  
- Real-time WhatsApp notifications  
- User interface on OLED display

[![Demo Video on YouTube](https://img.youtube.com/vi/71LS64RbG-M/hqdefault.jpg)](https://www.youtube.com/watch?v=71LS64RbG-M)

## Hardware Components

- **STM32 Nucleo-64 Board**: Main processing unit 
- **ESP32 Wemos S2 mini Module**: WiFi handling and notifications  
- **128x64 I2C OLED Display**: User interface  
- **4x4 Matrix Keypad**: PIN code entry  
- **Reed Switch Sensor**: Door/window opening detection  
- **HC-SR501 PIR Sensor**: Motion detection  
- **Active Buzzer**: Acoustic siren  

## Libraries Used

This project makes use of the following open-source libraries:
- [STM32 SSD1306 library](https://github.com/afiskon/stm32-ssd1306): For interfacing with the OLED display.
- [WiFiManager](https://github.com/tzapu/WiFiManager): Simplifies WiFi connectivity setup.

### Steps
1. **Clone the Repository**:
   ```bash
   git clone https://github.com/castagnoemanuele/STM32-smart-alarm.git
   cd STM32-smart-alarm
   ```

2. **Hardware Setup**:
   - Connect all components as per the provided schematic.
   - Ensure proper power supply and secure connections.

3. **Flashing the STM32**:
   - Open the project in STM32CubeIDE.
   - Compile the source code and flash it to the STM32 Nucleo-64 board.

4. **Flashing the ESP32**:
   - Open the ESP32 code in Arduino IDE.
   - Compile and upload the code to the ESP32 S2 mini board.

5. **WhatsApp Notification Setup**:
   - To receive WhatsApp notifications when an alarm is triggered, you need to obtain an API key from CallMeBot:
     1. Add the phone number +34 694 29 84 96 to your phone contacts (name it as you wish)
     2. Send the message "I allow callmebot to send me messages" to this contact via WhatsApp
     3. Wait to receive the message "API Activated for your phone number. Your APIKEY is XXXXXX" from the bot
     4. Enter your phone number (with international prefix) and the received APIKEY in the WiFiManager configuration interface
   - Note: If you don't receive the API key within 2 minutes, please try again after 24 hours.
   
   Example of activation message exchange with CallMeBot:
   
   ![CallMeBot Activation](Assets/callmebot-activation.png)

6. **ESP32 Setup**:
   - On power-up, connect to the "AutoConnectAP" network  
   - Enter WiFi credentials in the web interface  
   - Set phone number and WhatsApp API key  

   ![ESP32 Configuration](Assets/ESP32-Configuration.png)

## Usage

1. **Arming the system**:
   - Press 'A' and enter your PIN  
   - Confirm with 'D'  

2. **Disarming**:
   - Press 'B' and enter your PIN  
   - Confirm with 'D' 

## Technical Notes

- For ESP32, use `driver/i2c.h` instead of `Wire.h`  
- Power components with 5V for stability

## License

This project is licensed under the [MIT License](LICENSE).

## Acknowledgments

Special thanks to the authors of the open-source libraries and the community for their valuable contributions.

---

**Future Developments**:

- Mobile app for remote control  
- Home Assistant integration  
- GSM module for SMS alerts  
- Multi-zone expansion  
