cc = gcc
target = main
objects = main.o get.o count.o print.o

$(target): $(objects)
	$(cc) -o $(target) $(objects)

$(objects) : book.h

.PHONY : clean
clean:
	rm $(target) $(objects)
