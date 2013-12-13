#include "PeerObject.h"


int AddtoPeerObjectList(struct PeerObject * PeerObject, char * ipaddress, int remotprot) {

	PeerObject->remotport = remotprot;


	strcpy(PeerObject->ipaddress, ipaddress);

	PeerObject->time_up = 0;
	PeerObject->time_down = 0;

	return 0;
}
