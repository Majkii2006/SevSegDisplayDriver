#include <stdint.h>

#include "7SEG_MODULE.h"

#include "esp_etm.h"
#include "esp_rom_sys.h"

#include "driver/gpio.h"

#include "freertos/FreeRTOS.h"
#include "freertos/idf_additions.h"
#include "freertos/projdefs.h"

#include "hal/gpio_types.h"

static const uint8_t digitPattern[] = {
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
		gpio_set_level(display->digitPins[i], display->isAnode ? 0 : 1); //check for kathode or anode default LED
	}

}


void display_setNumber(SevenSegment_t *display, long number) {
	//need to separate the long number provided by the user and then save this numbers to the buffer displayBuffer[]
	//check if the number is negative => show the number without the sign
	if (number < 0) number = -number;	

	for (uint8_t i = 0; i < display->numDigits; i++) {
		display->displayBuffer[i] = number % 10;
		number /= 10; //it's like the integer division so we don't care about the rest
			      // number 328 --> displayBuffer[0] = 8; 
			      // number 328 --> displayBuffer[1] = 2;
			      // number 328 --> displayBuffer[2] = 3;
	}


} 

void display_refresh(SevenSegment_t *display) {
	//the main refreshing function that provides the multiplexing of our LED screen
	for (uint8_t d = 0; d < display->numDigits; d++) {
		for (uint8_t i = 0; i < display->numDigits; i++) {
			gpio_set_level(display->digitPins[i], display->isAnode ? 0 : 1);
		}

		//Take the number from buffer:
		uint8_t num = display->displayBuffer[d];
		uint8_t mask = digitPattern[num];

		for (uint8_t s = 0; s < display->numSegments; s++) {
			uint8_t bit = (mask >> s) & 1;

			gpio_set_level(display->segmentPins[s], display->isAnode ? !bit : bit);
		}

			gpio_set_level(display->digitPins[d], display->isAnode ? 1 : 0);
			vTaskDelay(pdMS_TO_TICKS(10));
	}	
}





