// Priority Readers and Writers
// Write a multi-threaded C program that gives readers priority over writers concerning a shared (global) variable. Essentially, if any readers are waiting, then they have priority over writer threads -- writers can only write when there are no readers. This program should adhere to the following constraints:
// 
// * Multiple readers/writers must be supported (5 of each is fine)
// * Readers must read the shared variable X number of times
// * Writers must write the shared variable X number of times
// * Readers must print:
//     * The value read
//     * The number of readers present when value is read
// * Writers must print:
//     * The written value
//     * The number of readers present were when value is written (should be 0)
// * Before a reader/writer attempts to access the shared variable it should wait some random amount of time
//     * Note: This will help ensure that reads and writes do not occur all at once
// * Use pthreads, mutexes, and condition variables to synchronize access to the shared variable

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t RESOURCE_MUTEX;
pthread_cond_t CV_READER;
pthread_cond_t CV_WRITER;

int reader_cnt = 0;
int writer_cnt = 0;
int value = 0;

void read() {
    printf("\tStart reading: %d [%d].", value, reader_cnt);
    printf("\tEnd reading.\n");
}

void write(int write_value) {
     printf("Start writing: %d [%d].", write_value, reader_cnt);
     value = write_value;
     printf("End writing.\n");
}

void *reader() {
    struct timespec nanosleep_time = { 0, 1000 };
    for (int i = 0; i < 2000; i++) {
        if (reader_cnt < 0) {
            printf("Serious error. There can be 0 or more readers reading at the same moment.\n");
            exit(1);
        }
        // nanosleep(&nanosleep_time, &nanosleep_time);
        pthread_mutex_lock(&RESOURCE_MUTEX);
        // while (writer_cnt > 0) {
        //     printf("\tReader waiting. %d writers active.\n", writer_cnt);
        //     pthread_cond_wait(&CV_READER, &RESOURCE_MUTEX);
        // }
        reader_cnt++;
        pthread_mutex_unlock(&RESOURCE_MUTEX);
        read();
        pthread_mutex_lock(&RESOURCE_MUTEX);
        reader_cnt--;
        if (reader_cnt == 0)
        {
            // printf("\t!!! Readers done! Broadcasting to all blocked writers!\n");
            pthread_cond_broadcast(&CV_WRITER);
        }
        pthread_mutex_unlock(&RESOURCE_MUTEX);
    }
}

void *writer() {
    struct timespec nanosleep_time = { 0, 1000 };
    for (int i = 0; i < 2000; i++) {
        // nanosleep(&nanosleep_time, &nanosleep_time);
        pthread_mutex_lock(&RESOURCE_MUTEX);
        while (reader_cnt > 0) {
            printf("Writer waiting. %d readers active.\n", reader_cnt);
            pthread_cond_wait(&CV_WRITER, &RESOURCE_MUTEX);
        }
        writer_cnt++;
        pthread_mutex_unlock(&RESOURCE_MUTEX);
        write(value + 1);
        pthread_mutex_lock(&RESOURCE_MUTEX);
        writer_cnt--;
        if (writer_cnt == 0)
        {
            // printf("!!! Writers done! Broadcasting to all blocked readers!\n");
            pthread_cond_broadcast(&CV_READER);
        }
        pthread_mutex_unlock(&RESOURCE_MUTEX);
    }
}

int main() {
    printf("Started.\n");
    pthread_t writer_thread_1;
    pthread_t writer_thread_2;
    pthread_t writer_thread_3;
    pthread_t writer_thread_4;
    pthread_t writer_thread_5;
    pthread_t reader_thread_1;
    pthread_t reader_thread_2;
    pthread_t reader_thread_3;
    pthread_t reader_thread_4;
    pthread_t reader_thread_5;

    pthread_create(&writer_thread_1, NULL, writer, NULL);
    pthread_create(&writer_thread_2, NULL, writer, NULL);
    pthread_create(&writer_thread_3, NULL, writer, NULL);
    pthread_create(&writer_thread_4, NULL, writer, NULL);
    pthread_create(&writer_thread_5, NULL, writer, NULL);
    pthread_create(&reader_thread_1, NULL, reader, NULL);
    pthread_create(&reader_thread_2, NULL, reader, NULL);
    pthread_create(&reader_thread_3, NULL, reader, NULL);
    pthread_create(&reader_thread_4, NULL, reader, NULL);
    pthread_create(&reader_thread_5, NULL, reader, NULL);

    pthread_join(writer_thread_1, NULL);
    pthread_join(writer_thread_2, NULL);
    pthread_join(writer_thread_3, NULL);
    pthread_join(writer_thread_4, NULL);
    pthread_join(writer_thread_5, NULL);
    pthread_join(reader_thread_1, NULL);
    pthread_join(reader_thread_2, NULL);
    pthread_join(reader_thread_3, NULL);
    pthread_join(reader_thread_4, NULL);
    pthread_join(reader_thread_5, NULL);

    printf("Finished.\n");
}
