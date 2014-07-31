/*

  SubSpaceTx.ino

  Arduino code for the SubSpaceTx project.

  Display messages from http://subspacetx.herokuapp.com, which
  takes its data from searching twitter for messages with the
  hashtag #subspacetx.

  It works, sometimes...

*/

#include <Adafruit_CC3000.h>
#include <ccspi.h>
#include <SPI.h>
#include <string.h>
#include <LiquidCrystal.h>
#include "WIFI_Config.h"

#include <EEPROM.h>
#include "EEPROM_Util.h"

#define ADAFRUIT_CC3000_IRQ 3
#define ADAFRUIT_CC3000_VBAT 2
#define ADAFRUIT_CC3000_CS 10  // SCK = 13, MISO = 12, and MOSI = 11

#define IDLE_TIMEOUT_MS 10000  // because heroku takes forever to spool up
#define WEBSITE "subspacetx.herokuapp.com"
#define WEBPAGE "/tweets.txt"

#define MESSAGE_SPACE 140
#define USERNAME_SPACE 17
#define NUM_TWEETS 6
#define BUFFER_SPACE 156
#define IP 942

#define LCD_COLS 16
#define LCD_ROWS 2

Adafruit_CC3000 cc3000 = Adafruit_CC3000(ADAFRUIT_CC3000_CS,
  ADAFRUIT_CC3000_IRQ, ADAFRUIT_CC3000_VBAT, 14);

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

uint32_t ip;

char buffer[BUFFER_SPACE];
char username[USERNAME_SPACE];
char message[MESSAGE_SPACE];

long updateInterval = 300000;  // 5 minutes
long previousTime = updateInterval;  // update on boot

void setup() {
  Serial.begin(115200);
  Serial.println(F("Welcome to the #subspacetx interface."));

  lcd.begin(LCD_COLS, LCD_ROWS);  // columns, rows

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("#subspacetx LCD"));
  delay(5000);
}

void loop() {
  unsigned long currentTime = millis();

  if ((currentTime - previousTime) > updateInterval) {
    previousTime = currentTime;

    lcd.setCursor(0, 0);
    lcd.print(F("#subspacetx LCD"));
    lcd.setCursor(0, 1);
    lcd.print(F("  updating...   "));
    Serial.println(F("Updating..."));

    getTweets();
  }

  //EEPROM_Util::serialDumpTable();

  EEPROM_Util::readString(0, username, USERNAME_SPACE);
  EEPROM_Util::readString(17, message, MESSAGE_SPACE);

  lcd.clear();
  lcd.setCursor(0, 0);
  Serial.println(username);
  lcd.print(username);

  message[LCD_COLS + 1] = '\0';
  Serial.println(message);
  lcd.setCursor(0, 1);
  lcd.print(message);

  delay(5000);
}

void getTweets() {
  cc3000.begin();
  cc3000.connectToAP(WLAN_SSID, WLAN_PASS, WLAN_SECURITY);
  cc3000.checkDHCP();

  while (!getConnectionDetails()) {
    delay(1000);
  }

  printIPtoLCD();

  ip = 0;
  while (ip == 0) {
    if (!cc3000.getHostByName(WEBSITE, &ip)) {
      Serial.println(F("Couldn't resolve!"));
    }
    delay(500);
  }

  cc3000.printIPdotsRev(ip);
  Serial.println();

  Adafruit_CC3000_Client www = cc3000.connectTCP(ip, 80);
  if (www.connected()) {
    www.fastrprint(F("GET "));
    www.fastrprint(WEBPAGE);
    www.fastrprint(F(" HTTP/1.1\r\n"));
    www.fastrprint(F("Host: "));
    www.fastrprint(WEBSITE);
    www.fastrprint(F("\r\n"));
    www.fastrprint(F("\r\n"));
    www.println();
  } else {
    Serial.println(F("Connection failed"));
    return;
  }

  unsigned long lastRead = millis();
  int i = 0;

  while (www.connected() && (millis() - lastRead < IDLE_TIMEOUT_MS)) {
    while (www.available()) {
      char c = www.read();
      if (c == '@')  // Start of tweet
        i = 0;

      if (i < BUFFER_SPACE)
        buffer[i++] = c;

      Serial.print(c);
      lastRead = millis();
    }
  }
  www.close();
  Serial.println();

  EEPROM_Util::writeString(0, buffer);

  cc3000.disconnect();
}

bool getConnectionDetails() {
  uint32_t ip, netmask, gateway, dhcp, dns;

  if(!cc3000.getIPAddress(&ip, &netmask, &gateway, &dhcp, &dns)) {
    return false;

  } else {
    Serial.print(F("\nIP:      "));
    cc3000.printIPdotsRev(ip);

    Serial.print(F("\nDNS:     "));
    cc3000.printIPdotsRev(dns);
    Serial.println();

    EEPROM_Util::writeUint32_t(IP, ip);

    return true;
  }
}

void printIPtoLCD() {
  uint32_t ip;
  EEPROM_Util::readUint32_t(IP, &ip);

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print(F("#subspacetx LCD"));
  lcd.setCursor(0, 1);
  lcd.print(F("ip: "));
  lcd.print((uint8_t)(ip >> 24));
  lcd.print('.');
  lcd.print((uint8_t)(ip >> 16));
  lcd.print('.');
  lcd.print((uint8_t)(ip >> 8));
  lcd.print('.');
  lcd.print((uint8_t)(ip));
}

