#ifdef _DEBUG
#include <stdio.h>
#endif

#include "bass_crossfade.h"
#include "crossfade_queue.h"
#include "queue.h"

QUEUE* queue = NULL;

BOOL crossfade_queue_create() {
	if (queue) {
		return FALSE;
	}
	queue = queue_create(MAX_CHANNELS, TRUE);
	if (!queue) {
#if _DEBUG
		printf("Failed to create queue.\n");
#endif
		return FALSE;
	}
	return TRUE;
}

BOOL crossfade_queue_contains(const HSTREAM handle) {
	if (!queue) {
		return FALSE;
	}
	return queue_contains(queue, handle);
}

BOOL crossfade_queue_enqueue(const HSTREAM handle) {
	if (!queue) {
		return FALSE;
	}
#if _DEBUG
	printf("Enqueuing channel: %d\n", handle);
#endif
	return queue_enqueue(queue, handle, TRUE);
}

BOOL crossfade_queue_dequeue(HSTREAM* const handle) {
	if (!queue) {
		return FALSE;
	}
	if (!queue_dequeue(queue, handle)) {
		return FALSE;
	}
#if _DEBUG
	printf("Dequeuing channel: %d\n", *handle);
#endif
	return TRUE;
}

BOOL crossfade_queue_push(const HSTREAM handle) {
	if (!queue) {
		return FALSE;
	}
#if _DEBUG
	printf("Enqueuing channel: %d\n", handle);
#endif
	return queue_push(queue, handle, TRUE);
}

BOOL crossfade_queue_peek(HSTREAM* const handle) {
	if (!queue) {
		return FALSE;
	}
	return queue_peek(queue, handle);
}

BOOL crossfade_queue_remove(const HSTREAM handle) {
	if (!queue) {
		return FALSE;
	}
#if _DEBUG
	printf("Removing channel: %d\n", handle);
#endif
	return queue_remove(queue, handle);
}

BOOL crossfade_queue_is_empty(BOOL* const empty) {
	if (!queue) {
		return FALSE;
	}
	return queue_is_empty(queue, empty);
}

BOOL crossfade_queue_count(DWORD* const length) {
	if (!queue) {
		return FALSE;
	}
	*length = queue->length;
	return TRUE;
}

HSTREAM* crossfade_queue_get_all(DWORD* const length) {
	static HSTREAM buffer[MAX_CHANNELS];
	if (!queue) {
		return NULL;
	}
	if (!queue_get_all(queue, buffer, length)) {
		*length = 0;
	}
	return buffer;
}

BOOL crossfade_queue_clear() {
	return crossfade_queue_free() && crossfade_queue_create();
}

BOOL crossfade_queue_free() {
	if (!queue) {
		return FALSE;
	}
	queue_free(queue);
	queue = NULL;
	return TRUE;
}