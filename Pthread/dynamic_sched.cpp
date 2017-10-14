#include <iostream>
#include <pthread.h>
#include <cstdlib> 
#include <string.h>
#include <chrono>
#include <cmath>

#ifdef __cplusplus
extern "C" {
#endif

float f1(float x, int intensity);
float f2(float x, int intensity);
float f3(float x, int intensity);
float f4(float x, int intensity);

#ifdef __cplusplus
}
#endif

// global variables and mutex;
static int n, functionid,intens,tdnum,gran;
static int global_start = 0;
static bool loop_done = false;
static float a, b;
pthread_mutex_t mutexsum;
pthread_mutex_t mutexloop;
float iter_sum = 0;
float chunk_sum = 0;
// function choice
float (*f[4])(float,int) = {f1,f2,f3,f4};

// sum function
float sumfunc(int start, int end){
 float sum = 0;
 float xp = 0;
 float temp = 0;   
 for(int i = start; i < end; i++ ){   
 xp = a + (i+0.5)*(b-a)/n;
 temp += (*f[(functionid-1)])(xp,intens);
 sum = temp*(b-a)/n;
 }
 return sum;
}

//getnext function

void getnext(int* start, int* end){
    *start = global_start;
    *end =  *start+ gran;
    pthread_mutex_lock(&mutexloop);
    global_start += gran;
    pthread_mutex_unlock(&mutexloop);
   if(*end < n && *end >= 0){
       loop_done = false;
   } else {
       *end = n;
       loop_done = true;
   }
}


// threading functions
void * thread(void* t_id){
    float sum = 0;
    float local_sum = 0;
    while(!loop_done){
    int start,end;
    getnext(&start,&end);
    sum = sumfunc(start,end); 
    local_sum += sum;
    }
    float * t = new float(local_sum);
    pthread_exit((void*)t);
}

//iteration functions
void * iter(void* t_id){
    float sum = 0;
    while (!loop_done){    
    float xp = 0;
    float temp = 0; 
    int start,end;
    getnext(&start,&end);
    // lock every sum result  
    for(int i = start; i < end; i++ ){   
      xp = a + (i+0.5)*(b-a)/n;
      temp = (*f[(functionid-1)])(xp,intens);
      sum = temp*(b-a)/n;
      pthread_mutex_lock(&mutexsum);
      iter_sum += sum;
      pthread_mutex_unlock(&mutexsum);
     }
   }
    pthread_exit(NULL);
}

    
  void * chunk(void* t_id){
    float sum = 0;
    while(!loop_done){
    int start,end;
    getnext(&start,&end);
    sum = sumfunc(start,end); 
    pthread_mutex_lock(&mutexsum);
    chunk_sum += sum;
    pthread_mutex_unlock(&mutexsum);
    }
    pthread_exit(NULL);
  }

  
int main (int argc, char* argv[]) {

  if (argc < 9) {
    std::cerr<<"usage: "<<argv[0]<<" <functionid> <a> <b> <n> <intensity> <nbthreads> <sync> <granularity>"<<std::endl;
    return -1;
  }

  // initiate global variables
  functionid = atoi(argv[1]);
  a = atof(argv[2]);
  b = atof(argv[3]);
  n = atoi(argv[4]);
  intens = atoi(argv[5]);
  tdnum = atoi(argv[6]); 
  int td_method;
  gran = atoi(argv[8]);
          
  /* threading methods*/
  // passing thread numbers
   pthread_t threads[tdnum];
   
  //Choose methods
   if(strcmp(argv[7], "iteration") == 0) {
      td_method= 0;
  } else if(strcmp(argv[7], "thread") == 0){
      td_method = 1;
  } else {
      td_method = 2;
  }
   
   void* (*method[3])(void *){iter,thread,chunk};
   
    // Start counting time
    std::chrono::time_point<std::chrono::system_clock> start = std::chrono::system_clock::now();
    
   // Creating threads  
    for(int t_id=0; t_id<tdnum; t_id++){
   int * i = new int(t_id);
    pthread_create(&threads[t_id], NULL, method[td_method], i);
    }
    
  // Joining threads 
   
   switch(td_method){
       case 0: // iteration join 
           for(int t_id=0; t_id<tdnum; t_id++) {
               pthread_join(threads[t_id],NULL);
           }
               std::cout<<iter_sum<<std::endl;
           break;
      
       case 1: // thread join
           float thread_sum;
           for(int t_id=0; t_id<tdnum; t_id++) {
      
               float *status;
               pthread_join(threads[t_id], (void**) &status);
               thread_sum += *status; 
           }
               std::cout<<thread_sum<<std::endl;
           break;   
       case 2: // chunk join
           for(int t_id=0; t_id<tdnum; t_id++) {
               pthread_join(threads[t_id],NULL);
           }
               std::cout<<chunk_sum<<std::endl;
          break;     
  }
   std::chrono::time_point<std::chrono::system_clock> end = std::chrono::system_clock::now();
   std::chrono::duration<double> elapsed_seconds = end-start;
   std::cerr<<elapsed_seconds.count()<<std::endl;
   return 0;
}
