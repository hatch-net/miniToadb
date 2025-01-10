#
#
sqlparser: sqlscanner.l sqlgram.y
	bison -d -o sqlgram.c sqlgram.y
	flex -o sqlscanner.c sqlscanner.l
	gcc -g -o $@ sqlscanner.c sqlgram.c  

clean:
	rm *.o sqlparser *.c *.h 
