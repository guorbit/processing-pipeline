
#include <stdio.h>
#include <sys/ioctl.h> //Functions for controlling system requests 
#include <linux/i2c-dev.h> //Built in linux kernel functions for an I2C bus
#include <fcntl.h>
#include <iostream>

#ifndef I2C_HPP
#define I2C_HPP

class I2CBUS{
private:
	int image_file; //Variable for data 
    char *jetson_bus; 
    unsigned char address;
public:

};

#endif
  git config --global user.email "2470451a@student.gla.ac.uk"
  git config --global user.name "Sean Andrew"
