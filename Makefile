all: serial.o prologix gpibprobe pld10a visatest visa.c.splint visatest.valgrind

clean:
	rm -f *~ serial.o gpibprobe pld10a prologix visatest

serial.o: serial.h serial.c
	gcc -Wall -c serial.c

prologix: serial.h serial.o prologix.c prologix_test.c
	gcc -Wall -o prologix serial.o prologix.c prologix_test.c

gpibprobe: serial.h serial.o prologix.h prologix.c gpibprobe.c
	gcc -Wall -o gpibprobe serial.o prologix.c gpibprobe.c

pld10a: serial.h serial.o prologix.h prologix.c pld10a.c
	gcc -Wall -o pld10a serial.o prologix.c pld10a.c

visatest: visatype.h visa.h visa.c visatest.c
	gcc -Wall -g -o visatest visa.c visatest.c
	./visatest

visa.c.splint: Makefile tb.h visatype.h visa.h visa.c visatest.c
	splint -compdef -compdestroy -temptrans -branchstate -kepttrans -compmempass -onlytrans -varuse -nullstate -observertrans -statictrans -usedef -unqualifiedtrans -must-freeonly visa.c visatest.c > visa.c.splint

visatest.valgrind: Makefile visatest
	valgrind --leak-check=full --show-leak-kinds=all ./visatest > visatest.valgrind
