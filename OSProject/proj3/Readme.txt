1.In my program, there are two disk client, one is random dclient, named dclient.
	another is for input from the stdin, so it is named as stdindclient
2.If there is some problem for running in the fork server, pls swith to timberlake server.
3.When you restart the fserver and dserver, 
  Please rm the socket file.!!!!!!!
  This is very important, Or the server will crash.
4.The mygradclient is the test script for the User Level Command
5.There is 3 testrun file
	testrun.txt: bserver lsserver and dserver running log
	testrunClient.txt: client running log
	testrunFServer.txt: File server running log


thanks

wei zhu
wzhu4@buffalo.edu
