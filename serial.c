// Standard C library
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

// POSIX includes
#include <fcntl.h>
#include <unistd.h>
#include <termios.h>

// Module include
#include "serial.h"

struct serial
{
	// Serial Device
	char * dev_name;
	int    dev_fd;
	struct termios options;

	// Baudrate
	int	baud_rate, baud_code;

	// Character size
	int	size;
	// Character size may be 5, 6, 7 or 8 bits

	// Parity
	int	parity;
	// Parity algorithm can be:
	// 0 - no parity (can be interpreted as mark parity, i.e. parity is '1')
	// 1 - odd parity
	// 2 - even parity
	// 3 - space parity (parity always '0')

	// Carrier detect / Ring control and status
	char carrier_detect;

	// Flow control
	char read_all;
	// Read all requested bytes, i.e. delay return until they are all
	// available.
	// If 0 no delay (FNDELAY) is done and read is returned uncompleted,
	// including having read 0 bytes.
	// If 1 delay is introduced if all requested bytes is not in the buffer
	// and the read waits for the remaining bytes, or possibly time-out.

	char hardware_flowctrl;
	// Enable of Clear To Send (CTS) and Ready To Send (RTS) hardware
	// supported flow control.
	// If 0 no hardware flow control handshake take place
	// If 1 hardware flow control is used

	char software_flowctrl;
	// Enable of XON and XOFF software flow control.
	// If 0 no software flow control handshake take place
	// If 1 software flow control is used

	// Input processing
	char canonical;
	// Allows editing on the input line buffer when enabled.
	// If 0 the input buffer is in raw mode.
	// If 1 the input buffer is in canonical mode.

	char input_cr;
	// Allows for steering of carrige return
	// If 0 no carrige return filtering is done (raw mode).
	// If 1 carrige return is ignored.
	// If 2 carrige return is mapped to new line.

	int timeout_ms;

	// Debug modes

	char wr_hex;
	// Allows for write hexdump
	// If 0 no hexdump of write
	// If 1 hexdump of write

	char rd_hex;
	// Allows for read hexdump
	// If 0 no hexdump of read
	// If 1 hexdump of read
};

#define BAUDCODES 16
int	baud_rates[BAUDCODES] =
{
	50, 75, 110, 134, 150, 200, 300, 600, 1200, 1800,
	2400, 4800, 9600, 19200, 38400, 57600 //, 76800, 115200
};
int	baud_codes[BAUDCODES] =
{
	B50, B75, B110, B134, B150, B200, B300, B600, B1200, B1800,
	B2400, B4800, B9600, B19200, B38400, B57600 //, B76800, B115200
};

serial * serial_create()
{
	serial * s;
	s = (serial *)malloc(sizeof(serial));
	s->dev_name = NULL;
	s->dev_fd = -1;
	return s;
}

void serial_free(serial * s)
{
	if (s->dev_fd >= 0)
		serial_close(s);
	free(s);
}

int	serial_find_baudcode(int baudrate)
{
	int i;
	for (i = 0; i < BAUDCODES; i++)
		if (baud_rates[i] == baudrate)
			return baud_codes[i];
	return -1;
}

int	serial_find_baudrate(int baudcode)
{
	int i;
	for (i = 0; i < BAUDCODES; i++)
		if (baud_codes[i] == baudcode)
			return baud_rates[i];
	return -1;
}

serial * serial_set_baudrate(serial * s, int baudrate)
{
	int baudcode;

	baudcode = serial_find_baudcode(baudrate);
	if (baudcode < 0)
		return NULL;
	tcgetattr(s->dev_fd, &s->options);
	cfsetispeed(&s->options, baudcode);
	cfsetospeed(&s->options, baudcode);
	s->baud_rate = baudrate;
	s->baud_code = baudcode;
	s->options.c_cflag |= (CLOCAL | CREAD);
	tcsetattr(s->dev_fd, TCSANOW, &s->options);
	return s;
}

int	serial_get_baudrate(serial * s)
{
	return s->baud_rate;
}

