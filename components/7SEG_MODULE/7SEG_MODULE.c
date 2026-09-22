#include <stdint.h>

#include "7SEG_MODULE.h"

#include "driver/gpio.h"

#include "hal/gpio_types.h"

static const uint8_t digitPatterns[] = {
	0b00111111, // 0 its for the 0bGFEDCBA pattern, for default kathode controlled display, we can do it for anode via negation every bit
	0b00000110, // 1
	0b01011011, // 2
	0b01001111, // 3
	0b01100110, // 4
	0b01101101, // 5
	0b01111101, // 6
        0b00000111, // 7
	0b01111111, // 8
	0b01101111  // 9
};




void display_init(SevenSegment_t* display, const uint8_t segPins[], uint8_t nSeg, const uint8_t digPins[], uint8_t nDig, bool isAno) {

	//overfill protection	
	if (nDig > MAX_DIGITS) { 
		nDig = MAX_DIGITS;
	}

	display->segmentPins = segPins;
	display->digitPins = digPins;
	display->numDigits = nDig;
	display->numSegments = nSeg;
	display->isAnode = isAno;

	//initialization as output
	for (uint8_t i = 0; i < display->numSegments; i++) {
		gpio_set_direction(display->segmentPins[i], GPIO_MODE_OUTPUT);
	}	
	for (uint8_t i = 0; i < display->numDigits; i++) {
		gpio_set_direction(display->digitPins[i], GPIO_MODE_OUTPUT);
		gpio_set_level(display->digitPins[i], display->isAnode ? 0 : 1);
	}

}
