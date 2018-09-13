CC=clang

all: tpm tfork tdown tclip

tpm: tpm.c
	${CC} -o tpm tpm.c

tdown: tdown.c
	${CC} -o tdown tdown.c

tfork: tfork.c
	${CC} -o tfork tfork.c

tclip: tclip.c
	${CC} -o tclip tclip.c

clean:
	rm -f *.o
