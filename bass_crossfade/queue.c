#ifdef _DEBUG
#include <stdio.h>
#endif

#include "queue.h"

#define QUEUE_LOCK_TIMEOUT 1000

QUEUE* queue_create(const DWORD capacity, const BOOL synchronized) {
	QUEUE* queue = calloc(sizeof(QUEUE), 1);
	queue->capacity = capacity;
	if (synchronized) {
		queue->lock = CreateSemaphore(NULL, 1, 1, NULL);
		if (!queue->lock) {
#if _DEBUG
			printf("Failed to create semaphore.\n");
#endif
		}
	}
	return queue;
}

void queue_free(QUEUE* queue) {
	DWORD data;
	while (queue_dequeue(queue, &data)); 
	if (queue->lock) {
		if (!CloseHandle(queue->lock)) {
#if _DEBUG
			printf("Failed to release semaphore.\n");
#endif
		}
		else {
			queue->lock = NULL;
		}
	}
	free(queue);
	queue = NULL;
}

BOOL queue_enter(const QUEUE* const queue) {
	DWORD result;
	if (queue->lock) {
		result = WaitForSingleObject(queue->lock, QUEUE_LOCK_TIMEOUT);
		if (result == WAIT_OBJECT_0) {
			return TRUE;
		}
		else {
#if _DEBUG
			printf("Failed to enter semaphore.\n");
#endif
			return FALSE;
		}
	}
	return TRUE;
}

void queue_exit(const QUEUE* const queue) {
	if (queue->lock) {
		if (!ReleaseSemaphore(queue->lock, 1, NULL)) {
#if _DEBUG
			printf("Failed to exit semaphore.\n");
#endif
		}
	}
}

static BOOL _queue_contains(const QUEUE* const queue, const DWORD data) {
	QUEUE_NODE* node;
	if (queue->length > 0) {
		node = queue->head;
		while (node) {
			if (node->data == data) {
				return TRUE;
			}
			node = node->previous;
		}
	}
	return FALSE;
}

BOOL queue_contains(const QUEUE* const queue, const DWORD data) {
	BOOL success;
	if (!queue_enter(queue)) {
		return FALSE;
	}
	success = _queue_contains(queue, data);
	queue_exit(queue);
	return success;
}

BOOL queue_enqueue(QUEUE* const queue, const DWORD data, const BOOL unique) {
	BOOL success;
	QUEUE_NODE* node;
	if (!queue_enter(queue)) {
		return FALSE;
	}
	if (queue->length == queue->capacity) {
		success = FALSE;
	}
	else {
		if (unique && _queue_contains(queue, data)) {
			success = FALSE;
		}
		else {
			node = calloc(sizeof(QUEUE_NODE), 1);
			node->data = data;
			node->previous = NULL;
			if (queue->length == 0) {
				queue->head = node;
				queue->tail = node;
			}
			else {
				queue->tail->previous = node;
				queue->tail = node;
			}
			queue->length++;
			success = TRUE;
		}
	}
	queue_exit(queue);
	return success;
}

BOOL queue_dequeue(QUEUE* const queue, DWORD* const data) {
	BOOL success;
	QUEUE_NODE* node;
	if (!queue_enter(queue)) {
		return FALSE;
	}
	if (queue->length == 0) {
		success = FALSE;
	}
	else {
		node = queue->head;
		queue->head = node->previous;
		queue->length--;
		free(node);
		*data = node->data;
		success = TRUE;
	}
	queue_exit(queue);
	return success;
}

BOOL queue_push(QUEUE* const queue, const DWORD data, const BOOL unique) {
	BOOL success;
	QUEUE_NODE* node;
	if (!queue_enter(queue)) {
		return FALSE;
	}
	if (unique && _queue_contains(queue, data)) {
		success = FALSE;
	}
	else {
		node = calloc(sizeof(QUEUE_NODE), 1);
		node->data = data;
		node->previous = NULL;
		if (queue->length == 0) {
			queue->head = node;
			queue->tail = node;
		}
		else {
			node->previous = queue->head;
			queue->head = node;
		}
		queue->length++;
		success = TRUE;
	}
	queue_exit(queue);
	return success;
}

BOOL queue_peek(const QUEUE* const queue, DWORD* const data) {
	BOOL success = FALSE;
	if (!queue_enter(queue)) {
		return FALSE;
	}
	if (queue->length > 0) {
		*data = queue->head->data;
		success = TRUE;
	}
	queue_exit(queue);
	return success;
}

BOOL queue_is_empty(const QUEUE* const queue, BOOL* const empty) {
	if (!queue_enter(queue)) {
		return FALSE;
	}
	*empty = queue->length == 0;
	queue_exit(queue);
	return TRUE;
}

BOOL queue_get_all(const QUEUE* const queue, DWORD* const data, DWORD* const length) {
	DWORD a;
	QUEUE_NODE* node;
	BOOL success;
	if (!queue_enter(queue)) {
		return FALSE;
	}
	if (queue->length == 0) {
		success = FALSE;
	}
	else {
		node = queue->head;
		*length = queue->length;
		for (a = 0; a < *length; a++) {
			data[a] = node->data;
			node = node->previous;
		}
		success = TRUE;
	}
	queue_exit(queue);
	return success;
}

BOOL queue_remove(QUEUE* const queue, const DWORD data) {
	QUEUE_NODE* head;
	QUEUE_NODE* tail;
	BOOL success = FALSE;
	if (!queue_enter(queue)) {
		return FALSE;
	}
	if (queue->length == 0) {
		success = FALSE;
	}
	else {
		head = queue->head;
		tail = queue->tail;
		if (head->data == data) {
			queue->head = queue->head->previous;
			free(head);
			success = TRUE;
		}
		else {
			while (head && head->data != data) {
				tail = head;
				head = head->previous;
			}
			if (head) {
				tail->previous = head->previous;
				if (queue->tail == head) {
					queue->tail = tail;
				}
				free(head);
				success = TRUE;
			}
		}
		if (success) {
			queue->length--;
		}
	}
	queue_exit(queue);
	return success;
}