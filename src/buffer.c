/***************************************************************************
 BUFFER.C
 ***************************************************************************/

#include "buffer.h"

buffer::buffer() {
	head = NULL;
	last = NULL;
	status = 0;
	tot_delay = 0.0;
	tot_packs = 0.0;
}

void buffer::insert(packet *pack) {
	if (head == NULL) {
		head = pack;
	} else {
		last->next = pack;
	}

	last = pack;
	last->next = head;
}

packet* buffer::get() {

	if (head == NULL)
		return NULL;

	packet *pack = head;

	if (last == head) {
		last = NULL;
		head = NULL;
	} else {
		head = head->next;
		last->next = head;
	}

	return pack;
}

const network_buffer* network_buffer::EXIT = new network_buffer(0);
