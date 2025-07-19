#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <Preferences.h>
#include <HTTPClient.h>
#include <Arduino.h>
#include <driver/i2c.h>

Preferences prefs;

String phoneNumber = "";
String apiKey = "";

#define SDA_PIN 37
#define SCL_PIN 39
#define I2C_ADDRESS 0x5C // I2C address of the slave device

void sendWhatsAppAlert(String message)
{
    // Check if phone number and API key are valid
    String phone = phoneNumber;
    String api = apiKey;
    phone.trim();
    api.trim();

    if (phone.length() == 0 || api.length() == 0)
    {
        Serial.println("[ERROR] Phone number or API key not set!");
        return;
    }

    String encodedMessage = "";
    for (int i = 0; i < message.length(); i++)
    {
        char c = message[i];
        if (isalnum(c))
        {
            encodedMessage += c;
        }
        else
        {
            encodedMessage += "%" + String(c, HEX);
        }
    }

    String url = "https://api.callmebot.com/whatsapp.php?phone=" + phone +
                 "&text=" + encodedMessage +
                 "&apikey=" + api;

    Serial.println("URL: " + url); // Print URL for debugging

    HTTPClient http; // Declare HTTPClient object

    http.begin(url);                                  // Initialize HTTP request
    http.addHeader("User-Agent", "ESP32-SmartAlarm"); // Add user-agent header
    int httpCode = http.GET();                        // Send GET request

    if (httpCode > 0)
    {
        Serial.println("[OK] WhatsApp sent! Response: " + String(httpCode));
    }
    else
    {
        Serial.println("[FAIL] Error sending WhatsApp: " + http.errorToString(httpCode));
    }

    http.end(); // End HTTP request
}

void setup()
{
    ///////////////////// SERIAL ///////////////////////
    Serial.begin(115200);
    delay(1000); // Give time for serial monitor to open

    /////////////////////// I2C ///////////////////////
    i2c_config_t conf = {};
    conf.sda_io_num = (gpio_num_t)SDA_PIN;
    conf.scl_io_num = (gpio_num_t)SCL_PIN;
    conf.sda_pullup_en = GPIO_PULLUP_ENABLE;
    conf.scl_pullup_en = GPIO_PULLUP_ENABLE;
    conf.mode = I2C_MODE_SLAVE;
    conf.slave.addr_10bit_en = 0;
    conf.slave.slave_addr = I2C_ADDRESS;

    Serial.println("[OK] I2C Slave started");    i2c_param_config(I2C_NUM_0, &conf);
    ESP_ERROR_CHECK(i2c_driver_install(I2C_NUM_0, I2C_MODE_SLAVE, 128, 128, 0));
    Serial.println("[OK] I2C Slave driver installed");

    /////////////////////// PINS ///////////////////////
    pinMode(LED_BUILTIN, OUTPUT); // Initialize LED digital pin 15 as an output

    // Flash LED to indicate boot
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);
    delay(500);
    digitalWrite(LED_BUILTIN, HIGH);
    delay(500);
    digitalWrite(LED_BUILTIN, LOW);

    ////////////////////// WIFI ///////////////////////
    WiFiManager wm;

    WiFiManagerParameter custom_phone("phone", "Phone number (+39...)", "", 16);
    WiFiManagerParameter custom_api("apikey", "API Key", "", 40);

    wm.addParameter(&custom_phone);
    wm.addParameter(&custom_api);

    if (!wm.autoConnect("AutoConnectAP", "password"))
    {
        Serial.println("[FAIL] WiFi Connect failed, restarting");
        ESP.restart();
    }

    Serial.println("[OK] Connected to WiFi");
    digitalWrite(LED_BUILTIN, HIGH);

    prefs.begin("config", false);

    // If first time, save; else load existing
    if (String(custom_phone.getValue()).length() > 0 && String(custom_api.getValue()).length() > 0)
    {
        prefs.putString("phone", custom_phone.getValue());
        prefs.putString("apikey", custom_api.getValue());
        Serial.println("[OK] Saved new phone and API key");
    }

    // Load saved values into global vars
    phoneNumber = prefs.getString("phone", "");
    apiKey = prefs.getString("apikey", "");

    Serial.println("[INFO] Loaded phone: " + phoneNumber);
    Serial.println("[INFO] Loaded API key: " + apiKey);

    sendWhatsAppAlert("System ready and online"); // Test message

    prefs.end();
}

void loop()
{
    static uint8_t data[128];
    static size_t data_len = 0;
    static unsigned long lastBlink = 0;

    // Handle incoming data
    data_len = i2c_slave_read_buffer(I2C_NUM_0, data, sizeof(data), 10 / portTICK_PERIOD_MS);
    if (data_len > 0)
    {
        Serial.print("Received ");
        Serial.print(data_len);
        Serial.println(" byte(s):");

        for (size_t i = 0; i < data_len; i++)
        {
            Serial.print("Received char: ");
            Serial.println((char)data[i]);

            // Flash LED as feedback
            digitalWrite(LED_BUILTIN, LOW);
            delay(100);
            digitalWrite(LED_BUILTIN, HIGH);
            delay(100);

            if (data[i] == '1')
            {                // Send IP address to master
                String ipAddress = WiFi.localIP().toString();
                Serial.println("Sending IP: " + ipAddress);
                
                // Clear the buffer first
                memset(data, 0, sizeof(data));
                
                size_t ip_len = ipAddress.length();
                for (size_t j = 0; j < ip_len; j++)
                {
                    data[j] = ipAddress[j];
                }
                data[ip_len] = '\0'; // Ensure proper null termination
                
                // Use a timeout of 100ms instead of 10ms
                esp_err_t write_status = i2c_slave_write_buffer(I2C_NUM_0, data, ip_len + 1, 100 / portTICK_PERIOD_MS);
                
                if (write_status == ESP_OK) {
                    Serial.println("IP address sent successfully");
                } else {
                    Serial.println("Failed to send IP address: " + String(write_status));
                }

                Serial.println("Command 1 received: Sent IP address: " + ipAddress);
            }
            else if (data[i] == '2')
            {
                // Send WhatsApp alert for alarm
                Serial.println("Command 2 received: Sending alarm message via WhatsApp.");
                sendWhatsAppAlert("ALERT: Movement detected!");
            }
            else if (data[i] == '3')
            {
                // Send WhatsApp for door/window open
                Serial.println("Command 3 received: Sending door/window open message via WhatsApp.");
                sendWhatsAppAlert("ALERT: Door/Window opened!");
            }
            else
            {
                Serial.println("Unknown command received.");
            }
            data[i] = 0; // Clear data buffer for next read
        }
    }

    // Blink LED every 100ms (0.05s on, 0.05s off)
    if (millis() - lastBlink > 100)
    {
        digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); // Toggle LED state
        lastBlink = millis();
    }
}