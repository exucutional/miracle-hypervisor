CC =  x86_64-linux-gnu-gcc

CFLAGS = -g -ffreestanding -nostdlib -fno-common -mcmodel=small -mno-red-zone -mno-mmx -mno-sse -mno-sse -Wall -Wextra -Werror -c

OBJPATH = ./obj

SRCC = kernel.c serial_print.c

OBJC = $(SRCC:%.c=$(OBJPATH)/%.o)
HEADERS = serial_print.h types.h

all: $(OBJPATH) $(OBJC)

$(OBJPATH):
	@mkdir $@

$(OBJPATH)/%.o: %.c $(HEADERS)
	@echo "COMPILING:"
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJPATH)
