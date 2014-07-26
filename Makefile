CFLAGS=-g -Wall `libmikmod-config --cflags`
LDFLAGS=`libmikmod-config --libs` -lsndfile

SRC=modex.c
OBJ=$(SRC:.c=.o)

all: $(SRC) modex

clean:
	$(RM) -f $(OBJ)
	$(RM) -f modex

modex: $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS)
	
.c.o:
	$(CC) $(CFLAGS) $< -c -o $@
	
