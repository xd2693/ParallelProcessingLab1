#pragma once

#include <stdlib.h>
#include <pthread.h>
#include <spin_barrier.h>
#include <iostream>
#include <vector>
#include <assert.h>
//delete this
#include <math.h>

void* compute_prefix_sum(void* a);

struct work_descriptor
{
    int num_data;
    int total_threads;
    int my_tid;
    int levels;
    std::vector<std::pair<int, int>> work_EachStep;
    std::vector<std::pair<int, int>> copy_EachStep;
    
    inline int next_power_of_two_level(int x) {
        int pow = 1;
        int level = 0;
        while (pow < x) {
            pow *= 2;
            level ++;
        }
        return level;
    }

    work_descriptor(int numData, int totalThreads, int tid);
    
    
    void showMyWork();
};