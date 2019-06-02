simpleMain : buzzer.o flame.o gas.o led.o pir.o main.o
	gcc -o simpleMain buzzer.o flame.o gas.o led.o pir.o main.o -lwiringPi -lpthread

buzzer.o : buzzer.c
	gcc -c buzzer.c -lwiringPi -lpthread

flame.o : flame.c
	gcc -c flame.c -lwiringPi -lpthread

gas.o : gas.c
	gcc -c gas.c -lwiringPi -lpthread

led.o : led.c
	gcc -c led.c -lwiringPi -lpthread

pir.o : pir.c
	gcc -c pir.c -lwiringPi -lpthread

main.o : main.c
	gcc -c main.c -lwiringPi -lpthread

clean :
	rm *.o simpleMain
