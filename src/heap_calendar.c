/* -*- C++ -*- */
/*******************************************************
  C5 Project extension - HEAP_CALENDAR.C
 *******************************************************/

#include "global.h"
#include <stdlib.h>
#include "heap_calendar.h"

const int heap_calendar::INIT_CAPACITY = 64;

event* heap_calendar::get() {
	if (heap_size <= 0)
		return NULL;

	event *ev = heap[0];

	if (heap_size == 1) {
		heap[0] = NULL;
		heap_size--;
		return ev;
	}

	heap[0] = heap[heap_size - 1];
	heap_size--;
	heapify(0);

	return ev;
}

void heap_calendar::put(event *New_event) {
	if (heap_size == capacity) {
		capacity *= 2;
		heap = (event**) realloc(heap, capacity * sizeof(event*));
	}

	// First insert the new key at the end
	heap_size++;
	int i = heap_size - 1;
	heap[i] = New_event;

	// Fix the min heap property if it is violated
	while (i != 0 && compare(heap[parent(i)], heap[i]) > 0) {
		swap(&heap[i], &heap[parent(i)]);
		i = parent(i);
	}
}

void heap_calendar::heapify(int i) {
	int l = left(i);
	int r = right(i);
	int smallest = i;

	if (l < heap_size && compare(heap[l], heap[i]) < 0)
		smallest = l;
	if (r < heap_size && compare(heap[r], heap[smallest]) < 0)
		smallest = r;
	if (smallest != i) {
		swap(&heap[i], &heap[smallest]);
		heapify(smallest);
	}
}

