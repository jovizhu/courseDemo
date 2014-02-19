all: LCM1 LCM2 SER BBC

clean:
	rm LCM1
	rm LCM2
	rm BBC
	rm SER


LCM1: LarryCurlyMoe1.c jovi.h 
	gcc LarryCurlyMoe1.c -o LCM1 -pthread

LCM2: LarryCurlyMoe2.c jovi.h
	gcc LarryCurlyMoe2.c -o LCM2 -pthread

SER: SantaElfReindeer.c jovi.h
	gcc SantaElfReindeer.c -o SER -pthread

BBC: BurgerBuddies.c jovi.h
	gcc BurgerBuddies.c -o BBC -pthread
