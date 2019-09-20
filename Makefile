CC	= gcc
RM	= rm -fr

OUT	= jim

CFLAGS	+= -Wall
CFLAGS	+= -g
# CFLAGS	+= -O0

# CFLAGS	+= -fsanitize=undefined -fno-sanitize-recover=undefined
# LDFLAGS	+= -fsanitize=undefined -fno-sanitize-recover=undefined

# CFLAGS	+=-fsanitize=address -fsanitize-recover=address
# LDFLAGS	+=-fsanitize=address -fsanitize-recover=address

# CFLAGS	+= -fsanitize=address -fno-sanitize-recover=address
# LDFLAGS	+= -fsanitize=address -fno-sanitize-recover=address

all:	$(OUT)

jim: main.c
	$(CC) -o $@ $< $(CFLAGS) $(LDFLAGS)

clean:
	$(RM) $(OUT)

