
object = main.o cp_directory.o cp_single.o
CFLAGS = -g
mycp : $(object)
	gcc $(CFLAGS) $(object) -o mycp 

main.o : main.c
	gcc $(CFLAGS) -c main.c

cp_directory.o : cp_directory.c
	gcc $(CFLAGS) -c cp_directory.c

cp_single.o : cp_single.c
	gcc $(CFLAGS) -c cp_single.c
	
	
.PHONY : clean
clean:
	rm -f *.o mycp