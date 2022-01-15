#include <gtest/gtest.h>

#include <iostream>
#include "handler.h"

TEST(HandlerTest, Empty)
{
    std::istringstream iss("cmd1\n");
    std::ostringstream oss;
    bulk::Handler handler(3, iss);
    handler.registerLogger(logger::LogPtr{new logger::Console(oss)});
    EXPECT_TRUE(oss.str() == "");
}

TEST(HandlerTest, Simple)
{
    std::istringstream iss("cmd1\n");
    std::ostringstream oss;
    bulk::Handler handler(3, iss);
    handler.registerLogger(logger::LogPtr{new logger::Console(oss)});
    handler.start();
    EXPECT_TRUE(oss.str() == "bulk: cmd1\n");
}

TEST(HandlerTest, TaskCase1)
{
    std::istringstream iss("cmd1\n"
                           "cmd2\n"
                           "cmd3\n"
                           "cmd4\n"
                           "cmd5\n");
    std::ostringstream oss;
    bulk::Handler handler(3, iss);
    handler.registerLogger(logger::LogPtr{new logger::Console(oss)});
    handler.start();

//    std::cout << iss.str() << std::endl;
//    std::cout << oss.str() << std::endl;

    EXPECT_TRUE(oss.str() == "bulk: cmd1, cmd2, cmd3\n"
                             "bulk: cmd4, cmd5\n");
}

TEST(HandlerTest, TaskCase2)
{
    std::istringstream iss("cmd1\n"
                           "cmd2\n"
                           "{\n"
                           "cmd3\n"
                           "cmd4\n"
                           "}\n"
                           "{\n"
                           "cmd5\n"
                           "cmd6\n"
                           "{\n"
                           "cmd7\n"
                           "cmd8\n"
                           "}\n"
                           "cmd9\n"
                           "}\n"
                           "{\n"
                           "cmd10\n"
                           "cmd11\n"
                           );
    std::ostringstream oss;
    bulk::Handler handler(3, iss);
    handler.registerLogger(logger::LogPtr{new logger::Console(oss)});
    handler.start();

//    std::cout << iss.str() << std::endl;
//    std::cout << oss.str() << std::endl;

    EXPECT_TRUE(oss.str() == "bulk: cmd1, cmd2\n"
                             "bulk: cmd3, cmd4\n"
                             "bulk: cmd5, cmd6, cmd7, cmd8, cmd9\n");
}

