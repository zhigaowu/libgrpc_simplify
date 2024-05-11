/*
 * ==========================================================================
 *
 *       Filename:  test_cases.cpp
 *
 *    Description:  test cases
 *
 *        Version:  1.0
 *        Created:  2024-02-27 16:22:54
 *       Revision:  none
 *       Compiler:  g++
 *
 *         Author:  
 * ==========================================================================
 */

#include "logger/glog_logger.h"

#include <memory>
#include <thread>
#include <filesystem>

extern int test_sync(int argc, char **argv) ;
extern int test_async(int argc, char **argv) ;

int main(int argc, char **argv) 
{
    std::filesystem::path param_path(argv[0]);
    glog::Logger::Instance().Initialize(argv[0], std::filesystem::current_path().string() + "/logs", param_path.filename().string());

    int res = test_async(argc, argv);

    return res;
}
