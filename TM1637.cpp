#include <TM1637.h>

const uint8_t TubeTab[] =	{0b00111111,	//0
							 0b00000110,	//1
							 0b01011011,	//2
							 0b01001111,	//3
							 0b01100110,	//4
							 0b01101101,	//5
							 0b01111101,	//6
							 0b00000111,	//7
							 0b01111111,	//8
							 0b01101111,	//9
							 0b01000000,	//-
							 };
                           
void TM1637::init(uint8_t Data, uint8_t Clk) {
	Datapin = Data;
	Clkpin = Clk;
	pinMode(Datapin, OUTPUT);
	pinMode(Clkpin, OUTPUT);
}

void TM1637::setBrightness(uint8_t data) {
	if(data>0||data<8) brightness = data;
}

void TM1637::enableDisplay(bool data) {
	if(data) enable = 0b00001000;
	else enable = 0b00000000;
}

void TM1637::enableClockPoint(bool data) {
	if(data)clockPoint=0b10000000;
	else clockPoint=0;
}

void TM1637::reverseClockPoint(void) {
	if(clockPoint==0b10000000) clockPoint=0;
	else clockPoint=0b10000000;
}

void TM1637::start() {
	digitalWrite(Datapin,HIGH);
	digitalWrite(Datapin,LOW); 
	digitalWrite(Clkpin,LOW); 
}

void TM1637::stop() {
	digitalWrite(Clkpin,LOW);
	digitalWrite(Datapin,LOW);
	digitalWrite(Clkpin,HIGH);
	digitalWrite(Datapin,HIGH); 
}

void TM1637::sendByte(uint8_t data) {
	data|=clockPoint;
	for(uint8_t i=0;i<8;i++) {
		digitalWrite(Clkpin, LOW);
		if(data&0x01) digitalWrite(Datapin, HIGH);
		else digitalWrite(Datapin, LOW);
		digitalWrite(Clkpin, HIGH);
		data >>= 1;
	}
	digitalWrite(Clkpin, LOW);
	digitalWrite(Datapin,HIGH); 
	digitalWrite(Clkpin, HIGH);
	pinMode(Datapin,INPUT);
	delayMicroseconds(10);
	while(digitalRead(Datapin)) {
	}
	pinMode(Datapin,OUTPUT);
	digitalWrite(Datapin,LOW);
	delayMicroseconds(10);
	pinMode(Datapin,OUTPUT);
	delayMicroseconds(10);
}

void TM1637::writeSelected(uint8_t disp, uint8_t data, bool raw) {
	if(!raw) {
		if(data<=9) {
			data=TubeTab[data];
		}
		else return;
	}
	start();
	sendByte(SETTINGS|FIX_ADDR);
	stop();
	start();
	sendByte(ADDRESS|disp);
	sendByte(data);
	stop();
	start();
	sendByte(CONTROL|brightness|enable);
	stop();
}

void TM1637::writeFull(short int data) {
	uint32_t tmp = 0;
	if(data<-999||data>9999) return;
	if(data<0) {
		tmp=TubeTab[10];
		tmp<<=8;
		data=data-data-data;
	}
	else {
		tmp=TubeTab[data/1000];
		data%=1000;
		tmp<<=8;
	}
	tmp|=TubeTab[data/100];
	data%=100;
	tmp<<=8;
	tmp|=TubeTab[data/10];
	data%=10;
	tmp<<=8;
	tmp|=TubeTab[data];
	writeFullRaw(tmp);
}

void TM1637::writeFullRaw(uint32_t data) {
	start();
	sendByte(SETTINGS|AUTO_ADDR);
	stop();
	start();
	sendByte(ADDRESS);
	sendByte(data>>24);
	sendByte(data>>16|clockPoint);
	sendByte(data>>8);
	sendByte(data);
	stop();
	start();
	sendByte(CONTROL|brightness|enable);
	stop();
}

void TM1637::clearDisplay(void) {
	writeFullRaw(0);
}
