#ifndef __PROLOGIX_H
#define __PROLOGIX_H

typedef struct	prologix prologix;

// Prologix structure admin
extern prologix * prologix_create();
extern void	prologix_free(prologix * p);

// Prologix open device
extern prologix * prologix_open(prologix * p, char * device);

// Prologix data-transfer
extern int prologix_write_binary(prologix * p, unsigned char addr, char * str, int len);
extern int prologix_write_str(prologix * p, unsigned char addr, char * str);
extern int prologix_write(prologix * p, unsigned char addr, char * str);
extern int prologix_read(prologix * p, unsigned char addr, char * str, int len);

// Prologix configuration

extern int prologix_set_debug(prologix * p, int debug);

extern int prologix_set_addr(prologix * p, int addr);
extern int prologix_set_addr2(prologix * p, int addr, int addr2);
extern int prologix_get_addr(prologix * p);
extern int prologix_get_addr2(prologix * p);
extern int prologix_query_addr(prologix * p);

extern int prologix_set_auto(prologix * p, int autoval);
extern int prologix_get_auto(prologix * p);
extern int prologix_query_auto(prologix * p);

extern int prologix_set_eoi(prologix * p, int eoi);
extern int prologix_get_eoi(prologix * p);
extern int prologix_query_eoi(prologix * p);

extern int prologix_set_eos(prologix * p, int eoi);
extern int prologix_get_eos(prologix * p);
extern int prologix_query_eos(prologix * p);

extern int prologix_set_eot_enable(prologix * p, int eot_enable);
extern int prologix_get_eot_enable(prologix * p);
extern int prologix_query_eot_enable(prologix * p);

extern int prologix_set_eot_char(prologix * p, char eot_char);
extern int prologix_get_eot_char(prologix * p);
extern int prologix_query_eot_char(prologix * p);

extern int prologix_set_lon(prologix * p, int lon);
extern int prologix_get_lon(prologix * p);
extern int prologix_query_lon(prologix * p);

extern int prologix_set_mode(prologix * p, int mode);
extern int prologix_get_mode(prologix * p);
extern int prologix_query_mode(prologix * p);

extern int prologix_set_read_tmo_ms(prologix * p, int read_tmo_ms);
extern int prologix_get_read_tmo_ms(prologix * p);
extern int prologix_query_read_tmo_ms(prologix * p);

extern int prologix_set_savecfg(prologix * p, int savecfg);
extern int prologix_get_savecfg(prologix * p);
extern int prologix_query_savecfg(prologix * p);

extern int prologix_set_status(prologix * p, int status);
extern int prologix_get_status(prologix * p);
extern int prologix_query_status(prologix * p);

extern char * prologix_get_ver(prologix * p);
extern char * prologix_query_ver(prologix * p);

extern int prologix_query_all(prologix * p);

// GPIB bus control

// GPIB Selected Device Clear (SDC)
extern int prologix_clr(prologix * p);

// GPIB Interface Clear (IFC)
extern int prologix_ifc(prologix * p);

// GPIB Local/Remote operation of selected instrument
extern int prologix_llo(prologix * p, int addr);
extern int prologix_loc(prologix * p, int addr);

// GPIB controller reset
extern int prologix_rst(prologix * p);

// GPIB Serial poll
extern int prologix_spoll(prologix * p);
extern int prologix_spoll_addr(prologix * p, int addr);
extern int prologix_spoll_addr2(prologix * p, int addr, int addr2);

// GPIB Serial poll request read
extern int prologix_srq(prologix * p);

// GPIB addressed trigg
extern int prologix_trg(prologix * p, int count, int *addr1v, int *addr2v);

#endif // __PROLOGIX_H
