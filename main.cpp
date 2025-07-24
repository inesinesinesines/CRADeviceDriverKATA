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
	FlashMock mockFlash;
	DeviceDriver driver{ &mockFlash };
};

TEST_F(DDFixture, WriteToHW) {

	int data = driver.read(0xFF);
	EXPECT_EQ(0, data);
}

TEST_F(DDFixture, FiveReadFromHWSuccess) {

	unsigned char value = 0xAA;
	EXPECT_CALL(mockFlash, read).Times(5).WillRepeatedly(Return(value));
	unsigned char data = driver.read(0xFF);
	
	EXPECT_EQ(value, data);
}

TEST_F(DDFixture, FiveReadFromHWFail) {

	unsigned char value = 0xAA;
	EXPECT_CALL(mockFlash, read).Times(5).WillOnce(Return(0xAB)).WillRepeatedly(Return(value));

	EXPECT_THROW({ unsigned char data = driver.read(0xFF); }, ReadFailException);
}


int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}