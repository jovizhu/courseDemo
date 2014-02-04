UBTorrent
==========
Author : wei zhu
        jovi.zhu@hotmail.com

UB Torrent is a simply Bit Torrent client referring the subset of official Bit Torrent protocol. 
It supports the official meta info file structure (.torrent file), 
Tracker HTTP protocol and Peer Wire Protocol (over TCP). 
It was implemented by C and can work with official tracker system and official torrent file.

Issue: (The readme file is added after a long time. Many issues I can not remember )
1. Message handling should be rewrite. The currnt logic is confused. It is better to write a msg queue handling all the msg.
2. The initialize function should be dynamic based on the exactly torrent file.



ChattyMe (ver 0.3)
==========================
Authors: Wei Zhu
jovi.zhu@hotmail.com

Q: How to make file in SunOS?
A: Please use "gmake" instead of "make".

Q: Did ChattyMe fix the problem of the UDP_Echo example?
A: Yes. ChattyMe started the server service in the background 
   first, then took the commands from the cammand line. So it
   fixed the problem.
   
   
   
   
Storage System
==========================
Authors: Wei Zhu
jovi.zhu@hotmail.com

1.In my program, there are two disk client, one is random dclient, named dclient.
	another is for input from the stdin, so it is named as stdindclient
	
2.When you restart the fserver and dserver, 
  Please rm the socket file.!!!!!!!
  This is very important, Or the server will crash.
  
3.The mygradclient is the test script for the User Level Command

4.There is 3 testrun file
	testrun.txt: bserver lsserver and dserver running log
	testrunClient.txt: client running log
	testrunFServer.txt: File server running log
