#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

pthread_mutex_t RESOURCE_MUTEX;
pthread_cond_t CV_READER;
pthread_cond_t CV_WRITER;

int reader_cnt = 0;
int writer_cnt = 0;

void read() {
    //printf("Reading.\n");
}

void write() {
    // printf("Writing.\n");
}

void *reader() {
    for (int i = 0; i < 20000000; i++) {
        if (reader_cnt < 0) {
            printf("Serious error. There can be 0 or more writers writing at the same moment.\n");
            exit(1);
        }
        pthread_mutex_lock(&RESOURCE_MUTEX);
        while (writer_cnt > 0) {
            printf("\tReader waiting. %d writers active.\n", writer_cnt);
            pthread_cond_wait(&CV_READER, &RESOURCE_MUTEX);
        }
        reader_cnt++;
        pthread_mutex_unlock(&RESOURCE_MUTEX);
        read();
        pthread_mutex_lock(&RESOURCE_MUTEX);
        reader_cnt--;
        if (reader_cnt == 0)
            pthread_cond_signal(&CV_WRITER);
        pthread_mutex_unlock(&RESOURCE_MUTEX);
    }
}

void *writer() {
    for (int i = 0; i < 2000000; i++) {
        if (writer_cnt > 1) {
            printf("Serious error. There can be 0 or 1 writer writing at the same moment.\n");
            exit(1);
        }
        pthread_mutex_lock(&RESOURCE_MUTEX);
        while (reader_cnt > 0) {
            printf("Writer waiting. %d readers active.\n", reader_cnt);
            pthread_cond_wait(&CV_WRITER, &RESOURCE_MUTEX);
        }
        writer_cnt++;
        pthread_mutex_unlock(&RESOURCE_MUTEX);
        write();
        pthread_mutex_lock(&RESOURCE_MUTEX);
        writer_cnt--;
        pthread_mutex_unlock(&RESOURCE_MUTEX);
        pthread_cond_broadcast(&CV_READER);
    }
}

int main() {
    printf("Started.\n");
    pthread_t writer_thread;
    pthread_t reader_thread_1;
    pthread_t reader_thread_2;
    pthread_t reader_thread_3;
    pthread_t reader_thread_4;
    pthread_t reader_thread_5;
    pthread_t reader_thread_6;

    pthread_create(&writer_thread, NULL, writer, NULL);
    pthread_create(&reader_thread_1, NULL, reader, NULL);
    pthread_create(&reader_thread_2, NULL, reader, NULL);
    pthread_create(&reader_thread_3, NULL, reader, NULL);
    pthread_create(&reader_thread_4, NULL, reader, NULL);
    pthread_create(&reader_thread_5, NULL, reader, NULL);
    pthread_create(&reader_thread_6, NULL, reader, NULL);

    pthread_join(writer_thread, NULL);
    pthread_join(reader_thread_1, NULL);
    pthread_join(reader_thread_2, NULL);
    pthread_join(reader_thread_3, NULL);
    pthread_join(reader_thread_4, NULL);
    pthread_join(reader_thread_5, NULL);
    pthread_join(reader_thread_6, NULL);

    printf("Finished.\n");
}
