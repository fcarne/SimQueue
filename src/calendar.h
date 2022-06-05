/*******************************************************
 CALENDAR.H - Edited
 *******************************************************/

#ifndef _CALENDAR_H
#define _CALENDAR_H

#include "simulator.h"
#include "event.h"

class calendar {
public:
	virtual ~calendar() {
	}

	virtual event* get() = 0;
	virtual void put(event *New_event) = 0;
};

#endif

