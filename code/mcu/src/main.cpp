#include <PlayXSVFJTAGArduino.h>
//#include <PlayXSVFJTAGAVR.h>

//
// The maximum size of the arduino receive buffer is 63 bytes by default.
// This number can be changed if you change the compiler flags in the
// file hardware/arduino/avr/platform.local.txt (-O2 makes it faster):
//
// compiler.c.extra_flags=-DSERIAL_BUFFER_SIZE=256 -DSERIAL_RX_BUFFER_SIZE=256 -O2
// compiler.cpp.extra_flags=-DSERIAL_BUFFER_SIZE=256 -DSERIAL_RX_BUFFER_SIZE=256 -O2
//

#define VERSION		"1.01"

#ifndef SERIAL_RX_BUFFER_SIZE
#define SERIAL_RX_BUFFER_SIZE 64
#endif /* SERIAL_RX_BUFFER_SIZE */

#ifdef ARDUINO_MEGA
#error Building for Arduino not currently supported
#define TMS_PIN  	8
#define TDI_PIN  	9
#define TDO_PIN  	10
#define TCK_PIN  	11
#define VREF_PIN 	12
#else
#define VPP_EN		8	// PB6
#define ACT_LED		5	// PC6
#define RDY_LED		6	// PC7
#define TMS_PIN  	19	// PD0
#define TDI_PIN  	18	// PD1
#define TDO_PIN  	0	// PD2
#define TCK_PIN  	1	// PD3
#define VREF_PIN 	12	// Should be PD4...
#endif

void setup()
{
	pinMode(VPP_EN, OUTPUT);
	pinMode(ACT_LED, OUTPUT);
	pinMode(RDY_LED, OUTPUT);

	digitalWrite(VPP_EN, LOW);
	digitalWrite(ACT_LED, LOW);
	digitalWrite(RDY_LED, LOW);

	Serial.begin(115200);

	digitalWrite(RDY_LED, HIGH);

	// Wait for USB serial...
	bool act_on = true;
	while (!Serial) {
		for (int i = 0; i < 10; i++) {
			if (Serial) {
				break;
			} else {
				delay(100);
			}
		}

		// 1sec
		act_on = !act_on;
		digitalWrite(ACT_LED, act_on ? LOW : HIGH);
	}

	digitalWrite(ACT_LED, HIGH);
	digitalWrite(RDY_LED, LOW);
}

void loop()
{
	digitalWrite(VPP_EN, LOW);

	while (true) {
		int command = Serial.read();

		if (command == 'G') {
			// Go - break to play
			break;
		}
		if (command == 'H') {
			// High voltage go - enable and break to play
			digitalWrite(VPP_EN, HIGH);
			break;
		}
		if (command == 'V') {
			// Dump version
			Serial.print(F("\r\nQ0,"));
			Serial.print("Little ATF Programmer Board v");
			Serial.println(VERSION);
		}
	}

	PlayXSVFJTAGArduino p(Serial, SERIAL_RX_BUFFER_SIZE, TMS_PIN, TDI_PIN, TDO_PIN, TCK_PIN, VREF_PIN, true);
	//PlayXSVFJTAGAVR p(Serial, SERIAL_RX_BUFFER_SIZE);

	p.play();	
}
