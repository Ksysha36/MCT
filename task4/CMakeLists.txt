cmake_minimum_required(VERSION 3.2)
project(AddNormalToMesh)

set(INMOST_ROOT /home/ksu/MSU_6_sem/SVT/INMOST-install)

include_directories(${INMOST_ROOT}/include)

add_executable(add_normals add_normals_inmost.cpp)

target_link_libraries(add_normals ${INMOST_ROOT}/lib/libinmost.a pthread m)