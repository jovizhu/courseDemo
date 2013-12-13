#ifndef ANNOUNCE_H_
#define ANNOUNCE_H_

#include "NetworkUtil.h"
#include "bencode.h"



int EncodeHashtable(char * destination, uint8_t * source);

void AnnounceToTracker(MetaStruct * Metainfo, AnnouceRes * announceres, int exit_flag);

void Print_Trackerinfo(AnnouceRes * announceres);

void GetAnnouceInfo(be_node * node, ssize_t indent, AnnouceRes * announceres);


#endif /* ANNOUNCE_H_ */
