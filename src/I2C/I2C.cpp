#include "I2C.hpp"
#include <fcntl.h>

I2CBUS::I2CBUS() //Declaration of constructor
{
    I2CBUS::*jetson_bus = "/dev/i2c-1"
    I2CBUS::address = 0x64
    I2CBUS::image
}

I2CBUS::~I2CBUS() //Declaration of Destructor
{
    
}