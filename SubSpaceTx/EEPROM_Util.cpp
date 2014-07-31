#include "EEPROM_Util.h"

void EEPROM_Util::erase(byte b) {
  for (int i = EEPROM_MIN_ADDR; i < EEPROM_MAX_ADDR; i++) {
    EEPROM.write(i, b);
  }
}

void EEPROM_Util::serialDump() {
  byte b;
  char buffer[10];

  for (int i = EEPROM_MIN_ADDR; i <= EEPROM_MAX_ADDR; i++) {
    b = EEPROM.read(i);
    sprintf(buffer, "%03X: %02X", i, b);
    Serial.println(buffer);
  }
}

void EEPROM_Util::serialDumpTable(int bytesPerRow) {
  byte b;
  char buffer[10];
  int j = 0;

  for (int i = EEPROM_MIN_ADDR; i <= EEPROM_MAX_ADDR; i++) {
    if (j == 0) {
      sprintf(buffer, "%03X: ", i);
      Serial.print(buffer);
    }

    b = EEPROM.read(i);
    sprintf(buffer, "%02X ", b);
    j++;

    if (j == bytesPerRow) {
      j = 0;
      Serial.println(buffer);
    } else {
      Serial.print(buffer);
    }
  }

  Serial.println();
}

boolean EEPROM_Util::validAddr(int addr) {
  return ((EEPROM_MIN_ADDR <= addr) && (addr <= EEPROM_MAX_ADDR));
}

boolean EEPROM_Util::writeBytes(int startAddr, const byte* array, int numBytes) {
  if (!validAddr(startAddr) || !validAddr(startAddr + numBytes))
    return false;

  for (int i = 0; i < numBytes; i++)
    EEPROM.write(startAddr + i, array[i]);

  return true;
}

boolean EEPROM_Util::readBytes(int startAddr, byte array[], int numBytes) {
  if (!validAddr(startAddr) || !validAddr(startAddr + numBytes))
    return false;

  for (int i = 0; i < numBytes; i++)
    array[i] = EEPROM.read(startAddr + i);

  return true;
}

boolean EEPROM_Util::writeInt(int addr, int value) {
  byte *ptr;
  ptr = (byte*)&value;
  return writeBytes(addr, ptr, sizeof(value));
}

boolean EEPROM_Util::readInt(int addr, int* value) {
  return readBytes(addr, (byte*)value, sizeof(int));
}

boolean EEPROM_Util::writeUint32_t(int addr, uint32_t value) {
  byte *ptr;
  ptr = (byte*)&value;
  return writeBytes(addr, ptr, sizeof(value));
}

boolean EEPROM_Util::readUint32_t(int addr, uint32_t* value) {
  return readBytes(addr, (byte*)value, sizeof(uint32_t));
}

boolean EEPROM_Util::writeString(int addr, const char* string) {
  int numBytes = strlen(string) + 1;
  return writeBytes(addr, (const byte*)string, numBytes);
}

boolean EEPROM_Util::readString(int addr, char* buffer, int bufferSize) {
  byte ch;
  int bytesRead;

  if (!validAddr(addr))
    return false;

  if (bufferSize == 0)
    return false;

  if (bufferSize == 1) {
    buffer[0] = 0;
    return true;
  }

  bytesRead = 0;

  ch = EEPROM.read(addr + bytesRead);
  buffer[bytesRead] = ch;
  bytesRead++;

  while ((ch != 0x00) && (bytesRead < bufferSize) && ((addr + bytesRead) <= EEPROM_MAX_ADDR)) {
    ch = EEPROM.read(addr + bytesRead);
    buffer[bytesRead] = ch;
    bytesRead++;
  }

  if ((ch != 0x00) && (bytesRead >= 1))
    buffer[bytesRead - 1] = 0;

  return true;
}
