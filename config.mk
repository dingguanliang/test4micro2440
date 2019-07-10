####################################################

CROSS_COMPILE	= arm-liang-linux-gnueabi-

CC		= $(CROSS_COMPILE)gcc
AR		= $(CROSS_COMPILE)ar
LD		= $(CROSS_COMPILE)ld
OBJCOPY = $(CROSS_COMPILE)objcopy
OBJDUMP = $(CROSS_COMPILE)objdump

MAKE	= make

####################################################

CFLAGS    	= -Wall
CPPFLAGS	= -fno-builtin -nostdinc -I $(SRCTREE)/include
ARFLAGS 	= crs
LDFLAGS   	= -Map .map --script $(SRCTREE)/arch/arm/cpu/arm920t/link.lds
OBJCFLAGS 	= --gap-fill 0xff

####################################################

%.o: %.S %.c 
	$(CC) $(CFLAGS) $(CPPFLAGS) -c $@ $<

####################################################
