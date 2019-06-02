simpleMain : buzzer.o main.o
	gcc -o simpleMain buzzer.o main.o

buzzer.o : buzzer.c
	gcc -c buzzer.c

main.o : main.c
	gcc -c main.c

clean :
	rm *.o simpleMain
