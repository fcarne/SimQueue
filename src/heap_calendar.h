/*******************************************************
  C5 Project extension - HEAP_CALENDAR.H
 *******************************************************/

#ifndef HEAP_CALENDAR_H_
#define HEAP_CALENDAR_H_

#include "calendar.h"

class heap_calendar: public calendar {
	static const int INIT_CAPACITY;

	event **heap; // dynamic array
	int capacity; // maximum size of the heap
	int heap_size;  // # of events actually stored

public:
	heap_calendar();
	virtual ~heap_calendar();

	virtual event* get();
	virtual void put(event *New_event);
private:

	void heapify(int i);

	// swaps the contents of two pointers
	void swap(event **e1, event **e2) {
		event *temp = *e1;
		*e1 = *e2;
		*e2 = temp;
	}

	// the compare method needed to order the events in the calendar
	int compare(event *e1, event *e2) {
		return e1->time > e2->time ? 1 : -1;
	}

	// computes the index of the left child of the event stored at index i
	int left(int i) {
		return (2 * i + 1);
	}

	// computes the index of the right child of the event stored at index i
	int right(int i) {
		return (2 * i + 2);
	}

	// computes the index of the parent of the event stored at index i
	int parent(int i) {
		return (i - 1) / 2;
	}

};

inline heap_calendar::heap_calendar() {
	capacity = INIT_CAPACITY;
	heap_size = 0;
	heap = new event*[capacity];
}

inline heap_calendar::~heap_calendar() {
	for (int i = 0; i < heap_size; i++) {
		delete heap[i];
	}
}

#endif /* HEAP_CALENDAR_H_ */
