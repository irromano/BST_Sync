#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "BinaryTree.h"

#define ONE_K 1000
#define ONE_HUNDRED_K 100000
#define THREAD_COUNT 16

/* Mutex used for synchronization */
pthread_mutex_t lock;

/**
 * Workload for lab3. requres srand(time(NULL)) call in main.
 */
void workLoad(struct p *root)
{
    pthread_mutex_lock(&lock);

    for (int i = 0; i < ONE_K; i++)
    {
        int key = rand() % 16384 + 1; /* Random number between 1 to 16384 */
        add(key, root);
    }

    for (int i = 0; i < ONE_HUNDRED_K; i++)
    {
        int key = rand() % 16384 + 1; /* Random number between 1 to 16384 */
        add(key, root);
        removeVal(key, root);
    }

    printf("The size of the array is %d\r\n", size(root));
    if (checkIntegrity(root))
        printf("The integrity check passed\r\n");
    else
        printf("The integrity check failed\r\n");

    pthread_mutex_unlock(&lock);
}

int main()
{
    pthread_mutex_init(&lock, NULL);

    srand(time(NULL));
    struct p *root = (struct p *)malloc(sizeof(struct p));
    pthread_t tid;

    // Let us create three threads
    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_create(&tid, NULL, workLoad, root);

    pthread_exit(NULL);

    return 0;
}