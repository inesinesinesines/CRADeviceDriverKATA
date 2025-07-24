#include "gmock/gmock.h"
#include <stdexcept>
#include "device_driver.h"
#include "flash_memory_device.h"
#include "Application.cpp"

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



TEST_F(DDFixture, WriteToHWErasedArea) {

	EXPECT_CALL(mockFlash, read).Times(1).WillRepeatedly(Return(ERASE_VALUE));
	EXPECT_NO_THROW({ driver.write(ADDRESS, DATA); });
}

TEST_F(DDFixture, WriteToHWNonErasedArea) {

	EXPECT_CALL(mockFlash, read).Times(1).WillRepeatedly(Return(DATA));
	EXPECT_THROW({ driver.write(ADDRESS, DATA); }, WriteFailException);
}

TEST_F(DDFixture, FiveReadFromHWSuccess) {

	EXPECT_CALL(mockFlash, read).Times(READ_ASSURANCE_COUNT).WillRepeatedly(Return(DATA));	
	EXPECT_EQ(DATA, driver.read(ADDRESS));
}

TEST_F(DDFixture, FiveReadFromHWFail) {
	const unsigned char UNEVEN_DATA = 0xAB;
	EXPECT_CALL(mockFlash, read).WillOnce(Return(UNEVEN_DATA)).WillRepeatedly(Return(DATA));
	EXPECT_THROW({ unsigned char data = driver.read(ADDRESS); }, ReadFailException);
}







TEST(ApplicationTest, ReadPrint) {
	FlashMock flashmock;
	DeviceDriver dd{ &flashmock };
	Application app(&dd);

	EXPECT_CALL(flashmock, read).Times(25);

	app.readAndPrint(0x0, 0x4);

	// 프린트 검증 
	// 1. 내부적으로 래퍼 클래스 반환 (proxy) 
	// 2. console버퍼 옮겨서 확인
	// 3. 행동 검증 

}

TEST(ApplicationTest, WriteAll) {
	NiceMock<FlashMock> flashmock;
	DeviceDriver dd{ &flashmock };
	Application app(&dd);

	EXPECT_CALL(flashmock, read).Times(5).WillRepeatedly(Return((unsigned char)0xff));

	app.writeAll(0x0);

	// 프린트 검증 
	// 1. 내부적으로 래퍼 클래스 반환 (proxy) 
	// 2. console버퍼 옮겨서 확인
	// 3. 행동 검증 
}



int main() {
	::testing::InitGoogleMock();
	return RUN_ALL_TESTS();
}