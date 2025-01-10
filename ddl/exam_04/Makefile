#
#
sqlparser: sqlgram.y sqlscanner.l sqlparser.c
	bison -d -o sqlgram.c sqlgram.y
	flex -o sqlscanner.c sqlscanner.l
	gcc -g -o $@ sqlscanner.c sqlgram.c sqlparser.c

clean:
	rm *.o sqlparser sqlscanner.c sqlgram.c sqlscanner.h sqlgram.h
