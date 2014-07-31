# SubSpaceTx

Display tweets to #subspacetx on a LCD.


## Twitter specs

 * 140 character message
 * 15 character username


## Hardware

 * Arduino Uno (1024B EEPROM)
 * Robot LCD Shield v1.0
 * Adafruit CC3000 WiFi module


## EEPROM

 * Each Tweet takes up 156 bytes (140B message + 16B username)
 * 6 tweets can fit into 936 bytes
 * 8 for checksum
 * 32 for IP
 * 48 bytes left


## Software

 * Ruby Sinatra app that monitors for tweets with hashtag #subspacetx. Serves data via JSON and TXT.
 * Arduino code to connect to WiFi, retrieve and parse JSON tweets, save message and username to EEPROM, read data from EEPROM and display on the LCD.


## Notes

Very beta. Much bugs.
