all: bserver bclient lsserver lsclient dserver dclient fserver fclient mycat mywrite myappend myls myls1 myrm mycp mymv mynewfs stdindclient

clean:
	rm bserver
	rm bclient
	rm lsclient
	rm lsserver
	rm dserver
	rm dclient
	rm fclient
	rm fserver
	rm mycat
	rm mywrite
	rm myappend
	rm myls
	rm myls1
	rm myrm
	rm mycp
	rm mymv
	rm mynewfs
	rm stdindclient
	
bserver: bserver.c jovi.h
	gcc bserver.c -o bserver

bclient: bclient.c jovi.h
	gcc bclient.c -o bclient

lsserver: lsserver.c jovi.h
	gcc lsserver.c -o lsserver

lsclient: lsclient.c jovi.h
	gcc lsclient.c -o lsclient

dserver: dserver.c jovi.h
	gcc dserver.c -o dserver

dclient: dclient.c jovi.h
	gcc dclient.c -o dclient

stdindclient: stdindclient.c jovi.h
	gcc stdindclient.c -o stdindclient

fserver: fserver.c jovi.h
	gcc fserver.c -o fserver

fclient: fclient.c jovi.h
	gcc fclient.c -o fclient

mycat: mycat.c jovi.h
	gcc mycat.c -o mycat

mywrite: mywrite.c jovi.h
	gcc mywrite.c -o mywrite

myappend: myappend.c jovi.h
	gcc myappend.c -o myappend

myls: myls.c jovi.h
	gcc myls.c -o myls

myls1: myls1.c jovi.h
	gcc myls1.c -o myls1

myrm: myrm.c jovi.h
	gcc myrm.c -o myrm

mycp: mycp.c jovi.h
	gcc mycp.c -o mycp

mymv: mymv.c jovi.h
	gcc mymv.c -o mymv

mynewfs: mynewfs.c jovi.h
	gcc mynewfs.c -o mynewfs