serial * serial_set_size(serial * s, int size)
{
	if (size < 5)
		return NULL;
	if (size > 8)
		return NULL;
	tcgetattr(s->dev_fd, &s->options);
	s->options.c_cflag &= ~CSIZE;
	switch (size)
	{
		case 5:
			s->options.c_cflag |= CS5;
			break;
		case 6:
			s->options.c_cflag |= CS6;
			break;
		case 7:
			s->options.c_cflag |= CS7;
			break;
		case 8:
			s->options.c_cflag |= CS8;
			break;
		default:
			break;
	}
	tcsetattr(s->dev_fd, TCSANOW, &s->options);
	s->size = size;
	return s;
}

int	serial_get_size(serial * s)
{
	return s->size;
}

serial * serial_set_parity(serial * s, int parity)
{
	if (parity < 0)
		return NULL;
	if (parity > 3)
		return NULL;
	tcgetattr(s->dev_fd, &s->options);
	switch (parity)
	{
		case 0:	// No parity
			s->options.c_cflag &= ~PARENB;
			s->options.c_cflag &= ~CSTOPB;
			break;
		case 1: // Odd parity
			s->options.c_cflag |= PARENB;
			s->options.c_cflag |= PARODD;
			s->options.c_cflag &= ~CSTOPB;
			break;
		case 2: // Even parity
			s->options.c_cflag |= PARENB;
			s->options.c_cflag &= ~PARODD;
			s->options.c_cflag &= ~CSTOPB;
			break;
		case 3: // Space parity - 7 bit enforced
			s->options.c_cflag &= ~PARENB;
			s->options.c_cflag &= ~CSTOPB;
			s->options.c_cflag &= ~CSIZE;
			s->options.c_cflag |= CS8;
			break;
		default:
			break;
	}
	tcsetattr(s->dev_fd, TCSANOW, &s->options);
	s->parity = parity;
	return s;
}

int	serial_get_parity(serial * s)
{
	return s->parity;
}

serial * serial_set_carrier_detect(serial * s, char state)
{
	if (state == 0)
		s->carrier_detect = 0;
	else
		s->carrier_detect = 1;
	return s;
}

char	serial_get_carrier_detect(serial * s)
{
	return s->carrier_detect;
}

serial * serial_set_read_all(serial * s, char state)
{
	if (state == 0)
	{
		s->read_all = 0,
		fcntl(s->dev_fd, F_SETFL, FNDELAY);
	}
	else
	{
		s->read_all = 1;
		fcntl(s->dev_fd, F_SETFL, 0);
	}
	return s;
}

char	serial_get_read_all(serial * s)
{
	return s->read_all;
}

serial * serial_set_hardware_flowctrl(serial * s, char state)
{
	tcgetattr(s->dev_fd, &s->options);
	if (state == 0)
	{
		s->hardware_flowctrl = 0;
		s->options.c_cflag &= ~CRTSCTS;
	}
	else
	{
		s->hardware_flowctrl = 1;
		s->options.c_cflag |= CRTSCTS;
	}
	tcsetattr(s->dev_fd, TCSANOW, &s->options);
	return s;
}

char	serial_get_hardware_flowctrl(serial * s)
{
	return s->hardware_flowctrl;
}

serial * serial_set_software_flowctrl(serial * s, char state)
{
	tcgetattr(s->dev_fd, &s->options);
	if (state == 0)
	{
		s->software_flowctrl = 0;
		s->options.c_iflag &= ~(IXON | IXOFF | IXANY);
	}
	else
	{
		s->software_flowctrl = 1;
		s->options.c_iflag |= (IXON | IXOFF | IXANY);
	}
	tcsetattr(s->dev_fd, TCSANOW, &s->options);
	return s;
}

char	serial_get_software_flowctrl(serial * s)
{
	return s->software_flowctrl;
}

serial * serial_set_canonical(serial * s, char state)
{
	tcgetattr(s->dev_fd, &s->options);
	if (state == 0)
	{
		s->canonical = 0;
		s->options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG);
	}
	else if (state == 1)
	{
		s->canonical = 1;
		s->options.c_lflag |= (ICANON | ECHO | ECHOE);
	}
	else
	{
		s->canonical = 2;
		s->options.c_lflag &= ~(ECHO | ECHOE);
		s->options.c_lflag |= (ICANON);
	}
	tcsetattr(s->dev_fd, TCSANOW, &s->options);
	return s;
}

