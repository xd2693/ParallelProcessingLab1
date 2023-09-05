#ifndef _MY_BARRIER_H
#define _MY_BARRIER_H

#include <pthread.h>
#include <iostream>
#include <atomic>
class my_barrier;

class my_barrier{
    std::atomic<int> counter;
    std::atomic<int> go;
    int n_threads;
public:
    void init(int n);

    void my_barrier_wait();
};


#endif
