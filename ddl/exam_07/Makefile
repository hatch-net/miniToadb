#
#
sqlparser: sqlgram.y sqlscanner.l sqlparser.c node.c
	bison -d -o sqlgram.c sqlgram.y
	flex -o sqlscanner.c sqlscanner.l
	gcc -g -o $@ sqlscanner.c sqlgram.c sqlparser.c node.c

clean:
	rm *.o sqlparser sqlscanner.c sqlgram.c sqlscanner.h sqlgram.h
