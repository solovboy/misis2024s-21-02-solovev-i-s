include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(main main.cpp)

target_include_directories(main PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}/filter")

add_library(filter filter/filter.hpp filter/filter.cpp)

target_link_libraries(main filter ${OpenCV_LIBS})