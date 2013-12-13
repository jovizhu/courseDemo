
#ifndef MESSAGE_H_
#define MESSAGE_H_


#include "NetworkUtil.h"

int SendUnchokMessage(AnnouceRes * announceres, MetaStruct * metainfo);
int sendChockMessage(AnnouceRes * announceres, MetaStruct * metainfo);
int sendInterestMessage(AnnouceRes * announceres, MetaStruct * metainfo);
int sendUnInterestMessage(AnnouceRes * announceres, MetaStruct * metainfo);
int sendHaveMessage(AnnouceRes * announceres, MetaStruct * metainfo);
int SendHandShakeMessage(AnnouceRes * announceres, MetaStruct * metainfo);
int SendBitFieldMessage(AnnouceRes * announceres, MetaStruct * metainfo);
int SendHandShakeMessage2(int sock_fd, AnnouceRes * announceres, MetaStruct * metainfo);
int SendBitFieldMessage2(int sock_fd, AnnouceRes * announceres, MetaStruct * metainfo);
int sendInterestMessage2(int sock_fd, AnnouceRes * announceres, MetaStruct * metainfo);
int sendRequestMessage2(int sock_fd, AnnouceRes * announceres, MetaStruct * metainfo, int pieceindex);
int SendUnchockMessage2(int sock_fd, AnnouceRes * announceres, MetaStruct * metainfo);

#endif
