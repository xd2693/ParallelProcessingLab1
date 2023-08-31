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
    int sum=0;

    for(int i = t_id * n_vals / (n_threads+1); i < (t_id+1) * n_vals / (n_threads+1); i++)
    {
        sum+= input[i];
        output[i] = sum;
    }
    sum_offsets[t_id] = sum;
    sum = 0;

    pthread_barrier_wait(args->barrier);

    if(t_id==0){
       for(int i = 0; i < n_threads; i++)
       {
            sum+= sum_offsets[i];
            sum_offsets[i] = sum;
       } 
       
    } 

    pthread_barrier_wait(args->barrier);

    if(t_id<n_threads-1){
        for(int i = (t_id + 1) * n_vals / (n_threads+1); i < (t_id+2) * n_vals / (n_threads+1); i++)
        {            
            output[i]+=sum_offsets[t_id];
        }
    }
    else{
        sum = sum_offsets[t_id];
        for(int i = (t_id + 1) * n_vals / (n_threads+1); i < n_vals; i++)
        {
            sum+= input[i];
            output[i] = sum;
        }
    }
    

}