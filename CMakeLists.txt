project(benchmarks)

cmake_minimum_required(VERSION 3.4)


#########
# nt2

include(ExternalProject)

ExternalProject_Add( NT2Project
  SOURCE_DIR         ${CMAKE_CURRENT_SOURCE_DIR}/nt2
  BINARY_DIR         ${CMAKE_CURRENT_BINARY_DIR}/nt2/build
  CONFIGURE_COMMAND  ${CMAKE_COMMAND} -DNT2_VERSION_STRING="git" -DBOOST_ROOT=/home/tim/boost_1_61_0 ${CMAKE_CURRENT_SOURCE_DIR}/nt2
  BUILD_COMMAND      ${CMAKE_COMMAND} --build ${CMAKE_CURRENT_BINARY_DIR}/nt2/build --target stage
  INSTALL_COMMAND    "")

add_library( NT2 INTERFACE )
target_include_directories( NT2 INTERFACE ${CMAKE_CURRENT_BINARY_DIR}/nt2/build/include )
add_dependencies( NT2 NT2Project )
target_compile_definitions( NT2 INTERFACE BOOST_SIMD_NO_STRICT_ALIASING BOOST_SIMD_NO_NANS )


add_executable( benchmarks main.cpp multiply.cpp )

target_include_directories( benchmarks PUBLIC /home/tim/boost_1_61_0 )
target_link_libraries( benchmarks PUBLIC NT2 )
target_compile_options( benchmarks PUBLIC -march=native )

set_target_properties( benchmarks PROPERTIES
  CXX_STANDARD 14
  VISIBILITY_INLINES_HIDDEN ON)
