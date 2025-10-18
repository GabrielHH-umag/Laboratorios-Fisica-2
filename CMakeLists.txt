cmake_minimum_required(VERSION 3.14)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(external/matplotplusplus)
add_executable(main main.cpp)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data_sin_linealizar_1.txt ${CMAKE_CURRENT_BINARY_DIR}/data_sin_linealizar.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data_linealizada_1.txt ${CMAKE_CURRENT_BINARY_DIR}/data_linealizada.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data_sin_ampolleta_2.txt ${CMAKE_CURRENT_BINARY_DIR}/data_linealizada.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data_con_ampolleta_sin_lin2.txt ${CMAKE_CURRENT_BINARY_DIR}/data_linealizada.txt COPYONLY)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/data_con_ampolleta_lin2.txt ${CMAKE_CURRENT_BINARY_DIR}/data_linealizada.txt COPYONLY)
target_link_libraries(main PUBLIC matplot)