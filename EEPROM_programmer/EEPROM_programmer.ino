#define SHIFT_DATA 2
#define SHIFT_CLK 3
#define SHIFT_LATCH 4
#define EEPROM_D0 5
#define EEPROM_D7 12
#define WRITE_EN 13

#include "out.h"
unsigned char* data = __bin_out_bin;
unsigned int datasize = __bin_out_bin_len;

/*
 * Output the address bits and outputEnable signal using shift registers.
 */
void setAddress(int address, bool outputEnable) {
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST,
             (address >> 8) | (outputEnable ? 0x00 : 0x80));
    shiftOut(SHIFT_DATA, SHIFT_CLK, MSBFIRST, address);

    digitalWrite(SHIFT_LATCH, LOW);
    digitalWrite(SHIFT_LATCH, HIGH);
    digitalWrite(SHIFT_LATCH, LOW);
}

/*
 * Read a byte from the EEPROM at the specified address.
 */
byte readEEPROM(int address) {
    for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
        pinMode(pin, INPUT);
    }
    setAddress(address, /*outputEnable*/ true);

    byte data = 0;
    for (int pin = EEPROM_D7; pin >= EEPROM_D0; pin -= 1) {
        data = (data << 1) + digitalRead(pin);
    }
    return data;
}

/*
 * Write a byte to the EEPROM at the specified address.
 */
void writeEEPROM(int address, byte data) {
    setAddress(address, /*outputEnable*/ false);
    for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
        pinMode(pin, OUTPUT);
    }

    for (int pin = EEPROM_D0; pin <= EEPROM_D7; pin += 1) {
        digitalWrite(pin, data & 1);
        data = data >> 1;
    }
    digitalWrite(WRITE_EN, LOW);
    delayMicroseconds(1);
    digitalWrite(WRITE_EN, HIGH);
    delay(10);
}

/*
 * Read the contents of the EEPROM and print them to the serial monitor.
 */
void printContents() {
    for (int base = 0; base <= datasize + 255; base += 16) {
        byte data[16];
        for (int offset = 0; offset <= 15; offset += 1) {
            data[offset] = readEEPROM(base + offset);
        }

        char buf[80];
        sprintf(buf,
                "%04x:  %02x %02x %02x %02x %02x %02x %02x %02x   %02x %02x "
                "%02x %02x %02x %02x %02x %02x",
                base, data[0], data[1], data[2], data[3], data[4], data[5],
                data[6], data[7], data[8], data[9], data[10], data[11],
                data[12], data[13], data[14], data[15]);

        Serial.println(buf);
    }
}

void setup() {
    // put your setup code here, to run once:
    pinMode(SHIFT_DATA, OUTPUT);
    pinMode(SHIFT_CLK, OUTPUT);
    pinMode(SHIFT_LATCH, OUTPUT);
    digitalWrite(WRITE_EN, HIGH);
    pinMode(WRITE_EN, OUTPUT);
    Serial.begin(57600);

    // Erase entire EEPROM
    Serial.print("Erasing EEPROM");
    for (long address = 0; address <= datasize + 255; address += 1) {
        // data to erase with:
        writeEEPROM(address, 0xea);

        if (address % 512 == 0) {
            Serial.print(".");
        }
    }
    writeEEPROM(0x7ffc, 0x00);
    writeEEPROM(0x7ffd, 0x80);

    Serial.println(" done");

    Serial.print("Integer size: ");
    Serial.println(sizeof(int));

    Serial.print("Long size: ");
    Serial.println(sizeof(long));

    // Program data bytes
    Serial.print("Programming EEPROM");
    for (int address = 0; address < datasize; address += 1) {
        writeEEPROM(address, data[address]);

        if (address % 64 == 0) {
            Serial.print(". :) ");
        }
    }
    Serial.println(" done");

    // Read and print out the contents of the EERPROM
    Serial.println("Reading EEPROM");
    printContents();
}

void loop() {
    // put your main code here, to run repeatedly:
}