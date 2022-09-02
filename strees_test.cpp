#include "SkipList.h"
#include <cstdlib>
#include <pthread.h>


#define NUM_THREADS 1//线程数量
#define TEST_COUNT 100000//执行次数
SkipList<int,int> sl(16);


void *insertElement(void* threadid) {
    long tid; 
    tid = (long long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; //每一个线程执行的次数
	for(int i=0,tempKey=tid*tmp;i<tmp;i++,tempKey++)
        sl.insertElement(tempKey,i);
    pthread_exit(NULL);
}

void *getElement(void* threadid) {
    long tid; 
    tid = (long long)threadid;
    std::cout << tid << std::endl;  
    int tmp = TEST_COUNT/NUM_THREADS; 
	for(int i=0,tempKey=tid*tmp+1;i<tmp;i++,tempKey++)
        sl.searchElement(tempKey);
    pthread_exit(NULL);
}



int main()
{
    {

        pthread_t threads[NUM_THREADS];
        int rc;
        int i;

        auto start = std::chrono::high_resolution_clock::now();

        for( i = 0; i < NUM_THREADS; i++ ) {
            std::cout << "main() : creating thread, " << i << std::endl;
            rc = pthread_create(&threads[i], NULL, insertElement, (void *)i);

            if (rc) {
                std::cout << "Error:unable to create thread," << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                perror("pthread_create() error"); 
                exit(3);
            }
        }
        auto finish = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "insert elapsed:" << elapsed.count() << std::endl;
    }

    {
        pthread_t threads[NUM_THREADS];
        int rc;
        int i;
        auto start = std::chrono::high_resolution_clock::now();

        for( i = 0; i < NUM_THREADS; i++ ) {
            std::cout << "main() : creating thread, " << i << std::endl;
            rc = pthread_create(&threads[i], NULL, getElement, (void *)i);

            if (rc) {
                std::cout << "Error:unable to create thread," << rc << std::endl;
                exit(-1);
            }
        }

        void *ret;
        for( i = 0; i < NUM_THREADS; i++ ) {
            if (pthread_join(threads[i], &ret) !=0 )  {
                perror("pthread_create() error"); 
                exit(3);
            }
        }

        auto finish = std::chrono::high_resolution_clock::now(); 
        std::chrono::duration<double> elapsed = finish - start;
        std::cout << "get elapsed:" << elapsed.count() << std::endl;
    }

    cout<<"size:"<<sl.size()<<endl;
    cout<<"height:"<<sl.height()<<endl;
    return 0;
}
