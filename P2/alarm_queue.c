/* Function implementation in alarm_queue.h */

#include "alarm_queue.h"
#include <stdio.h>
#include <stdlib.h>

int next_alarm_id = 0;

/* 
 * Create a new alarm_queue 
 * Return NULL on failure, pointer to queue on success
 */
 alarm_queue_t 
 alarm_queue_new() {
	alarm_queue_t new_queue = (alarm_queue_t) malloc(sizeof(struct alarm_queue));
	if (new_queue == NULL) {
		fprintf(stderr, "Can't create alarm queue!");
		return NULL;
	}
	
	new_queue->head = NULL;
	new_queue->tail = NULL;
	new_queue->length = 0;
	
	return new_queue;
}

/*
 * Insert an alarm into alarm queue
 * The queue should be sorted by fire time
 * Return 0 on success, -1 on failure
 */
int 
alarm_queue_insert(alarm_queue_t alarm_queue, void* data) {
	alarm_t alarm = (alarm_t) data;
	alarm_t cur;
	
	if (alarm_queue == NULL) {
		return -1;
	}
	
	/* Empty queue */
	if (alarm_queue->head == NULL && alarm_queue->tail == NULL) {
		alarm_queue->head = alarm;
		alarm_queue->tail = alarm;
		alarm_queue->length += 1;
		return 0;
	}
	
	/* Find the insertion point */
	for (cur = alarm_queue->head; cur != NULL && cur->time_to_fire < alarm->time_to_fire; cur = cur->next);
	
	/* Insert before head */
	if (cur == alarm_queue->head) {
		alarm->prev = NULL;
		alarm->next = cur;
		alarm_queue->head = data;
		alarm_queue->length += 1;
		return 0;
	}
	
	/* Insert after tail */
	if (cur == NULL) {
		alarm_queue->tail->next = data;
		alarm->prev = alarm_queue->tail;
		alarm->next = NULL;
		alarm_queue->tail = data;
		alarm_queue->length += 1;
		return 0;
	}
	
	/* Insert in middle */
	cur->prev->next = data;
	alarm->prev = cur->prev;
	alarm->next = cur;
	cur->prev = data;
	alarm_queue->length += 1;
	return 0;
}

/*
 * Dequeue an alarm from alarm queue
 * Return 0 on success, -1 on failure
 */
int 
alarm_queue_dequeue(alarm_queue_t alarm_queue, void **data) {
	if (data == NULL) {
		return -1;
	}
	if (alarm_queue == NULL) {
		*data = NULL;
		return -1;
	}
	
	*data = (void*) alarm_queue->head;
	if (*data == NULL) {
		return -1;
	}
	
	alarm_queue_delete(alarm_queue, data);
}

/*
 * Delete an alarm from alarm queue
 * Return 0 on success, -1 on failure
 */
int 
alarm_queue_delete(alarm_queue_t alarm_queue, void **data) {
	alarm_t alarm;
	
	if (alarm_queue == NULL || data == NULL || *data == NULL) {
		return -1;
	}
	
	alarm = (alarm_t) *data;
	/* Head */
	if (alarm == alarm_queue->head) {
		alarm_queue->head = alarm->next;
	}
	/* Tail */
	if (alarm == alarm_queue->tail) {
		alarm_queue->tail = alarm->prev;
	}
	
	if (alarm->prev) {
		alarm->prev->next = alarm->next;
	}
	if (alarm->next) {
		alarm->next->prev = alarm->prev;
	}
	
	alarm->prev = NULL;
	alarm->next = NULL;
	alarm_queue->length--;
	
	return 0;
}

/*
 * Delete an alarm from alarm queue based on its alarm id
 * Return 0 on success, -1 on failure
 */
int 
alarm_queue_delete_by_id(alarm_queue_t alarm_queue, int alarm_id, void **data) {
	alarm_t alarm;
	int ret;
	
	if (alarm_queue == NULL || data == NULL) {
		return -1;
	}
	
	/* Find alarm with alarm_id */
	for (alarm = alarm_queue->head; alarm != NULL && alarm->alarm_id != alarm_id; alarm = alarm->next);
	/* Delete it */
	ret = alarm_queue_delete(alarm_queue, (void**)&alarm);
	*data = alarm;
	
	return ret;
}

/*
 * Return the queue length
 * Return -1 if the queue is NULL
 */
int 
alarm_queue_length(alarm_queue_t alarm_queue) {
	if (alarm_queue == NULL) {
		return -1;
	}
	return alarm_queue->length;
}

/*
 * Free an alarm queue
 */
int 
alarm_queue_free(alarm_queue_t alarm_queue) {
	return -1;
}