#
#
sqlparser: sqlgram.y sqlscanner.l sqlparser.c node.c link.c 
	bison -d -o sqlgram.c sqlgram.y
	flex -o sqlscanner.c sqlscanner.l
	gcc -g -o $@ sqlscanner.c sqlgram.c sqlparser.c node.c link.c


clean:
	rm sqlparser sqlscanner.c sqlgram.c sqlscanner.h sqlgram.h
