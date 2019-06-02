demo2 : buzzer.o flame.o gas.o led.o pir.o demo2.o
	gcc -o demo2 buzzer.o flame.o gas.o led.o pir.o demo2.o -lwiringPi -lpthread -lm

buzzer.o : buzzer.c
	gcc -c buzzer.c -lwiringPi -lpthread -lm

flame.o : flame.c
	gcc -c flame.c -lwiringPi -lpthread -lm

gas.o : gas.c
	gcc -c gas.c -lwiringPi -lpthread -lm

led.o : led.c
	gcc -c led.c -lwiringPi -lpthread -lm

pir.o : pir.c
	gcc -c pir.c -lwiringPi -lpthread -lm

demo2.o : demo2.c
	gcc -c demo2.c -lwiringPi -lpthread -lm

clean :
	rm *.o demo2
