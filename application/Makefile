<<<<<<< HEAD
simpleMain : buzzer.o flame.o gas.o led.o main.o
=======
simpleMain : buzzer.o flame.o gas.o main.o
>>>>>>> f269bcf199179a62ef37a78ad03ccd8d514a20bf
	gcc -o simpleMain buzzer.o flame.o gas.o led.o main.o -lwiringPi

buzzer.o : buzzer.c
	gcc -c buzzer.c -lwiringPi

flame.o : flame.c
	gcc -c flame.c -lwiringPi

gas.o : gas.c
	gcc -c gas.c -lwiringPi

led.o : led.c
	gcc -c led.c -lwiringPi

pir.o : pir.c
	gcc -c pir.c -lwiringPi

main.o : main.c
	gcc -c main.c -lwiringPi

clean :
	rm *.o simpleMain
