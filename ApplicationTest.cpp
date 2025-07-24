
#include "gmock/gmock.h"
#include "Application.cpp"
#include <string>
#include <vector>





TEST(ApplicationTest, ReadPrint) {



	Application app(FlashMock);

	app.readAndPrint()

}