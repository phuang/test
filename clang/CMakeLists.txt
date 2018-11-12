cmake_minimum_required(VERSION 3.1)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(GNUInstallDirs)
set (CMAKE_CXX_STANDARD 14)

add_subdirectory(test)

# 
# install(DIRECTORY data
#     DESTINATION ${CMAKE_INSTALL_DATADIR}/woboq
# )


