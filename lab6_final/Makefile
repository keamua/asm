.PHONY: all clean main run test debug
all: run
main.tab.cc: parser.y
	bison -o main.tab.cc -v parser.y
lex.yy.cc: lexer.l
	flex -o lex.yy.cc lexer.l
main: lex.yy.cc main.tab.cc
	g++ $(shell ls *.cpp *.cc) -o main.out
run: main
	./main.out
test:main
	for file in $(basename $(shell find test/*.c)); \
	do \
		./main.out <$$file.c >$$file.s; \
	done
clean:
	rm -f *.output *.yy.* *.tab.* *.out test/*.res test/*.out test/*.s
