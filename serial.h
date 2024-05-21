#ifndef __SERIAL_H
#define __SERIAL_H

typedef struct serial serial;

serial * serial_create();
void serial_free(serial * s);

int	serial_find_baudcode(int baudrate);
int	serial_find_baudrate(int baudcode);
serial * serial_set_baudrate(serial * s, int baudrate);
int	serial_get_baudrate(serial * s);

serial * serial_set_size(serial * s, int size);
int	serial_get_size(serial * s);

serial * serial_set_parity(serial * s, int parity);
int	serial_get_parity(serial * s);

serial * serial_set_carrier_detect(serial * s, char state);
char	serial_get_carrier_detect(serial * s);

serial * serial_set_read_all(serial * s, char state);
char	serial_get_read_all(serial * s);

serial * serial_set_hardware_flowctrl(serial * s, char state);
char	serial_get_hardware_flowctrl(serial * s);

serial * serial_set_software_flowctrl(serial * s, char state);
char	serial_get_software_flowctrl(serial * s);

serial * serial_set_canonical(serial * s, char state);
char	serial_get_canonical(serial * s);

serial * serial_set_min0timeout(serial * s, int timeout);
int	serial_get_timeout_ms(serial * s);

serial * serial_set_input_cr(serial * s, char state);
char	serial_get_input_cr(serial * s);

char	serial_get_wr_hex(serial * s);
serial * serial_set_wr_hex(serial * s, char wr_hex);

char	serial_get_rd_hex(serial * s);
serial * serial_set_rd_hex(serial * s, char rd_hex);

serial * serial_set_device(serial * s, char * device_name);
serial * serial_open(serial * s);
serial * serial_close(serial * s);

serial * serial_write_binary(serial * s, char * str, int len);
serial * serial_write_string(serial * s, char * str);
int	serial_read(serial * s, char * str, int len);

#endif // __SERIAL_H
