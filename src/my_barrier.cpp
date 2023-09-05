#include <my_barrier.h>


/************************
 * Your code here...    *
 * or wherever you like *
 ************************/

 class my_barrier{
     std::atomic<int> counter;
     std::atomic<int> go;
     int n_threads;

     my_barrier(int n){
         n_threads=n;
         std::atomic_init(&counter, 0);
         std::atomic_init(&go, 0);
         
     }

     void my_barrier_wait(){
         int local_go = std::atomic_load(&go);
         int local_counter = std::atomic_fetch_add(&counter, 1);
         if (local_counter+1 == n_threads){
             std::atomic_store(&counter, 0);
             std::atomic_fetch_xor(&go, 1);
         }
         else{
             while(local_go == std::atomic_load(&go))
             {
             }
         }
         
     }
 };