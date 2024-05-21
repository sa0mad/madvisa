// Standard includes
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Project includes
#include "serial.h"

// Module include
#include "prologix.h"

struct prologix
{
	// Serial interface
	serial *	s;

	// Prologix configuration cache
	int		addr;
	int		addr2;
	int		autoval;
	int		eoi; 
	int		eos;
	int		eot_enable;
	char		eot_char;
	int		lon;
	int		mode;
	int		read_tmo_ms;
	int		savecfg;
	int		status;
	char *		ver;

	// TX excape buffer
	char *		tx_escape_buffer;
	int		tx_escape_buffer_len;
};

prologix * prologix_create()
{
	prologix * p;

	p = (prologix *)malloc(sizeof(prologix));
	if (p == NULL)
		return NULL;
	p->s = serial_create();
	p->eoi = 0;
	p->tx_escape_buffer = (char *)malloc(sizeof(char)*8192);
	if (p->tx_escape_buffer == NULL)
	{
		fprintf(stderr, "%s: Unable to allocate tx_escape_buffer\n", __func__);
		prologix_free(p);
		return NULL;
	}

	p->addr = -1;
	p->addr2 = -1;
	p->ver = NULL;

	return p;
}

void	prologix_free(prologix * p)
{
	free(p->tx_escape_buffer);
	p->tx_escape_buffer = NULL;
	serial_close(p->s);
	serial_free(p->s);
	free(p);
}

prologix * prologix_open(prologix * p, char * device)
{
	serial * s = p->s;
	serial_set_carrier_detect(s, 1);
	serial_set_device(s, device);
	serial_open(s);
	serial_set_baudrate(s, 57600);
	serial_set_size(s, 8);
	serial_set_parity(s, 0);
	serial_set_read_all(s, 1);
	serial_set_hardware_flowctrl(s, 1);
	serial_set_software_flowctrl(s, 0);
	serial_set_canonical(s, 0);
	serial_set_min0timeout(s, 1000);
	serial_set_input_cr(s, 0);
	prologix_set_debug(p, 0);
	prologix_query_all(p);
	prologix_set_auto(p, 1);
	prologix_set_eoi(p, 1);
	prologix_set_eos(p, 0);
	prologix_set_eot_enable(p, 0);
	prologix_set_mode(p, 1);
	prologix_set_read_tmo_ms(p, 1000);
	prologix_set_savecfg(p, 0);
	return 0;
}

int prologix_tx_escape(prologix * p, char * str, int len)
{
	int i, tx;
	for (i=0, tx=0; i<len; i++)
	{
		switch (str[i])
		{
		case 10:
		case 13:
		case 27:
		case 43:
			p->tx_escape_buffer[tx++] = 27;
		default:
			p->tx_escape_buffer[tx++] = str[i];
			break;
		}
	}
	p->tx_escape_buffer_len = tx;
	return tx;
}

int prologix_write_binary(prologix * p, unsigned char addr, char * str, int len)
{
	prologix_set_addr(p, addr);
	prologix_tx_escape(p, str, len);
	serial_write_binary(p->s, p->tx_escape_buffer, p->tx_escape_buffer_len);
	return 0;
}

int prologix_write_str(prologix * p, unsigned char addr, char * str)
{
	int len;
	prologix_set_addr(p, addr);
	len = strlen(str);
	prologix_tx_escape(p, str, len);
	serial_write_binary(p->s, p->tx_escape_buffer, p->tx_escape_buffer_len);
	return 0;
}

int prologix_write(prologix * p, unsigned char addr, char * str)
{
	prologix_set_addr(p, addr);
	serial_write_string(p->s, str);
	return 0;
}

int prologix_read(prologix * p, unsigned char addr, char * str, int len)
{
	int i;
	int c, count;
	char * s;
	for (i = 0; i < len; i++) str[i] = 0;
	prologix_set_addr(p, addr);
	if (p->eoi)
		serial_write_string(p->s, "++read eoi\n");
	else
		serial_write_string(p->s, "++read 10");
	count = 0;
	s = str;
	do
	{
		do
		{
			c = serial_read(p->s, s, len);
			s += c;
			count += c;
		} while (c > 0);
	} while ((count == 0) || ((count > 0) && (str[count-1] != 10)));
	return count;
}

// Prologix command write and read

int prologix_command(prologix * p, char * command)
{
	serial_write_string(p->s, command);
	return 0;
}

int prologix_set(prologix * p, char * command, int val)
{
	char str[16];
	char * s = (char *)&str;
	int len;

	len = strlen(command);
	strncpy(s, command, 16);
	s[len - 2] = '0' + val;
	serial_write_string(p->s, s);
	return len;
}

int prologix_query(prologix * p, char * command)
{
	char str[6];
	char * s = (char *)&str;
	serial_write_string(p->s, command);
	serial_read(p->s, s, 6);
	return atoi(s);
}

char *prologix_query_str(prologix * p, char * command)
{
	char str[81];
	char * s = (char *)&str;
	char * news;
	int len;
	serial_write_string(p->s, command);
	len = serial_read(p->s, s, 80);
	news = (char *)malloc(sizeof(char)*(len+1));
	strncpy(news,s,len);
	return news;
}

