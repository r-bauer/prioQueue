# makefile for priority queue using generic linked list 

IDIR=./include
SDIR=./src
CC=gcc
CFLAGS=-I$(IDIR)

ODIR=obj
LDIR=./lib

LIBS=-lm

_DEPS=	qdriver.h \
	qapp.h \
	llgen.h \
	main.h

DEPS=$(addprefix $(IDIR)/, $(_DEPS))

_OBJ=	main.o \
	qapp.o \
	llgen.o \
	qdriver.o 

OBJ=$(addprefix $(ODIR)/, $(_OBJ))

$(ODIR)/%.o: $(SDIR)/%.c $(DEPS)
	$(CC) -c -o $@ $< $(CFLAGS)

prioqueue: $(OBJ)
	$(CC) -o $@ $^ $(CFLAGS) $(LIBS)

all: $(OBJ)

$(OBJ): | $(ODIR)

$(ODIR):
	mkdir -p $(ODIR)

.PHONY:	clean

clean: 
	rm -fr $(ODIR) prioqueue* *~ $(IDIR)/*~ $(SDIR)/*~

