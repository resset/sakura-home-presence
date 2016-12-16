#include <Arduino.h>
#include <Ethernet.h>

#define BLINK_INTERVAL 200

#define PIN_SENSOR_1 7
#define PIN_SENSOR_2 6

#define LED_1 5
#define LED_2 4

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};

// Set the static IP address to use if the DHCP fails to assign
IPAddress ip(192, 168, 0, 177);
// Set the static DNS address to use if the DHCP fails to assign
IPAddress dns(8, 8, 8, 8);
// Gateway for static IP configuration
IPAddress gateway(192, 168, 0, 1);
// Subnet for static IP configuration
IPAddress subnet(255, 255, 255, 0);

char server[] = "example.com";         // name address (using DNS)
//IPAddress server(192, 168, 0, 1);    // numeric IP (no DNS)

// Initialize the Ethernet client library with the IP address and port
// of the server that you want to connect to (port 80 is default for HTTP).
EthernetClient client;

void interrupt0_handler()
{
    digitalWrite(PIN_LED0, !digitalRead(PIN_LED0));
}

void setup_ethernet()
{
    // Start the Ethernet connection
    Serial.println("Eth: starting DHCP...");
    if (Ethernet.begin(mac) == 0) {
        Serial.println("Eth: failed to configure Ethernet using DHCP");
        // No point in carrying on, so do nothing forevermore,
        // try to congifure using IP address instead of DHCP
        Ethernet.begin(mac, ip, dns, gateway, subnet);
    } else {
        Serial.print("Eth: DHCP success, acquired IP: ");
          Serial.println(Ethernet.localIP());
    }
    // Delay for Ethernet to set up
    delay(1000);
}

void setup()
{
    pinMode(PIN_LED0, OUTPUT);
    pinMode(PIN_LED1, OUTPUT);
    pinMode(PIN_LED2, OUTPUT);
    pinMode(PIN_LED3, OUTPUT);
    pinMode(LED_1, OUTPUT);
    pinMode(LED_2, OUTPUT);
    digitalWrite(PIN_LED0, 1);
    digitalWrite(PIN_LED1, 0);
    digitalWrite(PIN_LED2, 0);
    digitalWrite(PIN_LED3, 0);
    digitalWrite(LED_1, 0);
    digitalWrite(LED_2, 0);

    Serial.begin(115200);
    delay(3000);
    Serial.println("Hello World, Sakura!");

	pinMode(0, INPUT);
    attachInterrupt(0, interrupt0_handler, CHANGE);

    pinMode(PIN_SENSOR_1, INPUT);
    pinMode(PIN_SENSOR_2, INPUT);

    setup_ethernet();
}

void send_data(int sensor_number)
{
    Serial.println("Eth: connecting...");

    if (client.connect(server, 80)) {
        Serial.println("Eth: connected");

        // Make a HTTP request
        client.print("GET /index.php?presence_sensor=");
        client.print(sensor_number);
        client.println(" HTTP/1.1");
        client.println("Host: example.com");
        client.println("Connection: close");
        client.println();

        /*if (client.available()) {
            char c = client.read();
            Serial.print(c);
        }
        if (!client.connected()) {
        }*/

        client.stop();
        Serial.println("Eth: disconnected");
    } else {
        Serial.println("Ethernet: connection failed");
    }
}

void loop()
{
    digitalWrite(PIN_LED0, 1);
    delay(BLINK_INTERVAL);
    digitalWrite(PIN_LED0, 0);
    delay(BLINK_INTERVAL);

    if (digitalRead(PIN_SENSOR_1)) {
        digitalWrite(PIN_LED1, 1);
        digitalWrite(LED_1, 1);

        Serial.print("Sending data from sensor: ");
        Serial.println(1);
        send_data(1);

        digitalWrite(PIN_LED1, 0);
        digitalWrite(LED_1, 0);
    }
    if (digitalRead(PIN_SENSOR_2)) {
        digitalWrite(PIN_LED2, 1);
        digitalWrite(LED_2, 1);

        Serial.print("Sending data from sensor: ");
        Serial.println(2);
        send_data(2);

        digitalWrite(PIN_LED2, 0);
        digitalWrite(LED_2, 0);
    }
}
