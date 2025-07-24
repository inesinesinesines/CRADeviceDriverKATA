#include "device_driver.h"
#include <stdexcept>
#include <set>


DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    int readData = (int)(m_hardware->read(address));

    checkReadPostCondition(address, readData);
    
    return readData;
}

void DeviceDriver::checkReadPostCondition(long address, int readData)
{
    for (int i = 0; i < 4; i++) {
        int testData = (int)(m_hardware->read(address));
        if (readData != testData) throw ReadFailException();
    }
}

void DeviceDriver::write(long address, int data)
{
    checkWritePreCondition(address);
    m_hardware->write(address, (unsigned char)data);
}

void DeviceDriver::checkWritePreCondition(long address)
{
    if (0xFF != m_hardware->read(address))
    {
        throw WriteFailException();
    }
}
