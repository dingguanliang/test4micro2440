####################################################

OBJTREE	:= $(CURDIR)
SRCTREE	:= $(CURDIR)
export	OBJTREE SRCTREE

include	$(SRCTREE)/config.mk

####################################################

LIBS	:= arch/arm/cpu/arm920t/lib.a
LIBS 	+= board/lib.a
LIBS 	+= arch/arm/lib/lib.a
LIBS 	+= common/lib.a
LIBS 	+= lib/lib.a
LIBS 	+= drivers/function/led/lib.a
LIBS 	+= drivers/function/button/lib.a
LIBS 	+= drivers/function/buzzer/lib.a
LIBS 	+= drivers/function/eeprom/lib.a
LIBS 	+= drivers/function/nand/lib.a
LIBS 	+= drivers/bus/dma/lib.a
LIBS 	+= drivers/bus/interrupt/lib.a
LIBS 	+= drivers/bus/wdt/lib.a
LIBS 	+= drivers/bus/timer/lib.a
LIBS 	+= drivers/bus/gpio/lib.a
LIBS 	+= drivers/bus/adc/lib.a
LIBS 	+= drivers/bus/rtc/lib.a
LIBS 	+= drivers/bus/iic/lib.a
LIBS 	+= drivers/bus/uart/lib.a

CDIR	:= $(dir $(LIBS))

####################################################

.PHONY: all
all: clean firmware.bin clean_dep

firmware.bin: firmware.elf
	$(OBJCOPY) $(OBJCFLAGS) -O binary $< $@

firmware.elf: mklibs
	$(LD) $(LDFLAGS) -o $@ $(LIBS)

.PHONY: mklibs
mklibs: 
	@for n in $(CDIR); \
	do \
		$(MAKE) -C $$n; \
	done

####################################################

.PHONY: clean_dep
clean_dep:
	@find -type f \( -name '.depend' \) -print | xargs rm -f
	@echo clear depend file complete!

####################################################

.PHONY: clean
clean:
	@find -type f \( -name '*.map' -o -name '*.asm' -o -name '*.elf' -o -name '*.bin' -o -name '*.o' -o -name '*.a' -o -name '.depend' \) -print | xargs rm -f
	@echo clear complete!

####################################################

.PHONY: disasm
disasm: all firmware.asm

firmware.asm:
	$(OBJDUMP) -D -b binary -m arm firmware.bin >> $@

####################################################
