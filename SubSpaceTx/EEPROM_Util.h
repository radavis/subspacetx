//
// EEPROM Utilities
//
// modified from http://playground.arduino.cc/Code/EepromUtil
//

#ifndef __EEPROM_UTIL_H__
#define __EEPROM_UTIL_H__

#include <Arduino.h>
#include <EEPROM.h>

#define EEPROM_MIN_ADDR 0
#define EEPROM_MAX_ADDR 1023

class EEPROM_Util {
  public:
    static void erase(byte b=0xFF);

    static void serialDump();
    static void serialDumpTable(int bytesPerRow=16);

    static boolean writeBytes(int startAddr, const byte* array, int numBytes);
    static boolean readBytes(int startAddr, byte array[], int numBytes);

    static boolean writeInt(int addr, int value);
    static boolean readInt(int addr, int* value);

    static boolean writeUint32_t(int addr, uint32_t value);
    static boolean readUint32_t(int addr, uint32_t* value);

    static boolean writeString(int addr, const char* string);
    static boolean readString(int addr, char* buffer, int bufferSize);

  private:
    static boolean validAddr(int addr);
};

#endif
