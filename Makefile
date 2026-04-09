TARGET = minecraft.nes
CC     = cl65
CFLAGS = -t nes -O
LDFLAGS = -C game.cfg
SRC    = main.c chr.s
CHR    = game.chr

all: $(TARGET)

$(CHR):
	python3 generate_chr.py

$(TARGET): $(SRC) $(CHR)
	$(CC) $(CFLAGS) $(LDFLAGS) $^ -o $@

clean:
	rm -f $(TARGET) $(CHR)
