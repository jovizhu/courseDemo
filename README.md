UBTorrent
==========
Author : wei zhu
        jovi.zhu@hotmail.com

UB Torrent is a simply Bit Torrent client referring the subset of official Bit Torrent protocol. 
It supports the official meta info file structure (.torrent file), 
Tracker HTTP protocol and Peer Wire Protocol (over TCP). 
It was implemented by C and can work with official tracker system and official torrent file.

Issue: (The readme file is added after a long time. Many issues I can not remember )
1. Message handling should be rewrite.
   The currnt logic is confused.
   It is better to write a msg queue handling all the msg
2. The initialize function should be dynamic based on the exactly torrent file.



Program: ChattyMe (version release 0.3)
==========================
Authors: Wei Zhu
jovi.zhu@hotmail.com

Q: How to make file in SunOS?
A: Please use "gmake" instead of "make".

Q: Did ChattyMe fix the problem of the UDP_Echo example?
A: Yes. ChattyMe started the server service in the background 
   first, then took the commands from the cammand line. So it
   fixed the problem.
