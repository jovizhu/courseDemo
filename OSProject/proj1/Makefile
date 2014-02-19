all: Encrypt ForkEncrypt PipeEncrypt TimeEncrypt MyShell DupShell ThreadAdd

clean:
	rm Encrypt
	rm ForkEncrypt
	rm PipeEncrypt
	rm TimeEncrypt
	rm MyShell
	rm DupShell
	rm ThreadAdd

Encrypt: Encrypt.c jovi.h
	gcc Encrypt.c -o Encrypt

ForkEncrypt: ForkEncrypt.c jovi.h
	gcc ForkEncrypt.c -o ForkEncrypt

PipeEncrypt: PipeEncrypt.c jovi.h
	gcc PipeEncrypt.c -o PipeEncrypt

TimeEncrypt: TimeEncrypt.c jovi.h
	gcc TimeEncrypt.c -o TimeEncrypt

MyShell: MyShell.c jovi.h
	gcc MyShell.c -o MyShell

DupShell: DupShell.c jovi.h
	gcc DupShell.c -o DupShell

ThreadAdd: ThreadAdd.c jovi.h
	gcc ThreadAdd.c -o ThreadAdd -pthread
