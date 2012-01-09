APP=ipparser

all: ipparser.c
	gcc -g -o $(APP) $<

clean:
	rm -f *.o $(APP)
