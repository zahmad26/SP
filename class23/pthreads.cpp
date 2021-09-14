// #include <stdio.h>
// #include <stdlib.h>
// #include <pthread.h>

// void *print_message_function( void *ptr );

// main()
// {
//      pthread_t thread1, thread2;
//      char *message1 = "Thread 1";
//      char *message2 = "Thread 2";
//      int  iret1, iret2;

//     /* Create independent threads each of which will execute function */

//      iret1 = pthread_create( &thread1, NULL, print_message_function, (void*) message1);
//      iret2 = pthread_create( &thread2, NULL, print_message_function, (void*) message2);

//      /* Wait till threads are complete before main continues. Unless we  */
//      /* wait we run the risk of executing an exit which will terminate   */
//      /* the process and all threads before the threads have completed.   */

//      pthread_join( thread1, NULL);
//      pthread_join( thread2, NULL);

//      printf("Thread 1 returns: %d\n",iret1);
//      printf("Thread 2 returns: %d\n",iret2);
//      exit(0);
// }

// void *print_message_function( void *ptr )
// {
//      char *message;
//      message = (char *) ptr;
//      printf("%s \n", message);
// }

#include <iostream>
#include <cstdlib>
#include <pthread.h>

using namespace std;

#define NUM_THREADS 5

void *PrintHello(void *threadid)
{
   long tid;
   tid = (long)threadid;
   cout << "Hello World! Thread ID, " << tid << endl;
   pthread_exit(NULL);
}

int main()
{
   // pthread_t threads[NUM_THREADS];
   // int rc;
   // int i;

   // for( i = 0; i < NUM_THREADS; i++ ) {
   //    cout << "main() : creating thread, " << i << endl;
   //    rc = pthread_create(&threads[i], NULL, PrintHello, (void *)i);

   //    if (rc) {
   //       cout << "Error:unable to create thread," << rc << endl;
   //       exit(-1);
   //    }
   // }
   // pthread_exit(NULL);

   int x;
   pthread_t thread1;
   pthread_attr_t attr;
   pthread_attr_init(&attr);

   x = pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
   if (x > 0)
   {
      perror("detach");
   }
   x = pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);
   if (x != 0)
   {
      perror("scope");
   }
   x = pthread_create(&thread1, &attr, thread_runner, NULL);
   if (x != 0)
   {
      perror("create");
   }
}