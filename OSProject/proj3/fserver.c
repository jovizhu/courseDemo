/*
   * Program: fserver.c
   * 
   * Requires: jovi.h
   * 
   * Description:
   * 
   * To execute:
   * 
   * Written by: Wei Zhu (Jovi) 
   * Date : 11/30/2010
  */



#include "jovi.h"

/*
 *  Function name: main
 *	Author: wzhu4@buffalo.edu
 *
 *	Date: 11/30/2010
 */

int main( int argc, char * argv[])
{
	pid_t child_pid;
	int i,j,k; // for loop used as a index

	char * command_array[MAXLINE];
	int command_count;
	char read_line[MAXLINE];
	int bytes_read;
	int read_line_len;
	char send_line[MAXLINE];
	int listen_fd;
	int dsock_fd;
	struct sockaddr_un fserver_address, fclient_address, dserver_address;
	int file_size;
	char result_line[MAXLINE];
	char CommandLineToDisk[MAXLINE];
	char tempstr[MAXLINE];
	int FileExistFlag = 0;
	int spaceflag;

	char dserversocketname[MAXLINE];
	char fserversocketname[MAXLINE];
		
   	int result; 

	// define for the File Tabel
	struct FileObject MyFileTable[MAXFILENUM];
	char BlockInUse[256];
	char ResponseLineFromDisk[MAXLINE];
	int read_bytes;
	int sector_index, track_index;
	int sector_num, track_num;

	if ( argc !=3 )
	{
		printf ( "Usage: fserver dserversocket fserversocket\n " );
		exit (1);
	}

	// if argc != 1
	memset(dserversocketname,0,sizeof(dserversocketname));
	memset(fserversocketname,0,sizeof(fserversocketname));

	// initial the FileTable
	for( i=0; i<MAXFILENUM; i++)
	{
		memset(MyFileTable[i].Filename,0,9); 
		MyFileTable[i].Filesize = 0;
	}

	//Initial the blockInUse Table
	memset(BlockInUse,0,sizeof(BlockInUse));
	BlockInUse[0]=1; // for the file table
	BlockInUse[1]=1; // for myself


	strcpy(dserversocketname,argv[1]);
	strcpy(fserversocketname,argv[2]);
	printf("Disk Server Socket File name is %s \n",dserversocketname);
	printf("File Server Socket File name is %s \n",fserversocketname);


	//Connect to the Basic Disk server
	if((dsock_fd = socket(PF_UNIX, SOCK_STREAM,0)) < 0 )
	{
		printf("client error,  can not create socket to disk server \n");
		exit(1);
	}

	// set the disk server socket attribute
	dserver_address.sun_family = AF_UNIX;
	strcpy(dserver_address.sun_path, dserversocketname);

	//connect to the disk server socket
	if(connect(dsock_fd, ( struct sockaddr*) &dserver_address, sizeof(dserver_address)) <0){
		printf("Client Error: can not connect to the disk server \n");
		exit(1);
	}


	if(write(dsock_fd,"I",1) < 0 )
	{
		printf("write() error\n");
		exit(1);
	}


	memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
	//sleep(1);
	if(read(dsock_fd, ResponseLineFromDisk,sizeof(ResponseLineFromDisk)) < 0)
	{
		printf("read() error\n");
		exit(1);
	}

	printf("Internal received:\"%s\"",ResponseLineFromDisk);
	sscanf(ResponseLineFromDisk,"%d %d",&track_num,&sector_num);
	printf("Disk server has %d tracks / %d sectors per track\n", track_num, sector_num);


	//Get the filetable and File block used situation
	if(write(dsock_fd,"R 0 0",5) < 0)
	{
		printf("write error\n");
		exit(1);
	}
	memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
	//sleep(1);
	if(read(dsock_fd,ResponseLineFromDisk,sizeof(ResponseLineFromDisk))<0)
	{
		printf("read() error\n");
		exit(1);
	}
	memcpy(MyFileTable,ResponseLineFromDisk+1,sizeof(MyFileTable));

	// Get the Block Used 
	if(write(dsock_fd,"R 0 1",5) <0)
	{
		printf("write error\n");
		exit(1);
	}

	memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
	//sleep(1);
	if(read(dsock_fd,ResponseLineFromDisk,sizeof(ResponseLineFromDisk))<0)
	{
		printf("read() error\n");
		exit(1);
	}
	memcpy(BlockInUse, ResponseLineFromDisk+1,sizeof(BlockInUse));

	 // The first two elements must be occupied
	BlockInUse[0]= 1;
	BlockInUse[1]= 1;

	// Establish the Listening Server for the File System
	printf("File server listening on UNIX socket \n");
	printf("\"%s\" \n\n",fserversocketname);


	// Create a unix socket
	if ((listen_fd = socket(PF_UNIX, SOCK_STREAM, 0)) < 0) {
		printf("server error: can't create socket for the File Server \n");
		exit(1);
	}

	// Setup socket attribute
	fserver_address.sun_family = AF_UNIX;
	strcpy(fserver_address.sun_path, fserversocketname);

	// bind server_address to the passive socket
	if (bind(listen_fd, (struct sockaddr *) &fserver_address, sizeof(fserver_address))< 0) {
	printf("bind() error \n");
	}

	// start listening for incoming connection requests
	// LISTENQ is set to 5
	if (listen(listen_fd, LISTENQ) < 0) {
		printf("listen() failed");
	}
		
		
	//infinite loop to accept connection

	while(1) {

		socklen_t client_len = sizeof(fclient_address);
		int conn_fd;

		printf("Awaiting connection...\n");

		if ((conn_fd = accept(listen_fd, (struct sockaddr *) &fclient_address,
				&client_len)) < 0) {
			if (conn_fd == 0) {
				printf("Errno is Zero \n");
				continue;
			} else if ( conn_fd < 0){
				printf("accept() failed \n");
			}
		}
		
		printf("Acquired client. \n");
		memset(read_line,0,sizeof(read_line));
		memset(command_array,0,sizeof(command_array));
		
		//Get the filetable and File block used situation
		if(write(dsock_fd,"R 0 0",5) < 0)
		{
		   printf("write error\n");
		   exit(1);
		}
		memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
		//sleep(1);
		if(read(dsock_fd,ResponseLineFromDisk,sizeof(ResponseLineFromDisk))<0)
		{
		   printf("read() error\n");
		   exit(1);
		}
		memcpy(MyFileTable,ResponseLineFromDisk+1,sizeof(MyFileTable));
		  
		// Get the Block Used 
		if(write(dsock_fd,"R 0 1",5) <0)
		{
		   printf("write error\n");
		   exit(1);
		}   
		  
		memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
		//sleep(1);
		if(read(dsock_fd,ResponseLineFromDisk,sizeof(ResponseLineFromDisk))<0)
		{
		   printf("read() error\n");
		   exit(1);
		}   
		memcpy(BlockInUse, ResponseLineFromDisk+1,sizeof(BlockInUse));

	    // The first two elements must be occupied
		BlockInUse[0]= 1;
		BlockInUse[1]= 1;
		     
		if ((child_pid = fork()) < 0) {
			printf("fork() failed \n");
		}
		
		if (child_pid == 0) { 
			/* child process*/
			printf("this is child process\n");
			while(1)
			{
				memset(read_line,0,sizeof(read_line));
				//sleep(1);
				bytes_read = read(conn_fd, read_line, MAXLINE);
				if (bytes_read < 0){
						printf(" read() failed \n");
				}
				if ( bytes_read == 0 ){
					printf("bytes_read ==0\n");
					exit(1);
				}
				

				memset(result_line,0,sizeof(result_line));
				close(listen_fd); /* close the listening socket */

				read_line[bytes_read]=0;
				printf("Received Request: ");
				printf("\"%s\" \n", read_line);

				read_line_len=strlen(read_line);
			
				command_count=parseLine(read_line, command_array);
				command_array[command_count]=NULL;
			
				if( (strcmp(command_array[0],"F")==0) && (command_count == 1))
				{
						//clear FAT
						printf("Clear FAT \n");
						memset(CommandLineToDisk, 0, sizeof(CommandLineToDisk));
						strcpy(CommandLineToDisk,"W 0 0 256 ");
						write(dsock_fd,CommandLineToDisk,256+10);

						memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
						//sleep(1);
						read_bytes=read(dsock_fd,ResponseLineFromDisk,1);
						printf("Internal Received %s\n", ResponseLineFromDisk);

						//clear File Table
						printf("Clear File Table \n");
						memset(CommandLineToDisk,0, sizeof(CommandLineToDisk));
						strcpy(CommandLineToDisk, "W 0 1 256 ");
						write(dsock_fd,CommandLineToDisk,256+10);

						memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
						//sleep(1);
						read_bytes=read(dsock_fd,ResponseLineFromDisk,1);
						printf("Internal Received %s\n", ResponseLineFromDisk);

						// write nothing to the file client
						//write(conn_fd,0,0);
						// clear it also in the FIlServer
						memset(BlockInUse,0,sizeof(BlockInUse));
						memset(MyFileTable,0,sizeof(MyFileTable));
						BlockInUse[0]=1;
						BlockInUse[1]=1;
				}
				else if( (strcmp(command_array[0],"C")==0 ) && ( command_count == 2 ) )
				{
					// Block 0 and 1 was reserved for FAT and File Table
					printf("create file\n");
					memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
					memset(result_line,0,sizeof(result_line));
					strcpy(CommandLineToDisk,"W 0 0 256 ");
					FileExistFlag = 0; // default is no existing file

					for(i=0;i<MAXFILENUM;i++)
					{
						if(strcmp(MyFileTable[i].Filename,command_array[1]) == 0 )
						{
							FileExistFlag = 1;
							break;
						}
					}

					//set the default error
					result_line[0]='2';

					if (strlen(command_array[1]) > 8 )
					{
						FileExistFlag = 2; // to avoid the following processing.
						result_line[0] = '2' ;
					}
							
					if( FileExistFlag == 0 )
					{
						printf(" File no existence\n");
						for(i=0;i<MAXFILENUM;i++)
						{
							if(strlen(MyFileTable[i].Filename) == 0)
							{
								printf("%d th in MyFIleTalbe was empty \n",i);
								strcpy(MyFileTable[i].Filename,command_array[1]);
								break;
							}
						}

						memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
						memcpy(CommandLineToDisk+10, MyFileTable,sizeof(MyFileTable));
						write(dsock_fd,CommandLineToDisk,256+10);
						//sleep(1);
						read_bytes = read( dsock_fd,ResponseLineFromDisk,1);
						printf("Internal Received %s \n", ResponseLineFromDisk);

						if(ResponseLineFromDisk[0] == '1')
						{
							result_line[0]='0';
						}
					}
					else if ( FileExistFlag == 1 )
					{
						// File already exist
						printf("file flag is set to one \n");
						result_line[0]= '1';
					}
					//send to the client the result
					write(conn_fd,result_line,1);
				}
				else if ( (strcmp(command_array[0],"W" ) == 0 ) && ( command_count == 4))
				{
					printf("Write function\n");
					memset(result_line,0,sizeof(result_line));
					result_line[0] = '2';
					int temp_file_size = atoi(command_array[2]);
					int temp;



					if( temp_file_size <= 1000)
					{
						for(i=0;i<MAXFILENUM;i++)
						{
					   		printf("Enter the loop \n");

					    	if(strcmp(MyFileTable[i].Filename,command_array[1]) == 0 )
					    	{
								//Eraser the BlockInUse 
								int blockstart = MyFileTable[i].BlockToUse;
								while(BlockInUse[blockstart] > 1)
								{
									temp = BlockInUse[blockstart];
									BlockInUse[blockstart] = 0;
									blockstart = BlockInUse[temp]; 
								}
								BlockInUse[blockstart] = 0;

					       		// write the file in the block
					       		printf("Find the file table in the index %d \n",i);
					       		FileExistFlag = 1;
						   		MyFileTable[i].Filesize = atoi(command_array[2]);
					                    
					       		int blocknum = MyFileTable[i].Filesize/256 + 1;
						   		printf("block number needed is %d \n",blocknum);
								spaceflag = 0;

								// find the first block	
								for (j=2;j<256;j++)
								{
									if(BlockInUse[j] == 0)
									{
										MyFileTable[i].BlockToUse = j;
										BlockInUse[j]=1;
										spaceflag =1; // set the space flag
										break;	
									}
								}

								blockstart = MyFileTable[i].BlockToUse;
								printf("BlockInUse[%d] was find to store as the first\n",blockstart);

								//find the following block
								for ( k = 1; (k< blocknum) && (spaceflag == 1); k++)
								{
									//start from the 2nd, 1,2 was occupied by the fat and block table
									spaceflag = 0;
						   			for( j =2;(j<256) && (spaceflag == 0);j++)
						   			{
										if(BlockInUse[j]==0)
										{
											spaceflag = 1;
											BlockInUse[blockstart]=j;
											BlockInUse[j] = 1;
											printf("BlockInUse[%d] was find to store\n",j);
											blockstart = j;
											break;
										}
						   			}
								}

								// no enough space for the file
								if( spaceflag = 0)
										break;

						   		printf("write will start at the block[%d]\n",blockstart);
						   		// write from the blockstart
								blockstart = MyFileTable[i].BlockToUse;
								printf("BlockInUse[%d] was selected as the first\n",blockstart);

						   		int tempfilelength = MyFileTable[i].Filesize;
						   		char * strpoint = command_array[3];
						   		// defined in the before
						   		//char tempstr[9];

						   		while(tempfilelength > 256 )
						   		{
									memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
									memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
									memset(tempstr,0,sizeof(tempstr));

									track_index = blockstart/sector_num;
									sector_index = blockstart%sector_num;
									printf("Track Index %d\n",track_index);
									printf("Sector Index %d\n",sector_index);

									sprintf(tempstr,"W %d %d 256 ",track_index,sector_index);
									printf("command is %s\n", tempstr);
									// copy the W X X to the CommandLineToDisk
									strcpy(CommandLineToDisk,tempstr);
									memcpy(CommandLineToDisk+strlen(tempstr), strpoint,256);

									write(dsock_fd,CommandLineToDisk,256+strlen(tempstr));
									//sleep(1);
									read_bytes = read(dsock_fd,ResponseLineFromDisk,1);

									printf("[Internal Received] %d\n",ResponseLineFromDisk[0]);

									tempfilelength=tempfilelength-256;
									strpoint = strpoint+256;
									
									blockstart = BlockInUse[blockstart];

									printf("BlockInUse[%d] was selected\n", blockstart); 
						   		}

						   		memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
						   		memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
						   		memset(tempstr,0,sizeof(tempstr));

								track_index = blockstart/sector_num;
								sector_index = blockstart%sector_num;
								printf("Track Index %d\n",track_index);
								printf("Sector Index %d\n",sector_index);

						   		sprintf(tempstr,"W %d %d %d ",track_index,sector_index,tempfilelength);
						   		printf("command is %s\n",tempstr);

						   		//copy the W X X to the CommandLineToDisk
						   		strcpy(CommandLineToDisk,tempstr);
						   		memcpy(CommandLineToDisk+strlen(tempstr), strpoint,tempfilelength);
						   		write(dsock_fd,CommandLineToDisk,strlen(tempstr)+256);
								//sleep(1);
						   		read_bytes = read( dsock_fd,ResponseLineFromDisk,1);

						   		printf("[Internal Received] %c\n",ResponseLineFromDisk[0]);
						   		result_line[0]= '0';

								// Update the file table to the d server
								memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
								strcpy(CommandLineToDisk,"W 0 0 256 ");
								memcpy(CommandLineToDisk+10, MyFileTable,sizeof(MyFileTable));
								write(dsock_fd,CommandLineToDisk,256+10);

								memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
								//sleep(1);
		                        read_bytes = read( dsock_fd,ResponseLineFromDisk,1);
								printf("[Internal Received]%s \n", ResponseLineFromDisk);

								//Update the BlockInUse for the D server
								printf("Reset File Table \n");
								memset(CommandLineToDisk,0, sizeof(CommandLineToDisk));
								strcpy(CommandLineToDisk, "W 0 1 256 ");
								memcpy(CommandLineToDisk+10, BlockInUse,256);
								write(dsock_fd,CommandLineToDisk,256+10);

		                        memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
								//sleep(1);
								read_bytes=read(dsock_fd,ResponseLineFromDisk,1);
								printf("[Internal Received] %c\n",ResponseLineFromDisk[0]);
						   		break;
                        	} //end of if MyFileTable.fileanme is empty

							result_line[0] = '1';
                     	} // end of for loop

					} // end of if filesize < 1000
					write(conn_fd,result_line,1);
				}
				else if( (strcmp(command_array[0], "R") == 0) && (command_count ==2))
				{
					printf("Read function \n");
					memset(result_line,0,sizeof(result_line));
					char * result_current_pointer = result_line;
					result_line[0]= '2';
					for(i=0;i<MAXFILENUM;i++)
					{
					    if(strcmp(MyFileTable[i].Filename,command_array[1]) == 0 )
					    {
					    	int blockstart = MyFileTable[i].BlockToUse;
							sprintf(result_line+1,"%d ",MyFileTable[i].Filesize);
							printf("%s\n",result_line);
							result_current_pointer = result_line+strlen(result_line);
							while( BlockInUse[blockstart] !=1)
							{
								//Read the  block.
								memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
								memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
								track_index = blockstart/sector_num;
								sector_index = blockstart%sector_num;
								printf("Track Index %d\n",track_index);
								printf("Sector Index %d\n",sector_index);
							
								sprintf(tempstr,"R %d %d",track_index,sector_index);
								printf("command is %s\n", tempstr);
								strcpy(CommandLineToDisk,tempstr);
							
								write(dsock_fd,CommandLineToDisk,256+strlen(tempstr));
								//sleep(1);
								read_bytes = read(dsock_fd,ResponseLineFromDisk,MAXLINE);
								printf("Received message Length is %d\n",read_bytes);
								printf("Internal Received %c\n",ResponseLineFromDisk[0]);
						    	printf("Received content is %s\n", &ResponseLineFromDisk[1]);
								//write(conn_fd,&ResponseLineFromDisk[1],256);
								memcpy(result_current_pointer,&ResponseLineFromDisk[1],256);
								result_current_pointer = result_current_pointer + 256;
								blockstart = BlockInUse[blockstart];
							} // end BlockInUse

							// the last block
							memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
					    	memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
							track_index = blockstart/sector_num;
							sector_index = blockstart%sector_num;
							printf("Track Index %d\n",track_index);
							printf("Sector Index %d\n",sector_index);
							
							sprintf(tempstr,"R %d %d",track_index,sector_index);
							printf("command is %s\n", tempstr);
							strcpy(CommandLineToDisk,tempstr);
							
							write(dsock_fd,CommandLineToDisk,strlen(CommandLineToDisk));
							//sleep(1);
							read_bytes = read(dsock_fd,ResponseLineFromDisk,MAXLINE);
							printf("Received message Length is %d\n",read_bytes);
							printf("Internal Received %c\n",ResponseLineFromDisk[0]);
							printf("Received content is %s\n", &ResponseLineFromDisk[1]);
							//write(conn_fd,&ResponseLineFromDisk[1],256);
							memcpy(result_current_pointer,&ResponseLineFromDisk[1],256);
							result_line[0] = '0';
							//write(conn_fd,result_line,MyFileTable[i].Filesize);
							break;
					    } // end strcmp filename
						result_line[0] = '1';
					} // end maxfilenum

					if(result_line[0] == '0')
					{
						write(conn_fd,result_line,strlen(result_line));
					}
					else
					{
						write(conn_fd,result_line,1);
					}
				} 
				else if( (strcmp(command_array[0],"A") == 0 ) && ( command_count == 4 ))
				{
				
					printf("Append function\n");
					memset(result_line,0,sizeof(result_line));
					result_line[0] = '2';
					int temp_file_size = atoi(command_array[2]);
					int temp;
					int previous_block_num;
					int new_block_num;
					int block_current;
					int offset;
					int previous_offset;

					for(i=0;i<MAXFILENUM;i++)
					{
					    if(strcmp(MyFileTable[i].Filename,command_array[1]) == 0 )
					    {
							//find the last BlockInUse 
							previous_block_num = MyFileTable[i].Filesize /256 +1;
							previous_offset = MyFileTable[i].Filesize%256;
							offset = 256 - (MyFileTable[i].Filesize%256);
							int blockstart = MyFileTable[i].BlockToUse;
							printf("blockstart was %d \n",blockstart);
							printf("BlockInUse[%d] is %d\n",blockstart,BlockInUse[blockstart]);

							while(BlockInUse[blockstart] > 1)
							{
								blockstart = BlockInUse[blockstart];
								//BlockInUse[blockstart] = 0;
								//blockstart = BlockInUse[temp]; 
								printf("block was reset to another %d \n",blockstart);
							}
							//blockstart will be the last block
		

					       	// write the file in the block
					       	printf("Find the file in the index %d \n",i);
					       	FileExistFlag = 1;
							temp_file_size = temp_file_size+MyFileTable[i].Filesize;
							if(temp_file_size > 1000)
							{
								result_line[0] = 2;
								printf("File Lenghth exceed the 1000\n");
								// exceed the length of 1000
								break;
							}
						   	MyFileTable[i].Filesize = temp_file_size;
							new_block_num = MyFileTable[i].Filesize/256+1;
					                    
					       	int blocknum = new_block_num - previous_block_num;
						   	printf("Another block number needed is %d, new is %d, previous is %d \n",blocknum,new_block_num,previous_block_num);

							spaceflag = 1;

							//find the following block
							block_current = blockstart;
							for ( k = 0; (k< blocknum) && (spaceflag == 1); k++)
							{
								//start from the 2nd, 1,2 was occupied by the fat and block table
								spaceflag = 0;
						   		for( j =2;(j<256) && (spaceflag == 0);j++)
						   		{
									if(BlockInUse[j]==0)
									{
										spaceflag = 1;
										BlockInUse[block_current]=j;
										BlockInUse[j] = 1;
										printf("BlockInUse[%d] was find to store\n",j);
										block_current = j;
										break;
									}
						   		}
							}

							// no enough space for the file
							if( spaceflag == 0)
								break;

						   	printf("write will srart at the block[%d]\n",blockstart);
							//Read the current block and then re-write it.
							memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
							memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
							track_index = blockstart/sector_num;
							sector_index = blockstart%sector_num;
							printf("Track Index %d\n",track_index);
							printf("Sector Index %d\n",sector_index);

							sprintf(tempstr,"R %d %d",track_index,sector_index);
							printf("command is %s\n", tempstr);
							strcpy(CommandLineToDisk,tempstr);

							write(dsock_fd,CommandLineToDisk,256+strlen(tempstr));
							//sleep(1);
							read_bytes = read(dsock_fd,ResponseLineFromDisk,MAXLINE);
							printf("Received message Length is %d\n",read_bytes);
							printf("Internal Received %c \n",ResponseLineFromDisk[0]);
							printf("Received content is %s \n", &ResponseLineFromDisk[1]);

						   	// write from the blockstart
						   	sprintf(tempstr,"W %d %d 256 ",track_index,sector_index);
							printf("command is %s\n", tempstr);
							strcpy(CommandLineToDisk,tempstr);
							memcpy(CommandLineToDisk+strlen(tempstr),ResponseLineFromDisk+1, 256);
							memcpy(CommandLineToDisk+strlen(tempstr)+(256-offset),command_array[3],offset);
							printf("command to be send is %s\n",CommandLineToDisk);
							//blockstart = MyFileTable[i].BlockToUse;
							printf("BlockInUse[%d] was selected as the first\n",blockstart);

						   	int tempfilelength = MyFileTable[i].Filesize-(previous_block_num-1)*256;
							write(dsock_fd,CommandLineToDisk,256+strlen(tempstr));
							read_bytes = read(dsock_fd,ResponseLineFromDisk,1);
							printf("[Internal Received] %d\n",ResponseLineFromDisk[0]);
							tempfilelength = tempfilelength - 256;
								
							if(tempfilelength > 0)
							{
								
						   		char * strpoint = command_array[3]+offset;
								blockstart = BlockInUse[blockstart];

						   		while(tempfilelength > 256 )
						   		{
									memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
									memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
									memset(tempstr,0,sizeof(tempstr));

									track_index = blockstart/sector_num;
									sector_index = blockstart%sector_num;
									printf("Track Index %d\n",track_index);
									printf("Sector Index %d\n",sector_index);

									sprintf(tempstr,"W %d %d 256 ",track_index,sector_index);
									printf("command is %s\n", tempstr);
									// copy the W X X to the CommandLineToDisk
									strcpy(CommandLineToDisk,tempstr);
									memcpy(CommandLineToDisk+strlen(tempstr), strpoint,256);
									printf("command to be send is %s\n",CommandLineToDisk);

									write(dsock_fd,CommandLineToDisk,256+strlen(tempstr));
									read_bytes = read(dsock_fd,ResponseLineFromDisk,1);

									printf("[Internal Received] %c \n",ResponseLineFromDisk[0]);

									tempfilelength=tempfilelength-256;
									strpoint = strpoint+256;
									
									blockstart = BlockInUse[blockstart];

									printf("BlockInUse[%d] was selected \n", blockstart); 
						   		}

						   		memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
						   		memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
						   		memset(tempstr,0,sizeof(tempstr));

								track_index = blockstart/sector_num;
								sector_index = blockstart%sector_num;
								printf("Track Index %d\n",track_index);
								printf("Sector Index %d\n",sector_index);

						   		sprintf(tempstr,"W %d %d %d ",track_index,sector_index,tempfilelength);
						   		printf("command is %s\n",tempstr);

						   		//copy the W X X to the CommandLineToDisk
						   		strcpy(CommandLineToDisk,tempstr);
						   		memcpy(CommandLineToDisk+strlen(tempstr), strpoint,tempfilelength);
								printf("command to be send is %s\n",CommandLineToDisk);

						   		write(dsock_fd,CommandLineToDisk,strlen(tempstr)+256);
								//sleep(1);
						   		read_bytes = read( dsock_fd,ResponseLineFromDisk,1);

						   		printf("[Internal Received] %c \n",ResponseLineFromDisk[0]);
						   		result_line[0]= '0';

							}


							result_line[0] = '0';
							// Update the File Table for the D server
							memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
							strcpy(CommandLineToDisk,"W 0 0 256 ");
							memcpy(CommandLineToDisk+10, MyFileTable,sizeof(MyFileTable));
							write(dsock_fd,CommandLineToDisk,256+10);

                            memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
							//sleep(1);
                            read_bytes = read( dsock_fd,ResponseLineFromDisk,1);
                            printf("Internal Received %s \n", ResponseLineFromDisk);

							//Update the BlockInUse for the D server
							printf("Reset File Table \n");
							memset(CommandLineToDisk,0, sizeof(CommandLineToDisk));
							strcpy(CommandLineToDisk, "W 0 1 256 ");
							memcpy(CommandLineToDisk+10, BlockInUse,256);
							write(dsock_fd,CommandLineToDisk,256+10);

		                    memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));
							//sleep(1);
							read_bytes=read(dsock_fd,ResponseLineFromDisk,1);
							printf("Internal Received %c \n",ResponseLineFromDisk[0]);

						   	break;
                        } //end of if MyFileTable.fileanme is empty

						result_line[0] = '1';

                     } // end of for loop
					 write(conn_fd,result_line,1);
				}
				else if( (strcmp(command_array[0],"L") == 0 ) && ( command_count == 2 ))
				{
					printf("list function \n");
					memset(result_line,0,sizeof(result_line));

					if( strcmp(command_array[1],"0") == 0 )
					{
						printf("Command is L 0\n");
						for(i=0;i<MAXFILENUM;i++)
						{
							if(strlen(MyFileTable[i].Filename) !=0 )
							{
								memset(tempstr,0,sizeof(tempstr));				
								sprintf(tempstr,"%s\n",MyFileTable[i].Filename);
								strcat(result_line,tempstr);
							}
						}
					}
					else if ( strcmp(command_array[1],"1") == 0)
					{
						printf("Command is L 1\n");
						for(i=0;i<MAXFILENUM;i++)
						{
							if ( strlen(MyFileTable[i].Filename) !=0)
							{
								memset(tempstr,0,sizeof(tempstr));
								sprintf(tempstr,"%s %d\n",MyFileTable[i].Filename,MyFileTable[i].Filesize);
								strcat(result_line,tempstr);
							}
						}
					}

					if(strlen(result_line)>0)
						result_line[strlen(result_line)-1]='\0';

					write(conn_fd,result_line,strlen(result_line)+1);
				}
				else if ( (strcmp(command_array[0], "D") == 0 ) && ( command_count == 2))
				{
					int temp;
					printf("Delete function\n");
					memset(result_line,0,sizeof(result_line));
					result_line[0]='2';
					FileExistFlag = 0;
					memset(ResponseLineFromDisk,0,sizeof(ResponseLineFromDisk));

				//	printf("check point 1 \n");

					for(i=0;i<MAXFILENUM;i++)
					{
					   if(strcmp(MyFileTable[i].Filename,command_array[1]) == 0 )
					   {
					      // Delete the file in the block
					      printf("Find the file in the index %d \n",i);
					      FileExistFlag = 1;
						  memset(MyFileTable[i].Filename,0,9);

						  int blocknum = MyFileTable[i].Filesize/256 + 1;
						  int blockstart = MyFileTable[i].BlockToUse;

						  for( j=0;j<blocknum;j++)
						  {
							 temp = BlockInUse[blockstart];
							 BlockInUse[blockstart]=0;
							 blockstart = temp;
						  }

						  MyFileTable[i].Filesize = 0;
						  MyFileTable[i].BlockToUse = 0;
					      break;
					   }
					}

					if(FileExistFlag == 1 )
					{
						printf("write to the d server \n");

						memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
						strcpy(CommandLineToDisk,"W 0 0 256 ");
						memcpy(CommandLineToDisk+10,MyFileTable,sizeof(MyFileTable));
						write(dsock_fd, CommandLineToDisk,256+10);
						//sleep(1);
						read_bytes = read( dsock_fd,ResponseLineFromDisk,1);

						if(ResponseLineFromDisk[0] == '1')
						{
							// Write successful
							printf("Internal message Delelte Successful\n");
						    result_line[0] = '0';


							memset(CommandLineToDisk,0,sizeof(CommandLineToDisk));
							strcpy(CommandLineToDisk, "W 0 1 256 ");
							memcpy(CommandLineToDisk+10,BlockInUse,256);
							write(dsock_fd, CommandLineToDisk,256+10);
							//sleep(1);
							read_bytes = read(dsock_fd, ResponseLineFromDisk,1);

							if(ResponseLineFromDisk[0] == '1')
							{
								printf("Block In used table delete successful \n");
								result_line[0]='0';
							}else
							{
								result_line[0]='2';
							}
						}else
						{
							result_line[0]='2';
						}

					}
					else if(FileExistFlag == 0)
					{
						printf("FIle not exiestne\n");
						//the file not exist in the file table
						result_line[0]='1';
					}
					write(conn_fd, result_line,1);
				}
				else
				{
					memset(result_line,0,sizeof(result_line));
					strcpy(result_line,"Unsupported command \n");
					write(conn_fd,result_line,strlen(result_line));
				}
			}
			// child process while(1);
			close(conn_fd);
		}
		else if( child_pid > 0)
		{
			waitpid(child_pid,NULL,0);
			close (conn_fd);
			printf("connection closed \n");
		}
		else
		{
			printf("fork() Error \n ");
		}

	}// main while(1)

}	


/*
 *	Function: ParseLine
 *	Input: command line
 *	Output: commandArray commandcount
 *	Description: parse the commandline and store the command into command_arry
 *	Author: wzhu4@buffalo.edu
 *	Date: 09/28/2010   */
int parseLine(char * line, char * command_array[]) {
		char * p;
		p=NULL;
		int count = 0;
		p = strtok(line, " ");
		// parse the command until the end or meet a '|'
		while (p && strcmp(p,"|") != 0) {
				command_array[count] = p;
			//	printf("split command is %s \n", command_array[count]);
				count++;
				p = strtok(NULL," ");
		}
		// return the command count
		return count;
}



int MMapFree(int * map, int fd, int file_size)
{

    /* Don't forget to free the mmapped memory
     */
    if (munmap(map, file_size) == -1) {
	perror("Error un-mmapping the file");
	/* Decide here whether to close(fd) and exit() or not. Depends... */
    }

    /* Un-mmaping doesn't close the file, so we still need to do that.
     */
    close(fd);

}

