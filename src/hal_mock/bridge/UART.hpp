
#ifndef UARTMOCK_HPP
#define UARTMOCK_HPP

class UartIO {
    public:
        UartIO(ThreadLogger * logger){};
        ~UartIO(){};
        void sendUart(unsigned char *msg){};
        int readUart(){return 0;};

};
#endif // UART_HPP