#include "prefix_sum.h"
#include "helpers.h"


work_descriptor::work_descriptor(int numData, int totalThreads, int tid)
: num_data(numData), total_threads(totalThreads), my_tid(tid)
{
    assert(tid < totalThreads);
    levels = next_power_of_two_level(num_data);
    int stride = 1;
    for (int i = 0; i < levels; i++)
    {
        int totalWork = num_data - stride;
        int baseWork = totalWork / total_threads;
        int unevenWork = totalWork % total_threads;
        int start, end; //Do not work on index end

        //compute start and end idex on every level, load balance for each thread
        if (my_tid < unevenWork) {
            start = my_tid * (baseWork + 1);
            end = start + baseWork + 1;
        }
        else {
            start = unevenWork * (baseWork + 1) + (my_tid - unevenWork) * baseWork;
            end = start + baseWork;
        }
        work_EachStep.emplace_back(std::pair<int,int>(start, end));

        int totalCopy = stride - stride/2;
        int baseCopy = totalCopy / total_threads;
        int copyStart = stride/2 + my_tid * baseCopy;
        int copyEnd = (my_tid == total_threads -1) ? stride : copyStart + baseCopy;
        copy_EachStep.emplace_back(std::pair<int,int>(copyStart, copyEnd));

        stride *= 2;
    }
}

//delete this     
void work_descriptor::showMyWork() {
    printf("I am thread %d\n", my_tid);
    for (int i = 0; i < levels; i ++)
    {
        int stride = pow(2, i);
        printf("On step %d I work on index %d to %d, stride %d\n", i, work_EachStep[i].first, work_EachStep[i].second, stride);
    }
}



void* compute_prefix_sum(void *a)
{
    prefix_sum_args_t *args = (prefix_sum_args_t *)a;

    /************************
     * Your code here...    *
     * or wherever you like *
     ************************/


    work_descriptor my_work_descriptor(args->n_vals, args->n_threads, args->t_id);

    int stride = 1;
    int *my_input, *my_output, *temp;
    my_input = args->buffer;
    my_output = args->output_vals;

    for (int level = 0; level < my_work_descriptor.levels; level++)
    {
        for (int i = my_work_descriptor.work_EachStep[level].first; i < my_work_descriptor.work_EachStep[level].second; i++)
        {
            my_output[i + stride] = args->op(my_input[i], my_input[i + stride], args->n_loops);
            //my_output[i] = my_input[i];
            printf("Level %d result %d is %d\n", level, i+stride, my_output[i + stride]);
            
        }
        for (int i = my_work_descriptor.copy_EachStep[level].first; i < my_work_descriptor.copy_EachStep[level].second; i++)
        {
            my_output[i] = my_input[i];
        }

        stride *= 2;

        

        temp = my_output;
        my_output = my_input;
        my_input = temp;

        pthread_barrier_wait(&args->barrier);

    }

    if (args->t_id == 0){
        args->output_vals = my_input;
        buffer = my_output;
    }


    return 0;
}
