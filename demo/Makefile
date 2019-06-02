demo : buzzer.o flame.o gas.o led.o pir.o demo.o
	gcc -o demo buzzer.o flame.o gas.o led.o pir.o demo.o -lwiringPi -lpthread -lm

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

demo.o : demo.c
	gcc -c demo.c -lwiringPi -lpthread -lm

clean :
	rm *.o demo
