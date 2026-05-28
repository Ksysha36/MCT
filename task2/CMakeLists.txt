cmake_minimum_required(VERSION 3.2)
project(Poisson2D)

set(INMOST_ROOT /home/ksu/MSU_6_sem/SVT/INMOST-install)

include_directories(${INMOST_ROOT}/include)
link_directories(${INMOST_ROOT}/lib)

add_executable(poisson2d src/main.cpp)

target_link_libraries(poisson2d ${INMOST_ROOT}/lib/libinmost.a pthread m)