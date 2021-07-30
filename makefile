#NAME = head
#NAME = tail
#NAME = wc
#NAME  = true
#NAME = false
NAME = env

FLAGS = -g -Wall -pedantic-errors

compile: $(NAME).c
	gcc -o $(NAME) $(FLAGS) $(NAME).c

run: compile
	./$(NAME)

clean:
	rm -f ./head
	rm -f ./tail
	rm -f ./wc
	rm -f ./true
	rm -f ./false
	rm -f ./env
