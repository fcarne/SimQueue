/*******************************************************
 CALENDAR HEAP H
 *******************************************************/

#ifndef CALENDAR_HEAP_H_
#define CALENDAR_HEAP_H_

#include "calendar.h"

class calendar_heap: public calendar {
	static const int INIT_CAPACITY;

	event **heap;
	int capacity;
	int heap_size;

public:
	calendar_heap();
	virtual ~calendar_heap();

	virtual event* get();
	virtual void put(event *New_event);
private:

	void heapify(int i);

	void swap(event **e1, event **e2) {
		event *temp = *e1;
		*e1 = *e2;
		*e2 = temp;
	}

	int compare(event *e1, event *e2) {
		return e1->time > e2->time ? 1 : -1;
	}

	int left(int i) {
		return (2 * i + 1);
	}

	int right(int i) {
		return (2 * i + 2);
	}

	int parent(int i) {
		return (i - 1) / 2;
	}

};

inline calendar_heap::calendar_heap() {
	capacity = INIT_CAPACITY;
	heap_size = 0;
	heap = new event*[capacity];
}

inline calendar_heap::~calendar_heap() {
	for (int i = 0; i < heap_size; i++) {
		delete heap[i];
	}
}

#endif /* CALENDAR_HEAP_H_ */
