#include "UART.hpp"

UartIO::UartIO(ThreadLogger *logger)
{
    UartIO::logger = logger;
    // SETUP SERIAL WORLD

    struct termios port_options; // Create the structure
    int uartId = -1;
    tcgetattr(uartId, &port_options); // Get the current attributes of the Serial port

    //------------------------------------------------
    //  OPEN THE UART
    //------------------------------------------------
    // The flags (defined in fcntl.h):
    //	Access modes (use 1 of these):
    //		O_RDONLY - Open for reading only.
    //		O_RDWR   - Open for reading and writing.
    //		O_WRONLY - Open for writing only.
    //	    O_NDELAY / O_NONBLOCK (same function)
    //               - Enables nonblocking mode. When set read requests on the file can return immediately with a failure status
    //                 if there is no input immediately available (instead of blocking). Likewise, write requests can also return
    //				   immediately with a failure status if the output can't be written immediately.
    //                 Caution: VMIN and VTIME flags are ignored if O_NONBLOCK flag is set.
    //	    O_NOCTTY - When set and path identifies a terminal device, open() shall not cause the terminal device to become the controlling terminal for the process.uartId = open("/dev/ttyTHS1", O_RDWR | O_NOCTTY | O_NDELAY);		//Open in non blocking read/write mode

    uartId = open(uart_target, O_RDWR | O_NOCTTY);

    tcflush(uartId, TCIFLUSH);
    tcflush(uartId, TCIOFLUSH);

    usleep(1000000); // 1 sec delay

    if (uartId == -1)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        logger->log(level, "Unable to open UART %s, try running as sudo", uart_target);
    }

    //------------------------------------------------
    // CONFIGURE THE UART
    //------------------------------------------------
    // flags defined in /usr/include/termios.h - see http://pubs.opengroup.org/onlinepubs/007908799/xsh/termios.h.html
    //	Baud rate:
    //         - B1200, B2400, B4800, B9600, B19200, B38400, B57600, B115200,
    //           B230400, B460800, B500000, B576000, B921600, B1000000, B1152000,
    //           B1500000, B2000000, B2500000, B3000000, B3500000, B4000000
    //	CSIZE: - CS5, CS6, CS7, CS8
    //	CLOCAL - Ignore modem status lines
    //	CREAD  - Enable receiver
    //	IGNPAR = Ignore characters with parity errors
    //	ICRNL  - Map CR to NL on input (Use for ASCII comms where you want to auto correct end of line characters - don't use for bianry comms!)
    //	PARENB - Parity enable
    //	PARODD - Odd parity (else even)

    port_options.c_cflag &= ~PARENB;                         // Disables the Parity Enable bit(PARENB),So No Parity
    port_options.c_cflag &= ~CSTOPB;                         // CSTOPB = 2 Stop bits,here it is cleared so 1 Stop bit
    port_options.c_cflag &= ~CSIZE;                          // Clears the mask for setting the data size
    port_options.c_cflag |= CS8;                             // Set the data bits = 8
    port_options.c_cflag &= ~CRTSCTS;                        // No Hardware flow Control
    port_options.c_cflag |= CREAD | CLOCAL;                  // Enable receiver,Ignore Modem Control lines
    port_options.c_iflag &= ~(IXON | IXOFF | IXANY);         // Disable XON/XOFF flow control both input & output
    port_options.c_iflag &= ~(ICANON | ECHO | ECHOE | ISIG); // Non Cannonical mode
    port_options.c_oflag &= ~OPOST;                          // No Output Processing

    port_options.c_lflag = 0; //  enable raw input instead of canonical,

    port_options.c_cc[VMIN] = VMINX; // Read at least 1 character
    port_options.c_cc[VTIME] = 0;    // Wait indefinetly

    cfsetispeed(&port_options, BAUDRATE); // Set Read  Speed
    cfsetospeed(&port_options, BAUDRATE); // Set Write Speed

    // Set the attributes to the termios structure
    int att = tcsetattr(uartId, TCSANOW, &port_options);

    if (att != 0)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        logger->log(level, "Unable to set UART %s attributes", uart_target);
        
    }
    else
    {
        logger->log("Serial port initialized.");
     
    }

    // Flush Buffers
    tcflush(uartId, TCIFLUSH);
    tcflush(uartId, TCIOFLUSH);

    usleep(500000); // 0.5 sec delay
}

UartIO::~UartIO()
{
    close(uartId);
}

void UartIO::sendUart(unsigned char *msg)
{
    unsigned char tx_buffer[20];

    int count = 0;
    while (msg[count] != '\0')
    {
        tx_buffer[count] = msg[count];
        count++;
    }

    tx_buffer[count] = '#';
    count++;

    if (uartId == -1)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        logger->log(level, "UART TX error on target %s", uart_target);
        return;
    }

    int n = write(uartId, tx_buffer, count);
    if (n < 0)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        logger->log(level, "UART TX error on target %s", uart_target);
        return;
    }
    else
    {
        logger->log("UART TX: %s", msg);
    }
}

int UartIO::readUart()
{

    unsigned char rx_buffer[VMINX];

    int rx_length;

    if (uartId == -1)
    {
        LoggingLevelWrapper level(LoggingLevel::ERROR);
        logger->log(level, "UART RX error on target %s", uart_target);
        
        return 0;
    }

    char message[NSERIAL_CHAR];
    for (int ii = 0; ii < NSERIAL_CHAR; ii++)
    {
        message[ii] = ' ';
    }

    int bytesAvailable;
    ioctl(uartId, FIONREAD, &bytesAvailable);

    if (bytesAvailable > 0)
    {
        rx_length = read(uartId, (void *)rx_buffer, VMINX); // Filestream, buffer to store in, number of bytes to read (max)

        if (rx_length < 0)
        {
            // An error occured (will occur if there are no bytes)
        }

        if (rx_length == 0)
        {
            // No data waiting
        }

        if (rx_length >= 0)
        {
            int i = 0;
            bool endstop = false;
            while (i < bytesAvailable && !endstop)
            {

                message[i] = rx_buffer[0]; // Build message 1 character at a time
                
                rx_length = read(uartId, (void *)rx_buffer, VMINX);
                if (rx_buffer[0] == '#')
                {
                    endstop = true;
                }
                i++;
            }
            
            logger->log("UART RX: %c", message[0]);
            tcflush(uartId, TCIOFLUSH);
            // std::cout << "UART RX: " << message << std::endl;
            return message[0];
        }
    }
    return 0;
}
