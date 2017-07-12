#ifndef four_digits_h
#define four_digits_h

#include <inttypes.h>
#include <Arduino.h>

class FOUR_DIGITS {
	public:
		void init(uint8_t Data, uint8_t Clk);
		void setBrightness(uint8_t data);
		void enableDisplay(bool data);
		void enableClockPoint(bool data);
		void reverseClockPoint(void);
		void writeFull(short int data);
		void writeFullRaw(uint32_t data);
		void writeSelected(uint8_t disp, uint8_t number, bool raw = false);
		void clearDisplay(void);
	private:
		const uint8_t SETTINGS = 0b01000000;
		const uint8_t ADDRESS = 0b11000000;
		const uint8_t CONTROL = 0b10000000;
		const uint8_t AUTO_ADDR = 0b00000000;
		const uint8_t FIX_ADDR = 0b00000100;
		uint8_t Datapin;
		uint8_t Clkpin;
		uint8_t brightness = 5;
		uint8_t enable = 0b00001000;
		uint8_t clockPoint = 0;
		void start(void);
		void stop(void);
		void sendByte(uint8_t Data);
};

#endif
