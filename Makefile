CC=gcc
CFLAGS=-g

INC= include/
SRC= src/
OBJ= main.o send_cmd.o back_cmd.o buffer.o object.o user.o
LIB= -lz -lcrypto
PROG= sent


ifeq ($(shell uname), Linux)
	INSTALL_PATH= "/usr/local/bin"
endif

all: $(PROG)

install: $(PROG)
	install $(PROG) $(INSTALL_PATH)

sent: $(OBJ)
	$(CC)  -o $@ $^ $(LIB) 
	@echo "Building for $(shell uname)"

main.o: $(INC)sent.h main.c
	$(CC) -I $(INC) $(CFLAGS) -c main.c
	@echo $@

send_cmd.o: $(INC)sent.h send_cmd.c
	$(CC) -I $(INC) $(CFLAGS) -c send_cmd.c
	@echo $@

back_cmd.o: $(INC)sent.h back_cmd.c
	$(CC) -I $(INC) $(CFLAGS) -c back_cmd.c
	@echo $@

buffer.o: $(INC)sent.h $(SRC)buffer.c
	$(CC) -I $(INC) $(CFLAGS) -c $(SRC)/buffer.c
	@echo $@

object.o: $(INC)sent.h $(SRC)object.c
	$(CC) -I $(INC) $(CFLAGS) -c $(SRC)/object.c
	@echo $@

user.o: $(INC)sent.h $(SRC)user.c
	$(CC) -I $(INC) $(CFLAGS) -c $(SRC)/user.c
	@echo $@

clean:
	-rm -f *.o -r .sent $(PROG)