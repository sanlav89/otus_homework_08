#include <gtest/gtest.h>

#include <iostream>
#include "handler.h"

TEST(HandlerTest, Empty)
{
    std::ostringstream oss;
    bulk::Handler handler(3, oss);

    handler.receiveCmdEof();

    EXPECT_TRUE(oss.str() == "");
}

TEST(HandlerTest, Simple)
{
    std::vector<bulk::Cmd> cmds = {
        "cmd1"
    };

    std::ostringstream oss;
    bulk::Handler handler(3, oss);

    for (const auto &cmd : cmds) {
        handler.reveiveCmd(cmd);
    }
    handler.receiveCmdEof();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_TRUE(oss.str() == "bulk: cmd1\n");
}

TEST(HandlerTest, TaskCase1)
{
    std::vector<bulk::Cmd> cmds = {
        "cmd1",
        "cmd2",
        "cmd3",
        "cmd4",
        "cmd5"
    };

    std::ostringstream oss;
    bulk::Handler handler(3, oss);

    for (const auto &cmd : cmds) {
        handler.reveiveCmd(cmd);
    }
    handler.receiveCmdEof();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_TRUE(oss.str() == "bulk: cmd1, cmd2, cmd3\n"
                             "bulk: cmd4, cmd5\n");
}


TEST(HandlerTest, TaskCase2)
{
    std::vector<bulk::Cmd> cmds = {
        "cmd1",
        "cmd2",
        "{",
        "cmd3",
        "cmd4",
        "}",
        "{",
        "cmd5",
        "cmd6",
        "{",
        "cmd7",
        "cmd8",
        "}",
        "cmd9",
        "}",
        "{",
        "cmd10",
        "cmd11"
    };

    std::ostringstream oss;
    bulk::Handler handler(3, oss);

    for (const auto &cmd : cmds) {
        handler.reveiveCmd(cmd);
    }
    handler.receiveCmdEof();

    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    EXPECT_TRUE(oss.str() == "bulk: cmd1, cmd2\n"
                             "bulk: cmd3, cmd4\n"
                             "bulk: cmd5, cmd6, cmd7, cmd8, cmd9\n");
}
