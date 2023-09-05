#pragma once

#include "operators.h"
#include <stdlib.h>
#include <pthread.h>
#include <my_barrier.h>

struct prefix_sum_args_t {
  int*               input_vals;
  int*               output_vals;
  int*               sum_offsets;
  bool               spin;
  int                n_vals;
  int                n_threads;
  int                t_id;
  int (*op)(int, int, int);
  int n_loops;
  pthread_barrier_t* barrier;
  my_barrier* counter_barrier;
};

prefix_sum_args_t* alloc_args(int n_threads);

int next_power_of_two(int x);

void fill_args(prefix_sum_args_t *args,
               int n_threads,
               int n_vals,
               int *inputs,
               int *outputs,
               int *sum_offsets,
               bool spin,
               int (*op)(int, int, int),
               int n_loops,
               pthread_barrier_t *barrier,
               my_barrier *counter_barrier);
