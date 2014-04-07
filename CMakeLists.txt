CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

PROJECT(threadtests)

set( CMAKE_CXX_FLAGS "-Wall -m64 -g -O3 -std=c++11 -lpthread" )

# boost requirements
if(APPLE)
set(Boost_USE_STATIC_LIBS   ON)
else(APPLE)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC")
endif(APPLE)

find_package(Boost 1.48.0 COMPONENTS  system  thread REQUIRED)

# notes: 
# http://stackoverflow.com/questions/1937232/linking-to-a-dynamic-library-on-a-mac-with-full-path
# we tried : http://stackoverflow.com/questions/7233383/cmake-on-mac-os-x-link-libraries-with-fullpath but with no real success


include_directories(${Boost_INCLUDE_DIRS})

add_executable(benchmark
   benchmark.cc
)


TARGET_LINK_LIBRARIES( benchmark ${Boost_LIBRARIES} )
