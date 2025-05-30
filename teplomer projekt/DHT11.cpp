/**
 * DHT11.cpp
 * Library for reading temperature and humidity from the DHT11 sensor.
 *
 * Author: Dhruba Saha
 * Version: 2.1.0
 * License: MIT
 */

#include "DHT11.h"
#include <stdint.h>

#define INPUT 0
#define OUTPUT 1
#define LOW 0
#define HIGH 1

void delayMicroseconds(unsigned int us) {
}

DHT11::DHT11(
    int pin,
    PinModeFunc pinModeFunc,
    DigitalWriteFunc digitalWriteFunc,
    DigitalReadFunc digitalReadFunc,
    DelayFunc delayFunc
) : _pin(pin), _pinMode(pinModeFunc), _digitalWrite(digitalWriteFunc), _digitalRead(digitalReadFunc), _delay(delayFunc), _delayMS(500) {}

int DHT11::readRawData(uint8_t data[5])
{
    _delay(_delayMS);
    startSignal();
    unsigned long timeout_start = 0;

    while (_digitalRead(_pin) == HIGH) {
    }

    if (_digitalRead(_pin) == LOW) {
        delayMicroseconds(80);
        if (_digitalRead(_pin) == HIGH) {
            delayMicroseconds(80);
            for (int i = 0; i < 5; i++) {
                data[i] = readByte();
                if (data[i] == DHT11::ERROR_TIMEOUT) {
                    return DHT11::ERROR_TIMEOUT;
                }
            }
            if (data[4] == ((data[0] + data[1] + data[2] + data[3]) & 0xFF)) {
                return 0; // Success
            } else {
                return DHT11::ERROR_CHECKSUM;
            }
        }
    }
    return DHT11::ERROR_TIMEOUT;
}

uint8_t DHT11::readByte()
{
    uint8_t value = 0;
    for (int i = 0; i < 8; i++) {
        while (_digitalRead(_pin) == LOW);
        delayMicroseconds(30);
        if (_digitalRead(_pin) == HIGH) {
            value |= (1 << (7 - i));
        }
        while (_digitalRead(_pin) == HIGH);
    }
    return value;
}

void DHT11::startSignal()
{
    _pinMode(_pin, OUTPUT);
    _digitalWrite(_pin, LOW);
    _delay(18);
    _digitalWrite(_pin, HIGH);
    delayMicroseconds(40);
    _pinMode(_pin, INPUT);
}

int DHT11::readTemperature()
{
    uint8_t data[5];
    int error = readRawData(data);
    if (error != 0) {
        return error;
    }
    return data[2];
}

int DHT11::readHumidity()
{
    uint8_t data[5];
    int error = readRawData(data);
    if (error != 0) {
        return error;
    }
    return data[0];
}

int DHT11::readTemperatureHumidity(int &temperature, int &humidity)
{
    uint8_t data[5];
    int error = readRawData(data);
    if (error != 0) {
        return error;
    }
    humidity = data[0];
    temperature = data[2];
    return 0; // Indicate success
}

const char* DHT11::getErrorString(int errorCode)
{
    switch (errorCode) {
        case DHT11::ERROR_TIMEOUT:
            return "Error 253: Reading from DHT11 timed out.";
        case DHT11::ERROR_CHECKSUM:
            return "Error 254: Checksum mismatch while reading from DHT11.";
        default:
            return "Error: Unknown.";
    }
}
