#
#
sqlparser: sqlgram.y sqlscanner.l sqlparser.c node.c link.c dictionary.c hatfile.c executor.c pages.c seqscan.c portal.c plan.c 
	bison -d -o sqlgram.c sqlgram.y
	flex -o sqlscanner.c sqlscanner.l
	gcc -g -o $@ sqlscanner.c sqlgram.c sqlparser.c node.c link.c dictionary.c hatfile.c executor.c pages.c seqscan.c portal.c plan.c 


clean:
	rm sqlparser sqlscanner.c sqlgram.c sqlscanner.h sqlgram.h
