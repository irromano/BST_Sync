#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>

#include "BinaryTree.h"

#define ONE_K 10
#define ONE_HUNDRED_K 1000000
#define THREAD_COUNT 1

/* Mutex used for synchronization */
pthread_mutex_t lock;
static struct p *root;

/**
 * Workload for lab3. requres srand(time(NULL)) call in main.
 */
void workLoad()
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
        root = removeVal(key, root);
        printf("Iteration %d, root %d, key %d\r\n", i, root->v, key);
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
    root = (struct p *)malloc(sizeof(struct p));
    int rootKey = rand() % 16384 + 1; /* Random number between 1 to 16384 */
    root->v = rootKey;
    pthread_t tid;

    // Creating Threads
    for (int i = 0; i < THREAD_COUNT; i++)
        pthread_create(&tid, NULL, workLoad, NULL);

    pthread_exit(NULL);

    return 0;
}