#include <iostream>

#include <stdio.h>

#include "mu_test.h"
#include "Nmutex.h"
#include "nthread.h"
#include "guard.h"
#include "atomicOperations.h"

#ifdef __linux__
#include <unistd.h>

#elif defined WIN32
#include <Windows.h>

#endif /* os */

using namespace advcpp;
using namespace advcpp::sync;

const unsigned int g_num_iterations = 1000;

class AddOne
{
public:
	AddOne(unsigned int* _data): m_data(_data){}
	void RunGuard()
	{
		for(unsigned int i = 0; i < g_num_iterations; ++i)
    	{
			Guard nadav(AddOne::m_mutex);
			++(*m_data);
		}
	}
	
	void RunMutex()
	{
		for(unsigned int i = 0; i < g_num_iterations; ++i)
    	{
			m_mutex.Lock();
			(++*((int*)m_data));
			m_mutex.Unlock();
		}
	}
	
	/*void RunCond()
	{
		m_mutex.Lock();
		(++*((int*)m_data));
		m_cond.Wait(AddOne::m_mutex);
		m_mutex.Unlock();
	}
	void BroadCast()
	{
	    m_cond.Broadcast();
	}*/
private:
	static Nmutex m_mutex;
	unsigned* m_data;
	
public:
	//static advcpp::Ncond m_cond;
};


/*--------------------------------------------*/

class Here
{
public:
    Here(int* _data): m_data(_data){}
    void Run()
    {
        for(unsigned int i = 0; i < g_num_iterations; ++i)
    	{
            AtomicAdd(*m_data, 1);
        }
    }
    int* m_data;
};

Nmutex AddOne::m_mutex;
//advcpp::Ncond AddOne::m_cond;

/*-------------------------------------------*/

UNIT(THREADS)

int data = 0;
const unsigned int numTreads = 100;

Nthread* threads[numTreads];

Here here(&data);
for(unsigned int i = 0; i < numTreads; ++i)
{
    threads[i] = new Nthread(&Here::Run, here);
}

for(unsigned int i = 0; i < numTreads; ++i)
{
    threads[i]->Join();
    delete threads[i];
}

ASSERT_THAT(numTreads * g_num_iterations == data);

END_UNIT

/*------------------------------------------*/

UNIT(mutex_test)

const unsigned int numThreads = 1000;
unsigned int data = 0;

Nthread* threads[numThreads];

for(unsigned int i = 0; i < numThreads; ++i)
{
    AddOne one(&data);
    threads[i] = new Nthread(&AddOne::RunMutex, one);
}

for(unsigned int i = 0; i < numThreads; ++i)
{
    threads[i]->Join();
    delete threads[i];
}

ASSERT_THAT(numThreads * g_num_iterations == data);

END_UNIT

/*----------------------------------*/
UNIT(guard_test)

const unsigned int numThreads = 1000;
unsigned int data = 0;

Nthread* threads[numThreads];
AddOne one(&data);

for(unsigned int i = 0; i < numThreads; ++i)
{
    threads[i] = new Nthread(&AddOne::RunGuard, one);
}

for(unsigned int i = 0; i < numThreads; ++i)
{
    threads[i]->Join();
    delete threads[i];
}

ASSERT_THAT(numThreads * g_num_iterations == data);
END_UNIT

/*-----------------------------------------------------*/

TEST_SUITE(RAII)

TEST(THREADS)
TEST(mutex_test)
TEST(guard_test)
//TEST(cond_test)

END_SUITE
