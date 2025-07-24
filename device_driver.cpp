#include "device_driver.h"
#include <stdexcept>
#include <set>


DeviceDriver::DeviceDriver(FlashMemoryDevice* hardware) : m_hardware(hardware)
{
}

int DeviceDriver::read(long address)
{
    std::set<int> data;
    for (int i = 0; i < 5; i++) {
        data.insert((int)(m_hardware->read(address)));
    }
    
    if (data.size() > 1) throw ReadFailException();
    return *data.begin();
}

void DeviceDriver::write(long address, int data)
{
    // TODO: implement this method
    m_hardware->write(address, (unsigned char)data);
}