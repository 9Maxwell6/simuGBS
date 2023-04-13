file(GLOB SRCS *
            ${CMAKE_CURRENT_LIST_DIR}/utils/* )


add_library(src ${SRCS})
add_executable(simuGBS ${SRCS})
target_link_libraries(simuGBS PRIVATE OpenMP::OpenMP_CXX)




