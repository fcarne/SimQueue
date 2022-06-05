/*******************************************************
 LINKED_CALENDAR.H - Old Calendar implementation
 *******************************************************/

#ifndef _CALENDAR_LINKED_H
#define _CALENDAR_LINKED_H

#include "calendar.h"

class linked_calendar: public calendar {

	event *head;
	event *last;

public:
	linked_calendar();
	virtual ~linked_calendar();

	virtual event* get();
	virtual void put(event *New_event);
};

inline linked_calendar::linked_calendar() {
	head = NULL;
	last = NULL;
}

inline linked_calendar::~linked_calendar() {
	event *temp = head;
	last->next = NULL;
	while (temp != NULL) {
		temp = temp->next;
		delete head;
		head = temp;
	}
}

#endif
