/*
  pins_arduino.h - Pin definition functions for Arduino
  Part of Arduino - http://www.arduino.cc/

  Copyright (c) 2007 David A. Mellis
  Modified for ESP8266 platform by Ivan Grokhotkov, 2014-2015.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General
  Public License along with this library; if not, write to the
  Free Software Foundation, Inc., 59 Temple Place, Suite 330,
  Boston, MA  02111-1307  USA

  $Id: wiring.h 249 2007-02-03 16:52:51Z mellis $
*/

#ifndef Pins_Arduino_h
#define Pins_Arduino_h
/*
#define EXTERNAL_NUM_INTERRUPTS 16
#define NUM_DIGITAL_PINS        17
#define NUM_ANALOG_INPUTS       1

#define analogInputToDigitalPin(p)  ((p > 0)?NOT_A_PIN:0)
#define digitalPinToInterrupt(p)    (((p) < EXTERNAL_NUM_INTERRUPTS)?p:NOT_A_PIN)
#define digitalPinHasPWM(p)         (((p) < NUM_DIGITAL_PINS)?p:NOT_A_PIN)

static const uint8_t SDA = 4;
static const uint8_t SCL = 5;
*/
static const uint8_t SS    = 15;
static const uint8_t MOSI  = 13;
static const uint8_t MISO  = 12;
static const uint8_t SCK   = 14;

static const uint8_t LED_BUILTIN = 2;
static const uint8_t BUILTIN_LED = 2;
static const uint8_t A0 = 17;

static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;
static const uint8_t RX   = 3;
static const uint8_t TX   = 1;

/*


// These serial port names are intended to allow libraries and architecture-neutral
// sketches to automatically default to the correct port name for a particular type
// of use.  For example, a GPS module would normally connect to SERIAL_PORT_HARDWARE_OPEN,
// the first hardware serial port whose RX/TX pins are not dedicated to another use.
//
// SERIAL_PORT_MONITOR        Port which normally prints to the Arduino Serial Monitor
//
// SERIAL_PORT_USBVIRTUAL     Port which is USB virtual serial
//
// SERIAL_PORT_LINUXBRIDGE    Port which connects to a Linux system via Bridge library
//
// SERIAL_PORT_HARDWARE       Hardware serial port, physical RX & TX pins.
//
// SERIAL_PORT_HARDWARE_OPEN  Hardware serial ports which are open for use.  Their RX & TX
//                            pins are NOT connected to anything by default.
#define SERIAL_PORT_MONITOR        Serial
#define SERIAL_PORT_USBVIRTUAL     Serial
#define SERIAL_PORT_HARDWARE       Serial
#define SERIAL_PORT_HARDWARE_OPEN  Serial
*/

#endif /* Pins_Arduino_h */


/****
 * Sming Framework Project - Open Source framework for high efficiency native ESP8266 development.
 * Created 2015 by Skurydin Alexey
 * http://github.com/anakod/Sming
 * All files of the Sming Core are provided under the LGPL v3 license.
 ****/

// File name selected for compatibility

#ifndef WIRING_PINS_ARDUINO_H_
#define WIRING_PINS_ARDUINO_H_

// const unsigned int A0 = 17; // Single ESP8266EX analog input pin (TOUT) 10 bit, 0..1v

#define NOT_A_PIN 0
#define NOT_A_PORT 0
#define NOT_ON_TIMER 0

#define PA 1
#define PB 2
#define PC 3

#define GPIO_REG_TYPE uint8_t

// We use maximum compatibility to standard Arduino logic.

// Conversion disabled for now
#define digitalPinToTimer(P) ( NOT_ON_TIMER )

#define digitalPinToPort(P) ( P < 0 ? NOT_A_PIN : ( (int)P < 8 ? PA : ( (int)P < 16 ? PB : ( (int)P == 16 ? PC : NOT_A_PIN ) ) ) )
#define digitalPinToBitMask(P) ( (int)P < 8 ? _BV((int)P) : ( P < 16 ? _BV( (int)P-8 ) : 1) )

#define STD_GPIO_OUT (PERIPHS_GPIO_BASEADDR + GPIO_OUT_ADDRESS)
#define STD_GPIO_IN (PERIPHS_GPIO_BASEADDR + GPIO_IN_ADDRESS)
#define STD_GPIO_ENABLE (PERIPHS_GPIO_BASEADDR + GPIO_ENABLE_ADDRESS)

#define portOutputRegister(P) ( ((volatile uint8_t*)(P != PC ? STD_GPIO_OUT : RTC_GPIO_OUT)) + ( ( ((int)P) == PB ) ? 1 : 0) )
#define portInputRegister(P)  ( ((volatile uint8_t*)(P != PC ? STD_GPIO_IN : RTC_GPIO_IN_DATA)) + ( ( ((int)P) == PB ) ? 1 : 0) )
#define portModeRegister(P)   ( ((volatile uint8_t*)(P != PC ? STD_GPIO_ENABLE : RTC_GPIO_ENABLE)) + ( ( ((int)P) == PB ) ? 1 : 0) ) // Stored bits: 0=In, 1=Out


#endif /* WIRING_PINS_ARDUINO_H_ */
