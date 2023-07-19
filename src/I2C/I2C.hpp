
#include <stdio.h>

#ifndef I2C_HPP //Define safeguards
#define I2C_HPP

class I2CBUS{

private:
    char *jetson_bus; //Bus number dependent on pin
    unsigned char address; //Slave address 8 bit length
    int image_file; //Variable for data 

public:
    I2C(); //Constructor
    ~I2C(); //Destructor
    void check_image(); //Ensure Image is accessible 
    void set_slave(); //Point driver to jetson slave
    void read_data(); //Read data from jetson nano
    void write_data(); //Write data to jetson nano
};

#endif