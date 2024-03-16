#include <windows.h>

typedef struct QUEUE_NODE QUEUE_NODE;

typedef struct QUEUE_NODE {
	DWORD data;
	QUEUE_NODE* previous;
};

typedef struct {
	QUEUE_NODE* head;
	QUEUE_NODE* tail;
	DWORD length;
	DWORD capacity;
	HANDLE lock;
} QUEUE;

QUEUE* queue_create(const DWORD capacity, const BOOL synchronized);

void queue_free(QUEUE* queue);

BOOL queue_contains(const QUEUE* const queue, const DWORD data);

BOOL queue_enqueue(QUEUE* const queue, const DWORD data, const BOOL unique);

BOOL queue_dequeue(QUEUE* const queue, DWORD* const data);

BOOL queue_push(QUEUE* const queue, const DWORD data, const BOOL unique);

BOOL queue_peek(const QUEUE* const queue, DWORD* const data);

BOOL queue_is_empty(const QUEUE* const queue, BOOL* const empty);

BOOL queue_get_all(const QUEUE* const queue, DWORD* const data, DWORD* const length);

BOOL queue_remove(QUEUE* const queue, const DWORD data);