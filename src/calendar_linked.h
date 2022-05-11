/*******************************************************
 CALENDAR LINKED H
 *******************************************************/

#ifndef _CALENDAR_LINKED_H
#define _CALENDAR_LINKED_H

#include "calendar.h"

class calendar_linked: public calendar {

	event *head;
	event *last;

public:
	calendar_linked();
	virtual ~calendar_linked();

	virtual event* get();
	virtual void put(event *New_event);
};

inline calendar_linked::calendar_linked() {
	head = NULL;
	last = NULL;
}

inline calendar_linked::~calendar_linked() {
	event *temp = head;
	last->next = NULL;
	while (temp != NULL) {
		temp = temp->next;
		delete head;
		head = temp;
	}
}

#endif
