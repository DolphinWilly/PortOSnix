/* Retail shop application and multiple function definitions */
#include <stdlib.h>
#include <stdio.h>
#include "queue.h"
#include "minithread.h"
#include "synch.h"

/* Edit the number of employees (N) and customers (M) here. */
static const int EMPLOYEE_NUM = 7;
static const int CUSTOMER_NUM = 31;

/* Current serial number and IDs, output starts from 1. */
static int serial_num = 0;
static int customer_id = 0;
static int employee_id = 0;

/* Serial number of the currently unpacked phone, output starts from 1. */
static int unpacked = 0;

/*
 * Phone semaphore, employees cannot unpack the next phone until
 * a customer receives the currently unpacked phone.
 */
static semaphore_t phone_sem;
/* Employee semaphore, employees work when there are customers. */
static semaphore_t employee_sem;
/* Binary semaphore, used for mutual exclusion. */
static semaphore_t binary_sem;
/* Checkout semaphore, only one customer can checkout at a given time. */
static semaphore_t checkout_sem;

static int
employee(int* arg) {
    int id;
    semaphore_P(binary_sem);
    id = ++employee_id;
    printf("Employee %d starts working at the store.\n", id);
    semaphore_V(binary_sem);

    while (1) {
        /* Wait if there is no customer */
        semaphore_P(employee_sem);
        printf("Employee %d unpacked phone %d\n", id, unpacked = ++serial_num);
        /* Tell the customer the phone is ready */
        semaphore_V(phone_sem);
    }

    return 0;
}

static int
customer(int* arg) {
    int id;
    semaphore_P(binary_sem);
    id = ++customer_id;
    printf("Customer %d arrives at the store.\n", id);
    semaphore_V(binary_sem);

    /* Only one customer can be in the checkout process */
    semaphore_P(checkout_sem);
    /* Tell employee there is a customer */
    semaphore_V(employee_sem);
    /* Wait for unpacked phone */
    semaphore_P(phone_sem);
    printf("Customer %d activated phone %d\n", id, unpacked);
    /* Wakes up next customer */
    semaphore_V(checkout_sem);

    return 0;
}

static int
start(int* arg) {
    int i;
    for (i = 0; i < EMPLOYEE_NUM; i++)
        minithread_fork(employee, NULL);
    for (i = 0; i < CUSTOMER_NUM; i++)
        minithread_fork(customer, NULL);
    return 0;
}

int
main(int argc, char** argv) {
    /* Semaphore creation and initialization. */
    phone_sem = semaphore_create();
    employee_sem = semaphore_create();
    binary_sem = semaphore_create();
    checkout_sem = semaphore_create();
    semaphore_initialize(phone_sem, 0);
    semaphore_initialize(employee_sem, 0);
    semaphore_initialize(binary_sem, 1);
    semaphore_initialize(checkout_sem, 1);
    /* Start main thread. */
    minithread_system_initialize(start, NULL);

    return 0;
}

