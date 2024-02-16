
#include <unistd.h>       // Used for UART
#include <sys/fcntl.h>    // Used for UART
#include <sys/ioctl.h>
#include <termios.h>      // Used for UART
#include <stdio.h>
#include <iostream>
#include "../utils/ThreadLogger.hpp"
#include "../utils/LoggingLevel.hpp"

#define     NSERIAL_CHAR   256
#define     VMINX          1
#define     BAUDRATE       B115200

#ifndef UART_HPP
#define UART_HPP

class UartIO {
    public:
        UartIO(ThreadLogger * logger);
        ~UartIO();
        void sendUart(unsigned char *msg);
        int readUart();
    private:
        int uartId;
        ThreadLogger * logger;
        const char *uart_target = "/dev/ttyTHS1";
};
#endif // UART_HPP