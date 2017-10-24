TOOLS_CFLAGS	:= -Wstrict-prototypes -Wshadow -Wpointer-arith -Wcast-qual \
		   -Wcast-align -Wwrite-strings -Wnested-externs -Winline \
		   -W -Wundef -Wmissing-prototypes
CFLAGS  := -O3 -g
#
# Programs
#
all:	gpio_thru_no_interrupt gpio_thru_interrupt


gpio_thru_no_interrupt:  gpio_thru_no_interrupt.c beaglebone_gpio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

gpio_thru_interrupt:  gpio_thru_interrupt.c beaglebone_gpio.h
	$(CC) $(CFLAGS) $(LDFLAGS) -o $@ $<

#
# Objects
#

%.o: %.c
	$(CC) $(CFLAGS) $(TOOLS_CFLAGS) -c $< -o $@

clean:
	rm gpio_through_no_interrupt

print:
	@echo LDFLAGS = $(LDFLAGS)
