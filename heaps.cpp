

void add_to_max_heap(int index, int dist, int *heap, int *heap_p, int size, int &count) {
	++count;

	heap[count] = index;
	heap_p[count] = dist;

	int idx = count;

	while (idx > 1 and heap_p[idx>>1] < heap_p[idx]) {
		register int tmp = heap[idx];
		heap[idx] = heap[idx>>1];
		heap[idx>>1] = tmp;

		tmp = heap_p[idx];
		heap_p[idx] = heap_p[idx>>1];
		heap_p[idx>>1] = tmp;

		idx >>= 1;
	}	
}

void pop_max_heap(int *heap, int *heap_p, int size, int &count) {
	if (count > 1) {
		// add the last element on top of the heap and remove the max element
		// TO DO: swap last element with first in order to obtain heapsort 
		heap[1] = heap[count];
		heap_p[1] = heap_p[count];
		--count;

		// go down until balanced
		int idx = 1;
		while (true) {
			int max_p_idx = idx;
			if (idx * 2 <= count and
				heap_p[idx * 2] > heap_p[max_p_idx]) {
				max_p_idx = idx * 2;
			}

			if (idx * 2 + 1 <= count and
				heap_p[idx * 2 + 1] > heap_p[max_p_idx]) {
				max_p_idx = idx * 2 + 1;
			}

			if (max_p_idx == idx) {
				break ;
			} else {
				register int tmp = heap[idx];
				heap[idx] = heap[max_p_idx];
				heap[max_p_idx] = tmp;

				tmp = heap_p[idx];
				heap_p[idx] = heap_p[max_p_idx];
				heap_p[max_p_idx] = tmp;

				idx = max_p_idx;
			}
		}
	} else {
		count = 0;
	}
}