// Prologix protocol settings

int prologix_set_debug(prologix * p, int debug)
{
	serial_set_rd_hex(p->s, debug);
	serial_set_wr_hex(p->s, debug);
	return 0;
}

int prologix_set_addr(prologix * p, int addr)
{
	char addrstr[11];
	char * s = (char *)&addrstr;
	if ((addr < 0) || (addr > 30))
		return -1;
	if ((p->addr == addr) && (p->addr2 == -1))
		return 0;
	p->addr = addr;
	p->addr2 = -1;
	snprintf(s, 11, "++addr %u\n", addr);
	prologix_command(p, s);
	return 0;
}

int prologix_set_addr2(prologix * p, int addr, int addr2)
{
	char addrstr[30];
	char * s = (char *)&addrstr;
	if ((addr < 0) || (addr > 30))
		return -1;
	if ((addr2 < 96) || (addr2 > 126))
		return -1;
	if ((p->addr == addr) && (p->addr2 == addr2))
		return 0;
	p->addr = addr;
	p->addr2 = addr2;
	snprintf(s, 30, "++addr %u %u\n", addr, addr2);
	prologix_command(p, s);
	return 0;
}

int prologix_get_addr(prologix * p)
{
	return p->addr;
}

int prologix_get_addr2(prologix * p)
{
	return p->addr2;
}

int prologix_query_addr(prologix * p)
{
	char str[6];
	char * s = (char *)&str;
	char * n = s;
	serial_write_string(p->s, "++addr\n");
	serial_read(p->s, s, 6);
	p->addr = strtol(s, &n, 10);
	p->addr2 = -1;
	if ((p->addr < 0) || (p->addr > 30))
		return -1;
	if (*n == 0)
		return p->addr;
	if (*n == 13)
		return p->addr;
	n++;
	p->addr2 = strtol(n, &n, 10);
	if ((p->addr < 96) || (p->addr > 126))
		return -1;
	return p->addr;
}

int prologix_set_auto(prologix * p, int autoval)
{
	if ((autoval < 0) || (autoval > 1))
		return -1;
	if (p->autoval == autoval)
		return 0;
	p->autoval = autoval;
	prologix_set(p, "++auto 0\n", autoval);
	return 0;
}

int prologix_get_auto(prologix * p)
{
	return p->autoval;
}

int prologix_query_auto(prologix * p)
{
	p->autoval = prologix_query(p, "++auto\n");
	return p->autoval;
}

int prologix_set_eoi(prologix * p, int eoi)
{
	if ((eoi < 0) || (eoi > 1))
		return -1;
	if (p->eoi == eoi)
		return 0;
	p->eoi = eoi;
	prologix_set(p, "++eoi 0\n", eoi);
	return 0;
}

int prologix_get_eoi(prologix * p)
{
	return p->eoi;
}

int prologix_query_eoi(prologix * p)
{
	p->eoi = prologix_query(p, "++eoi\n");
	return p->eoi;
}

int prologix_set_eos(prologix * p, int eos)
{
	if ((eos < 0) || (eos > 3))
		return -1;
	if (p->eos == eos)
		return 0;
	p->eos = eos;
	prologix_set(p, "++eos 0\n", eos);
	return 0;
}

int prologix_get_eos(prologix * p)
{
	return p->eos;
}

int prologix_query_eos(prologix * p)
{
	p->eos = prologix_query(p, "++eos\n");
	return p->eos;
}

int prologix_set_eot_enable(prologix * p, int eot_enable)
{
	if ((eot_enable < 0) || (eot_enable > 1))
		return -1;
	if (p->eot_enable == eot_enable)
		return 0;
	p->eot_enable = eot_enable;
	prologix_set(p, "++eot_enable 0\n", eot_enable);
	return 0;
}

int prologix_get_eot_enable(prologix * p)
{
	return p->eot_enable;
}

int prologix_query_eot_enable(prologix * p)
{
	p->eot_enable = prologix_query(p, "++eot_enable\n");
	return p->eot_enable;
}

int prologix_set_eot_char(prologix * p, char eot_char)
{
	char str[12];
	char * s = (char *)&str;
	if (p->eot_char == eot_char)
		return 0;	
	p->eot_char = eot_char;
	snprintf(s,12,"++eot %i\n",eot_char);
	prologix_command(p, s);
	return 0;
}

int prologix_get_eot_char(prologix * p)
{
	return p->eot_char;
}

int prologix_query_eot_char(prologix * p)
{
	p->eot_char = prologix_query(p, "++eot_char\n");
	return p->eot_char;
}

int prologix_set_lon(prologix * p, int lon)
{
	if ((lon < 0) || (lon > 1))
		return -1;
	if (p->lon == lon)
		return 0;
	p->lon = lon;
	prologix_set(p, "++lon 0\n", lon);
	return 0;
}

int prologix_get_lon(prologix * p)
{
	return p->lon;
}

