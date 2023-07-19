#include "I2C.hpp"
#include <sys/ioctl.h> //Functions for controlling system requests 
#include <linux/i2c-dev.h> //Built in linux kernel functions for an I2C bus
#include <fcntl.h> //I/O functions for compatibility with ioctl.h
#include <iostream>


I2CBUS::I2CBUS() //Declaration of constructor
{
    I2CBUS::*jetson_bus = "/dev/i2c-1"; //Location of bus pin 
    I2CBUS::address = 0x64; //Slave address of Jetson nano
    I2CBUS::image_file; 
}

I2CBUS::~I2CBUS() //Declaration of Destructor
{
    close(image_file); //Closes file 
}

void I2CBUS::check_image() //Check that the image file has been opened 
{
    if ((image_file = open(jetson_bus, 0_RDWR)) < 0) { //Opening the bus location for Reading and Writing 
        printf("Failed to open jetson bus \n"); //Print statement for failure to open
    }
    else {
        printf("Located jetson bus \n"); //Bus has been located 
    }
}

void I2CBUS::set_slave()  //Set slave device to be read or written from 
{
    if (ioctl(image_file, I2C_SLAVE, address)) { //Check if able to receive data from slave 
        printf("Failed to access slave. \n");
    }
}

void I2CBUS::read_data()
{

}

void I2CBUS::write_data()
{
    
}