CC =  x86_64-linux-gnu-gcc

CFLAGS = -ffreestanding -nostdlib -fno-common -mcmodel=small -mno-red-zone -mno-mmx -mno-sse -mno-sse -Wall -Wextra -Werror -c

OBJPATH = ./obj

SRCC = kernel.c

OBJC = $(SRCC:%.c=$(OBJPATH)/%.o)
HEADERS = 

all: $(OBJPATH) $(OBJPATH)/kernel.o

$(OBJPATH):
	@mkdir $@

$(OBJPATH)/%.o: %.c $(HEADERS)
	@echo "COMPILING:"
	$(CC) $(CFLAGS) $< -o $@

.PHONY: clean

clean:
	rm -rf $(OBJPATH)
