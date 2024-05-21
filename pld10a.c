// Standard C library
#include <stdio.h>

// Library includes
#include "prologix.h"

int main()
{
	prologix * p;
	int i;
	char str[161];
	char * s = (char *)str;
	int count;

	p = prologix_create();
	prologix_open(p, "/dev/ttyUSB0");
	prologix_set_debug(p,1);
	prologix_set_read_tmo_ms(p, 200);
	prologix_set_auto(p, 0);

	// 488.2 compatibility - EOI and terminating 0a
	prologix_set_eoi(p, 1);
	prologix_set_eos(p, 2);

	// Clean-read interface
	count = prologix_read(p, 0, s, 160);

	i = 5;
	prologix_write(p, i, "*IDN?\n");
	count = prologix_read(p, i, s, 160);
	if (count)
		printf("%2i: %s", i, s);
	//prologix_llo(p, i);

	// MODE PAR - Parallel mode - 312.5 ps
	//prologix_write(p, i, "MODE 312.5ps\n");
	// MODE SER - Serial mode - 625 ps
	prologix_write(p, i, "MODE 625ps\n");

	prologix_write(p, i, "MODE?\n");
	count = prologix_read(p, i, s, 160);
	if (count)
		printf("%2i: %s", i, s);
	//prologix_write(p, i, "*RST\n");
	prologix_write(p, i, "MON?\n");
	count = prologix_read(p, i, s, 160);
	if (count)
		printf("%2i: %s", i, s);
	prologix_write(p, i, "DEL?\n");
	count = prologix_read(p, i, s, 160);
	if (count)
		printf("%2i: %s", i, s);
	prologix_write(p, i, "DEL 123ps\n");
	prologix_write(p, i, "DEL?\n");
	count = prologix_read(p, i, s, 160);
	if (count)
		printf("%2i: %s", i, s);
	prologix_write(p, i, "*STB?\n");
	count = prologix_read(p, i, s, 160);
	if (count)
		printf("%2i: %s", i, s);
	else
		printf("%2i: #NOTHING#\n", i);
	prologix_write(p, i, "ERR?\n");
	count = prologix_read(p, i, s, 160);
	if (count)
		printf("%2i: %s", i, s);	
	prologix_free(p);
	return 0;
}
