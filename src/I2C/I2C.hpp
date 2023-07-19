
#include <stdio.h>
#include <sys/ioctl.h> //Functions for controlling system requests 
#include <linux/i2c-dev.h> //Built in linux kernel functions for an I2C bus
#include <fcntl.h>
#include <iostream>

#ifndef I2C_HPP
#define I2C_HPP

class I2CBUS{
private:
    char *jetson_bus; //Bus number dependent on pin
    unsigned char address; //Slave address 8 bit length
    int image_file; //Variable for data 
public:
    I2C(); //Constructor
    ~I2C(); //Destructor
    Check_Image(); //Ensure Image is accessible 

};

#endif