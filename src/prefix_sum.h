#pragma once

#include <stdlib.h>
#include <pthread.h>
#include <my_barrier.h>
#include <iostream>



void* compute_prefix_sum(void* a);
