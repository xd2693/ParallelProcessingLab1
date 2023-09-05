#include "prefix_sum.h"
#include "helpers.h"


void* compute_prefix_sum(void *a)
{
    prefix_sum_args_t *args = (prefix_sum_args_t *)a;

    /************************
     * Your code here...    *
     * or wherever you like *
     ************************/
    int *input, *output, *sum_offsets;
    input = args->input_vals;
    output = args->output_vals;
    sum_offsets = args->sum_offsets;
    int t_id = args->t_id;
    int n_threads = args->n_threads;
    int n_vals = args->n_vals;
    int loop = args->n_loops;
    int (*op)(int, int, int) = args->op;
    int sum = 0;
    int work_perthread = n_vals / (n_threads+1); //devide input data into threads+1 data blocks

    //Calculate and store the prefix sum in every data block except last data block
    for(int i = t_id * work_perthread; i < (t_id+1) * work_perthread; i++)
    {
        sum = op(input[i], sum, loop);
        output[i] = sum;
    }
    //store total sum of data block in sum_offsets
    sum_offsets[t_id] = sum;
    sum = 0;

    //printf("spin %d", args->spin);

    if(args->spin){
        args->counter_barrier->my_barrier_wait();
        //printf("in my barrier");
    }
    else{
        pthread_barrier_wait(args->barrier);
    }


    //accumulate and store sums in sum_offsets
    if(t_id==0){
       for(int i = 0; i < n_threads; i++)
       {
            sum = op(sum, sum_offsets[i], loop);
            sum_offsets[i] = sum;
       } 
       
    } 

    if(args->spin){
        args->counter_barrier->my_barrier_wait();
    }
    else{
        pthread_barrier_wait(args->barrier);
    }

    //for threads 0 through n_threads-2, add the sum from sum_offsets to output starting
    //from second data block, last thread needs to calculate accumulated sum for last block
    if(t_id < n_threads-1){
        for(int i = (t_id + 1) * work_perthread; i < (t_id+2) * work_perthread; i++)
        {            
            output[i] = op(output[i], sum_offsets[t_id], loop);
        }
    }
    else{
        sum = sum_offsets[t_id];
        for(int i = (t_id + 1) * work_perthread; i < n_vals; i++)
        {
            sum = op(sum, input[i], loop);
            output[i] = sum;
        }
    }
    
    return 0;

}