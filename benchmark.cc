#include <iostream>
#include <future>

#include <vector>
#include <chrono>
#include "threadpool.hpp"

#include <boost/date_time/posix_time/posix_time.hpp>


namespace bt = boost::posix_time; 
const size_t SIZE = 1e6; 
const int OPERATIONS = 10000; 

void sum_task(size_t start, size_t end, double * result) { 

    double sum = 0.0; 
    for(size_t i = start; i < end; ++i) { 
        sum += i; 
    }
    *result = sum; 
}


void threadpool_benchmark() { 
    
    using namespace boost::threadpool;
    for (int tpsize = 1; tpsize < 9; ++tpsize) { 
        pool tp(tpsize);

        bt::ptime mst1 = bt::microsec_clock::local_time();

        std::vector<double> results(OPERATIONS); 
        for(int i = 0; i < OPERATIONS; ++i) { 
            tp.schedule([&results, i](){ sum_task(i*SIZE, (i+1)*SIZE, 
                                                  &results[i]);}); 
        }
        tp.wait(); 
    
        bt::ptime mst2 = bt::microsec_clock::local_time();
        bt::time_duration msdiff = mst2 - mst1;

        std::cout << "tp size=" << tpsize 
                  << " " << msdiff.total_milliseconds() << " ms "
                  << std::endl; 
   }

}

void async_benchmark() { 


    bt::ptime mst1 = bt::microsec_clock::local_time();
    std::vector<std::future<void> > results; 
    std::vector<double> outputs(OPERATIONS); 
    
    for(int i = 0; i < OPERATIONS; ++i) { 
        results.push_back(std::async(std::launch::async, 
                                     [&outputs, i](){ 
                                         sum_task(i*SIZE, 
                                                  (i+1)*SIZE, 
                                                  &outputs[i]);}));
    }
    // get the results
    for(int i = 0; i < OPERATIONS; ++i) { 
        results[i].get(); 
    }
    
    bt::ptime mst2 = bt::microsec_clock::local_time();
    bt::time_duration msdiff = mst2 - mst1;
    
    std::cout << "async time ="
              << msdiff.total_milliseconds() << " ms "
              << std::endl; 
    
}

int main()
{
    threadpool_benchmark(); 
    async_benchmark();

}
