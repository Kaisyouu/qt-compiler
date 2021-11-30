all: compiler.l compiler.y
	bison -d compiler.y
	flex compiler.l
	gcc lex.yy.c compiler.tab.c -o com

clean:
	rm -f compiler.tab.c compiler.tab.h lex.yy.c com