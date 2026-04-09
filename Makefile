TARGET = minecraft.nes
CC     = cl65
CFLAGS = -t nes -O
LDFLAGS = -C game.cfg
SRC    = main.c chr.s
CHR    = game.chr

all: $(TARGET)

$(CHR): generate_chr.py
	python3 generate_chr.py

$(TARGET): $(SRC) $(CHR)
	$(CC) $(CFLAGS) $(LDFLAGS) $(SRC) -o $(TARGET)

clean:
	rm -f $(TARGET) $(CHR)