all:
	gcc sokoban.c -lncurses -o sokoban.o
	./sokoban.o
	clear
	echo "You Win!"
