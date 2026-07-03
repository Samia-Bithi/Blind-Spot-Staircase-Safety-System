/*
 * esp32_staircase_monitor.ino
 *
 * ESP32-S3 – Staircase Monitor with UART + WiFi + Blynk IoT
 * ===========================================================
 *
 * Wiring:
 *   ATmega32A PD1 (TX) --> Voltage Divider --> ESP32-S3 GPIO16 (RX2)
 *   ATmega32A GND      --> ESP32-S3 GND
 *
 * Messages received from ATmega32A:
 *   "FALL_DETECTED\n"  → LED ON  on Blynk
 *   "EXIT_OK\n"        → LED OFF on Blynk
 *
 * Baud rate ATmega↔ESP32 : 4800
 * Serial Monitor baud     : 115200
 */

#define BLYNK_TEMPLATE_ID    "TMPL6Ob03wymj"
#define BLYNK_TEMPLATE_NAME  "Atmega32"
#define BLYNK_AUTH_TOKEN     "ocglyclJo71iw65MsJcrZBdeDZxxeF3G"

#define BLYNK_PRINT Serial

#include <WiFi.h>
#include <BlynkSimpleEsp32.h>

/* ── WiFi credentials ─────────────────────────────────────── */
#define WIFI_SSID     "Edge 50"
#define WIFI_PASSWORD "@1234567890"

/* ── UART pins ────────────────────────────────────────────── */
#define RXD2      16
#define TXD2      17
#define BAUD_RATE 4800

/* ── Virtual pin ──────────────────────────────────────────── */
#define VPIN_LED  V1    /* LED widget – ON on fall, OFF on exit */

String receivedMessage = "";

/* ══════════════════════════════════════════════════════════ */
/*  Setup                                                     */
/* ══════════════════════════════════════════════════════════ */
void setup()
{
    Serial.begin(115200);
    delay(2000);

    Serial.println("=================================");
    Serial.println("  Staircase Monitor - ESP32-S3  ");
    Serial.println("  Connecting to Blynk...         ");
    Serial.println("=================================");

    /* UART2 – receive from ATmega32A */
    Serial2.begin(BAUD_RATE, SERIAL_8N1, RXD2, TXD2);

    /* Connect WiFi + Blynk */
    Blynk.begin(BLYNK_AUTH_TOKEN, WIFI_SSID, WIFI_PASSWORD);

    Serial.println("=========================================");
    Serial.println("  WiFi + Blynk Connected!               ");
    Serial.print  ("  IP Address: "); Serial.println(WiFi.localIP());
    Serial.println("=========================================");

    /* LED OFF at start */
    Blynk.virtualWrite(VPIN_LED, 0);
}

/* ══════════════════════════════════════════════════════════ */
/*  Loop                                                      */
/* ══════════════════════════════════════════════════════════ */
void loop()
{
    Blynk.run();

    /* ── Read UART from ATmega32A ───────────────────────── */
    while (Serial2.available())
    {
        char c = (char)Serial2.read();

        if (c == '\n')
        {
            receivedMessage.trim();
            handleMessage(receivedMessage);
            receivedMessage = "";
        }
        else
        {
            receivedMessage += c;
        }
    }
}

/* ══════════════════════════════════════════════════════════ */
/*  Handle message from ATmega32A                             */
/* ══════════════════════════════════════════════════════════ */
void handleMessage(String msg)
{
    if (msg.length() == 0) return;

    Serial.print("[WiFi] ");
    Serial.println(WiFi.status() == WL_CONNECTED ? "Online" : "Offline");
    Serial.print("[ATmega32A] --> ");
    Serial.println(msg);

    if (msg == "FALL_DETECTED")
    {
        Serial.println("*******************************************");
        Serial.println("  !! FALL DETECTED ON STAIRCASE !!         ");
        Serial.println("  Person has not moved for 15 seconds.      ");
        Serial.println("  Buzzer has been beeping for 5 seconds.    ");
        Serial.println("  Immediate attention may be required!      ");
        Serial.println("*******************************************");
        Serial.println();

        Blynk.virtualWrite(VPIN_LED, 255);      /* LED ON */
        Blynk.logEvent("fall_detected", "Person may have fallen on staircase!");
    }
    else if (msg == "EXIT_OK")
    {
        Serial.println("-------------------------------------------");
        Serial.println("  Person exited staircase safely.           ");
        Serial.println("  System reset and ready.                   ");
        Serial.println("-------------------------------------------");
        Serial.println();

        Blynk.virtualWrite(VPIN_LED, 0);        /* LED OFF */
    }
    else
    {
        Serial.print("  [Unknown message received]: ");
        Serial.println(msg);
    }
}