char	serial_get_canonical(serial * s)
{
	return s->canonical;
}

serial * serial_set_min0timeout(serial * s, int timeout)
{
	tcgetattr(s->dev_fd, &s->options);
	s->timeout_ms = timeout;
	s->options.c_cc[VMIN] = 0;
	s->options.c_cc[VTIME] = timeout / 100;
	tcsetattr(s->dev_fd, TCSANOW, &s->options);
	return s;
}

int	serial_get_timeout_ms(serial * s)
{
	return s->timeout_ms;
}

serial * serial_set_input_cr(serial * s, char state)
{
	tcgetattr(s->dev_fd, &s->options);
	if (state == 0)
	{
		s->input_cr = 0;
		s->options.c_iflag &= ~(IGNCR | ICRNL);
	}
	else if (state == 1)
	{
		s->input_cr = 1;
		s->options.c_iflag &= ~(ICRNL);
		s->options.c_iflag |= (IGNCR);
	}
	else
	{
		s->input_cr = 2;
		s->options.c_iflag &= ~(IGNCR);
		s->options.c_iflag |= (ICRNL);
	}
	tcsetattr(s->dev_fd, TCSANOW, &s->options);
	return s;
}

char	serial_get_input_cr(serial * s)
{
	return s->input_cr;
}

serial * serial_set_device(serial * s, char * device_name)
{
	s->dev_name = device_name;
	return s;
}

char	serial_get_wr_hex(serial * s)
{
	return s->wr_hex;
}

serial * serial_set_wr_hex(serial * s, char wr_hex)
{
	s->wr_hex = wr_hex;
	return s;
}

char	serial_get_rd_hex(serial * s)
{
	return s->rd_hex;
}

serial * serial_set_rd_hex(serial * s, char rd_hex)
{
	s->rd_hex = rd_hex;
	return s;
}

serial * serial_open(serial * s)
{
	int flags;

	flags = O_RDWR | O_NOCTTY;
	if (s->carrier_detect == 0)
		flags |= O_NDELAY;
	s->dev_fd = open(s->dev_name, flags);
	if (s->dev_fd == -1)
	{
		fprintf(stderr, "%s: Unable to open serial device %s\n", __func__, s->dev_name);
	}
	else
	{
		fcntl(s->dev_fd, F_SETFL, 0);
		s->read_all = 0;
	}
	s->wr_hex = 0;
	s->rd_hex = 0;

	return s;
}

serial * serial_close(serial * s)
{
	close(s->dev_fd);
	s->dev_fd = -1;
	return s;
}

int	serial_hexdump(char * prefix, char * str, int len)
{
	int i, j;
	for (i = 0; i < len; i+=16)
	{
		printf("%s %04x : ", prefix, i);
		for (j = 0; j < 16; j++)
		{
			if (i+j < len)
			{
				if (str[i+j] < 32)
					printf(".");
				else
					printf("%c", str[i+j]);
			}
			else
				printf(" ");
		}
		for (j = 0; j < 16; j++)
		{
			if ((j & 3) == 0)
				printf(" ");
			if (i+j < len)
				printf("%02x ", str[i+j]);
		}
		printf("\n");
	}
	return 0;
}

serial * serial_write_binary(serial * s, char * str, int len)
{
	int n;

	if (s->wr_hex)
		serial_hexdump("wr ", str, len);
	n = write(s->dev_fd, str, len);
	if (n < 0)
	{
		fprintf(stderr, "%s: Failed to write %i bytes.", __func__, len);
	}
	return s;
}

serial * serial_write_string(serial * s, char * str)
{
	int len, n;

	len = strlen(str);
	if (s->wr_hex)
		serial_hexdump("wr ", str, len);
	n = write(s->dev_fd, str, len);
	if (n < 0)
	{
		fprintf(stderr, "%s: Failed to write %i bytes.", __func__, len);
	}
	return s;
}

int	serial_read(serial * s, char * str, int len)
{
	int n;
	n = read(s->dev_fd, str, len);
	if (n < 0)
	{
		fprintf(stderr, "%s: ERROR %u Failed to read %i bytes.\n", __func__, n, len);
	}
	if (s->rd_hex)
		serial_hexdump("rd ", str, n);
	return n;
}
