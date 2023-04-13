//#include "C.operate.h"
//#include "C.millerrabin.h"
//
//int main(int argc, char* argv[])
//{
//    Num p;
//    srand((unsigned int)(time(NULL)));
//
//    LENGTH = atoi(argv[1]);
//    int numOfPrimes = atoi(argv[2]);
//    int testTimes = atoi(argv[3]);
//
//    int generatedPrimes = 0;
//    clock_t t = clock();
//    clock_t totalTime = 0;
//
//    while (generatedPrimes < numOfPrimes)
//    {
//        random(&p, LENGTH);
//
//        if (millerRabin(&p, testTimes) == true)
//        {
//            printf("Prime: \n");
//            print(&p);
//            printf("\n");
//            generatedPrimes++;
//            totalTime += clock() - t;
//            printf("Number of Prime: %d\n", generatedPrimes);
//            t = clock();
//        }
//        free(p.arr);
//    }
//
//    printf("Total time of %d prime numbers: %f s\n", numOfPrimes, 1.0 * totalTime / CLOCKS_PER_SEC);
//
//    return 0;
//}

// 多线程
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "C.operate.h"
#include "C.millerrabin.h"

typedef struct {
    int numOfPrimes;
    int testTimes;
    int *generatedPrimes;
    pthread_mutex_t *mutex;
} ThreadData;

void *generate_primes(void *arg) {
    ThreadData *data = (ThreadData *)arg;
    Num p;
    srand((unsigned int)(time(NULL)));

    while (1) {
        random(&p, LENGTH);

        if (millerRabin(&p, data->testTimes) == true) {
            pthread_mutex_lock(data->mutex);

            if (*(data->generatedPrimes) >= data->numOfPrimes) {
                pthread_mutex_unlock(data->mutex);
                free(p.arr);
                break;
            }

            printf("Prime: \n");
            print(&p);
            printf("\n");
            *(data->generatedPrimes) += 1;
            printf("Number of Prime: %d\n", *(data->generatedPrimes));

            pthread_mutex_unlock(data->mutex);
        }
        free(p.arr);
    }
    return NULL;
}

int main(int argc, char* argv[]) {
    if (argc < 5) {
        printf("Usage: %s <length> <numOfPrimes> <testTimes> <numOfThreads>\n", argv[0]);
        return 1;
    }

    LENGTH = atoi(argv[1]);
    int numOfPrimes = atoi(argv[2]);
    int testTimes = atoi(argv[3]);
    int numOfThreads = atoi(argv[4]);

    pthread_t *threads = (pthread_t *)malloc(numOfThreads * sizeof(pthread_t));
    ThreadData *threadData = (ThreadData *)malloc(numOfThreads * sizeof(ThreadData));
    int generatedPrimes = 0;
    pthread_mutex_t mutex;

    if (pthread_mutex_init(&mutex, NULL) != 0) {
        perror("pthread_mutex_init");
        return 1;
    }

    clock_t startTime = clock(); // 记录开始时间

    for (int i = 0; i < numOfThreads; i++) {
        threadData[i].numOfPrimes = numOfPrimes;
        threadData[i].testTimes = testTimes;
        threadData[i].generatedPrimes = &generatedPrimes;
        threadData[i].mutex = &mutex;

        if (pthread_create(&threads[i], NULL, generate_primes, &threadData[i]) != 0) {
            perror("pthread_create");
            return 1;
        }
    }

    for (int i = 0; i < numOfThreads; i++) {
        if (pthread_join(threads[i], NULL) != 0) {
            perror("pthread_join");
            return 1;
        }
    }

    clock_t endTime = clock(); // 记录结束时间

    double totalTime = (double)(endTime - startTime) / CLOCKS_PER_SEC; // 计算总时间
    printf("Total number of primes: %d\n", generatedPrimes);
    printf("Total time for generating primes: %f seconds\n", totalTime); // 打印总时间

    pthread_mutex_destroy(&mutex);
    free(threads);
    free(threadData);

    return 0;
}

