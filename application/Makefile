simpleMain : buzzer.o main.o
	gcc -o simpleMain buzzer.o flame.o main.o

buzzer.o : buzzer.c
	gcc -c buzzer.c

flame.o : flame.c
	gcc -c flame.c -lwiringPi

main.o : main.c
	gcc -c main.c

clean :
	rm *.o simpleMain
