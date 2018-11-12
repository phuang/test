cmake_minimum_required(VERSION 3.1)
project(test_clang)
set(CMAKE_CXX_STANDARD 14)

Find_Package(LLVM REQUIRED CONFIG)
message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION} in ${LLVM_INSTALL_PREFIX}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

Find_Package(Clang REQUIRED CONFIG HINTS
        "${LLVM_INSTALL_PREFIX}/lib/cmake/clang")
message(STATUS "Found Clang in ${CLANG_INSTALL_PREFIX}")

add_executable(
        test_clang clang.cc
        browser_ast_consumer.cc
        browser_ast_consumer.h
        browser_ast_visitor.cc
        browser_ast_visitor.h)

target_link_libraries(
        test_clang PRIVATE
        clangFrontend
        clangParse
        clangSema
        clangAST
        clangBasic
        clangLex
        clangTooling)

target_link_libraries(test_clang PRIVATE LLVM)

#if (TARGET LLVM)
#target_link_libraries(test_clang PRIVATE LLVM)
#else()
#llvm_map_components_to_libnames(llvm_libs core support)
#target_link_libraries(test_clang PRIVATE ${llvm_libs})
#endif()

target_include_directories(
        test_clang PUBLIC ${CLANG_INCLUDE_DIRS})