int prologix_query_lon(prologix * p)
{
	p->lon = prologix_query(p, "++lon\n");
	return p->lon;
}

int prologix_set_mode(prologix * p, int mode)
{
	if ((mode < 0) || (mode > 1))
		return -1;
	if (p->mode == mode)
		return 0;
	p->mode = mode;
	prologix_set(p, "++mode 0\n", mode);
	return 0;
}

int prologix_get_mode(prologix * p)
{
	return p->mode;
}

int prologix_query_mode(prologix * p)
{
	p->mode = prologix_query(p, "++mode\n");
	return p->mode;
}

int prologix_set_read_tmo_ms(prologix * p, int read_tmo_ms)
{
	char str[27];
	char * s = (char *)&str;
	if (p->read_tmo_ms == read_tmo_ms)
		return 0;
	p->read_tmo_ms = read_tmo_ms;
	snprintf(s,27,"++read_tmo_ms %i\n",read_tmo_ms);
	prologix_command(p, s);
	serial_set_min0timeout(p->s, read_tmo_ms);
	return 0;
}

int prologix_get_read_tmo_ms(prologix * p)
{
	return p->read_tmo_ms;
}

int prologix_query_read_tmo_ms(prologix * p)
{
	p->read_tmo_ms = prologix_query(p, "++read_tmo_ms\n");
	return p->read_tmo_ms;
}

int prologix_set_savecfg(prologix * p, int savecfg)
{
	if ((savecfg < 0) || (savecfg > 1))
		return -1;
	if (p->savecfg == savecfg)
		return 0;
	p->savecfg = savecfg;
	prologix_set(p, "++savecfg 0\n", savecfg);
	return 0;
}

int prologix_get_savecfg(prologix * p)
{
	return p->savecfg;
}

int prologix_query_savecfg(prologix * p)
{
	p->savecfg = prologix_query(p, "++savecfg\n");
	return p->savecfg;
}

int prologix_set_status(prologix * p, int status)
{
	if ((status < 0) || (status > 255))
		return -1;
	char str[22];
	char * s = (char *)&str;
	if (p->status == status)
		return 0;
	p->status = status;
	snprintf(s,22,"++status %i\n",status);
	prologix_command(p, s);
	return 0;
}

int prologix_get_status(prologix * p)
{
	return p->status;
}

int prologix_query_status(prologix * p)
{
	p->status = prologix_query(p, "++status\n");
	return p->status;
}

char * prologix_get_ver(prologix * p)
{
	return p->ver;
}

char * prologix_query_ver(prologix * p)
{
	if (p->ver)
		free(p->ver);
	p->ver = prologix_query_str(p, "++ver\n");
	return p->ver;
}

int prologix_query_all(prologix * p)
{
	prologix_query_addr(p);
	prologix_query_auto(p);
	prologix_query_eoi(p);
	prologix_query_eos(p);
	prologix_query_eot_enable(p);
	prologix_query_eot_char(p);
	//prologix_query_lon(p);
	prologix_query_mode(p);
	prologix_query_read_tmo_ms(p);
	prologix_query_savecfg(p);
	prologix_query_status(p);
	prologix_query_ver(p);
	return 0;
}

// GPIB Bus control

int prologix_clr(prologix * p)
{
	return prologix_command(p, "++clr\n");
}

int prologix_ifc(prologix * p)
{
	return prologix_command(p, "++ifc\n");
}

int prologix_llo(prologix * p, int addr)
{
	prologix_set_addr(p, addr);
	return prologix_command(p, "++llo\n");
}

int prologix_loc(prologix * p, int addr)
{
	prologix_set_addr(p, addr);
	return prologix_command(p, "++loc\n");
}

int prologix_rst(prologix * p)
{
	return prologix_command(p, "++rst\n");
}

int prologix_spoll(prologix * p)
{
	return prologix_command(p, "++spoll\n");
}

int prologix_spoll_addr(prologix * p, int addr)
{
	char str[12];
	char *s = (char *)&str;
	snprintf(s, 12, "++spoll %i\n", addr);
	return prologix_command(p, s);
}

int prologix_spoll_addr2(prologix * p, int addr, int addr2)
{
	char str[33];
	char *s = (char *)&str;
	snprintf(s, 33, "++spoll %i %i\n", addr, addr2);
	return prologix_command(p, s);
}

int prologix_srq(prologix * p)
{
	return prologix_query(p, "++srq\n");
}

int prologix_trg(prologix * p, int count, int *addr1v, int *addr2v)
{
	char str[112];
	char * s = (char *)&str;
	char * n = s;
	int i, len;
	if (count == 0)
		return prologix_command(p, "++trg\n");
	len = snprintf(n, 6, "++trg");
	n+=len;
	for (i = 0; i < count; i++)
	{
		len = snprintf(n,4," %i",addr1v[i]);
		n += len;
		if (addr2v[i] > 0)
		{
			len = snprintf(n,5," %i",addr2v[i]);
			n += len;
		}		
	}
	len = snprintf(n,2,"\n");
	n+=len;
	return prologix_command(p, s);
}
