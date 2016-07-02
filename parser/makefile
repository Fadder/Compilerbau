.SUFFIXES:
CC     = gcc
LEX    = flex
YACC   = bison

CFLAGS = -std=c99 -Wall -DLEXDEBUG
YFLAGS = -t -d -v
LFLAGS = -t

YFILES = minako-syntax.y
LFILES = minako-lexic.l
CFILES =

source = $(YFILES) $(LFILES) $(CFILES)
target = $(YFILES:%.y=%.tab.o) $(LFILES:%.l=%.o) $(CFILES:%.c=%.o)

# Compiling
%.tab.c: %.y
	$(YACC) $(YFLAGS) $<
%.c: %.l
	$(LEX) $(LFLAGS) $< > $@
%.o: %.c
	$(CC) $(CFLAGS) -c $<

# Targets
all: minako
minako: $(target)
	$(CC) $(CFLAGS) $^ -o $@

clean:
	$(RM) $(RMFILES) minako core a.out *.o *.tab.* *.output
