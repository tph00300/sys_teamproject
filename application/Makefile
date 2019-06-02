simpleMain : buzzer.o flame.o gas.o main.o
	gcc -o simpleMain buzzer.o flame.o gas.o main.o -lwiringPi

buzzer.o : buzzer.c
	gcc -c buzzer.c -lwiringPi

flame.o : flame.c
	gcc -c flame.c -lwiringPi

gas.o : gas.c
	gcc -c gas.c -lwiringPi

main.o : main.c
	gcc -c main.c -lwiringPi

clean :
	rm *.o simpleMain
