#include "device_driver.h"

class Application {
public:
	Application(DeviceDriver* dd) {
		deviceDriver = dd;
	};

	void readAndPrint(long startAddr, long endAddr) {
		for (int addr = startAddr; addr <= endAddr; addr++) {
			deviceDriver->read(addr);
		}
	};

	void writeAll(int value) {
		for (int addr = 0; addr < 5; addr++) {
			deviceDriver->write(addr, value);
		}
	};

private: 
	DeviceDriver* deviceDriver;
};


