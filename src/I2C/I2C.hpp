
#include <stdio.h>
#include <string>
#include <sys/ioctl.h> //Functions for controlling system requests 
#include <linux/i2c-dev.h> //Built in linux kernel functions for an I2C bus
#include <fcntl.h> //I/O functions for compatibility with ioctl.h
#include <iostream>
#include <unistd.h> //Constants and types for system calls

#ifndef I2C_HPP //Define safeguards
#define I2C_HPP

class I2CBUS{

private:
    std::string jetson_bus; //Bus number dependent on pin
    unsigned char address; //Slave address 8 bit length
    int image_file; //Variable for data 

public:
    I2CBUS(); //Constructor
    ~I2CBUS(); //Destructor
    void check_image(); //Ensure Image is accessible 
    void set_slave(); //Point driver to jetson slave
    void read_data(); //Read data from jetson nano
    void write_data(); //Write data to jetson nano
};

#endif