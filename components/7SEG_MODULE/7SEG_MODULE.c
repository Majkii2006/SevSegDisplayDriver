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
	0b00111111, // 0 its for the 0bGFEDCBA pattern, for default kathode controlled display 
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

// helper function for segments setting

static inline void dpSetSegments(SevenSegment_t* display, uint8_t digitIndex) {
	for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++){
		uint8_t bit = display->displayBuffer[digitIndex][actual_segment];
		gpio_set_level(display->segmentPins[actual_segment], display->isAnode ? !bit : bit);
	}
}



void display_setNumber(SevenSegment_t *display, long number) {
	//need to separate the long number provided by the user and then save this numbers to the buffer displayBuffer[]
	
	uint8_t number_reversed = 0;
	uint8_t rest = 0;
	//check if the number is negative => show the number without the sign
	if (number < 0) number = -number;	

	//need to reverse the number:
	while(number != 0) {
		rest = number % 10;
		number_reversed = number_reversed * 10 + rest;
		number /= 10;
	}	



	for (uint8_t actual_digit = 0; actual_digit < display->numDigits; actual_digit++) {

		uint8_t digit = number_reversed % 10;	
		number_reversed = number_reversed / 10;
		
		uint8_t mask = digitPattern[digit];

		for (uint8_t actual_segment = 0; actual_segment < display->numSegments; actual_segment++) {
			uint8_t bit = (mask >> actual_segment) & 1;	
			display->displayBuffer[actual_digit][actual_segment] = bit;
		}
					
	}

	printf("Set all of the numbers correctly!");

} 

void display_worker(SevenSegment_t* display) {
	//worker should only be the thing that displaying the number 		
	//worker only cares about the multiplexing 
	//worker should only turn off the previous digit, go to the next one and turn it on and repeat
	
	//turn off whole screen
	for(uint8_t actual_number = 0; actual_number < display->numDigits; actual_number++) {
		gpio_set_level(display->digitPins[actual_number], display->isAnode ? 1 : 0);
	}	

	dpSetSegments(display, display->whatNumber);

	gpio_set_level(display->digitPins[display->whatNumber], display->isAnode ? 0 : 1);
	
	display->whatNumber++;

	if (display->whatNumber >= display->numDigits) {
		display->whatNumber = 0;	
	}

}





