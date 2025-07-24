#include "gmock/gmock.h"
#include <stdexcept>
#include "device_driver.h"
#include "flash_memory_device.h"

using namespace testing;

class FlashMock : public FlashMemoryDevice {
public: 
	//virtual unsigned char read(long address) = 0;
	//virtual void write(long address, unsigned char data) = 0;
	MOCK_METHOD(unsigned char, read, (long address), (override));
	MOCK_METHOD(void, write, (long address, unsigned char data), (override));
};

class DDFixture : public testing::Test {
public: 
	NiceMock<FlashMock> mockFlash;
	DeviceDriver driver{ &mockFlash };

	const int ADDRESS = 0x0;
	const unsigned char DATA = 0xAA;
	const int ERASE_VALUE = 0xFF;
	const int READ_ASSURANCE_COUNT = 5;
};

TEST_F(DDFixture, WriteToHWNonErasedArea) {

	// when read returns with 0xFF value
	EXPECT_CALL(mockFlash, read).Times(1).WillRepeatedly(Return(DATA));
	// driver can write the value	
	EXPECT_THROW({ driver.write(ADDRESS, DATA); }, WriteFailException);
}

TEST_F(DDFixture, FiveReadFromHWSuccess) {

	EXPECT_CALL(mockFlash, read).Times(READ_ASSURANCE_COUNT).WillRepeatedly(Return(DATA));
	unsigned char data = driver.read(ADDRESS);
	
	EXPECT_EQ(DATA, data);
}

TEST_F(DDFixture, FiveReadFromHWFail) {
	const unsigned char UNEVEN_DATA = 0xAB;
	EXPECT_CALL(mockFlash, read).WillOnce(Return(UNEVEN_DATA)).WillRepeatedly(Return(DATA));

	EXPECT_THROW({ unsigned char data = driver.read(ADDRESS); }, ReadFailException);
}


int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}