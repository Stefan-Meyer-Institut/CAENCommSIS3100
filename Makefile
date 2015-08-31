# Makefile

libdir = ./lib/
libname = CAENComm
staticlib = $(libdir)$(libname).a

objects=$(patsubst %.c,%.o,$(wildcard ./src/*.c))

SISLIBS = /home/hbarhfs/work/sis1100-2.13-7/sis3100_calls/lib_sis3100.a
SISHEADER1 = /home/hbarhfs/work/sis1100-2.13-7/sis3100_calls/
SISHEADER2 = /home/hbarhfs/work/sis1100-2.13-7/
CFLAGS = -Iinclude/  -I$(SISHEADER1) -I$(SISHEADER2) -fPIC
CC=gcc

all: lib

-include $(objects:.o=.d)

lib: $(objects) 
	@-mkdir -p $(libdir)
	$(AR) -rs $(staticlib) $^ $(SISLIBS)


%.o: %.c
	$(CC) -c $(CFLAGS) $*.c -o $*.o
	$(CC) -MM $(CFLAGS) $*.c > $*.d
	@cp -f $*.d $*.d.tmp
	@sed -e 's|.*:|$*.o:|' < $*.d.tmp > $*.d
	@sed -e 's/.*://' -e 's/\\$$//' < $*.d.tmp | fmt -1 | \
	  sed -e 's/^ *//' -e 's/$$/:/' >> $*.d
	@rm -f $*.d.tmp


clean:
	rm $(objects) ./src/*.d
	rm -R ./lib/

