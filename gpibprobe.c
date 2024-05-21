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
	prologix_rst(p);
	prologix_set_mode(p,1);
	prologix_set_debug(p,0);
	prologix_set_eoi(p,1);
	prologix_set_read_tmo_ms(p, 1000);
	prologix_set_auto(p, 0);

	// Make controller
	prologix_ifc(p);
	for (i = 0; i <= 30; i++)
	{
		// Clear interface on i
		prologix_set_addr(p, i);
		prologix_clr(p);

		// Query for Identity
		prologix_write(p, i, "*idn?\n");
		count = prologix_read(p, i, s, 160);
		if (count)
			printf("%2i: %s\n", i, s);
		/*
		else
			printf("%2i: <nodevice>\n", i);
		*/
	}
	prologix_free(p);
	return 0;
}
