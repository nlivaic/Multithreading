#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define BUF_SIZE 6

pthread_mutex_t PTHREAD_BUFFER_MUTEX;
pthread_cond_t PTHREAD_COND_PROD;
pthread_cond_t PTHREAD_COND_CONS;

int add = 0;
int rem = 0;
int num = 0;

int buffer[BUF_SIZE];

void *producer() {
    for (int i = 0; i < 20; i++) {
        printf("Producer:: iteration %d.\n", i);
        if (num > BUF_SIZE) {
            printf("Buffer overflow.\n");
            exit(1);
        }
        pthread_mutex_lock(&PTHREAD_BUFFER_MUTEX);
        while (num == BUF_SIZE) {
            printf("Producer waits.");
            pthread_cond_wait(&PTHREAD_COND_PROD, &PTHREAD_BUFFER_MUTEX);
        }
        buffer[add] = i;
        printf("Producer:: added to buffer: %d.\n", i);
        add = (add + 1) % BUF_SIZE;
        num++;
        pthread_mutex_unlock(&PTHREAD_BUFFER_MUTEX);
        pthread_cond_signal(&PTHREAD_COND_CONS);
    }

    return 0;
}

void *consumer() {
    int item;
    while (1) {
        if (num < 0) {
            printf("Buffer cannot have less than zero items.\n");
            exit(1);
        }
        pthread_mutex_lock(&PTHREAD_BUFFER_MUTEX);
        while (num == 0) {
            printf("Consumer waits.");
            pthread_cond_wait(&PTHREAD_COND_CONS, &PTHREAD_BUFFER_MUTEX);
        }
        item = buffer[rem];
        rem = (rem + 1) % BUF_SIZE;
        printf("\t\t\tConsumer:: Consumed %d.\n", item);
        num--;
        pthread_mutex_unlock(&PTHREAD_BUFFER_MUTEX);
        pthread_cond_signal(&PTHREAD_COND_PROD);
    }

    return 0;
}

int main() {
    pthread_t thread1;
    pthread_t thread2;

    pthread_create(&thread1, NULL, producer, NULL);
    pthread_create(&thread2, NULL, consumer, NULL);

    pthread_join(thread1, NULL);
    pthread_join(thread2, NULL);

    printf("Parent thread quiting.");
    return 0;
}
