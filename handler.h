#pragma once

#include <iostream>
#include <stack>
#include <queue>
#include <list>
#include "logger.h"
#include "assignments.h"
#include "state.h"

namespace bulk {

class Handler
{
public:

    // High level logic
    Handler(const size_t &bulkSize, std::istream &is = std::cin);
    void reveiveCmd(const Cmd &cmd);
    void receiveCmdEof();
//    void start();

    // State Pattern side
    void setState(StateBasePtr state);

    // Properties
    size_t bulkSize() const;
    size_t cmdsSize() const;
    size_t bracketsSize() const;

    // Handler functionality
    void pushOpenedBracket();
    void popOpenedBracket();
    void pushCmd(const Cmd &cmd);
    void processBulk();

private:
    std::istream &m_is;
    size_t m_bulkSize;
    std::stack<Bracket> m_brackets;
    std::queue<Cmd> m_cmds;
    std::list<logger::LogPtr> m_loggers;
    StateBasePtr m_state;

    void registerLogger(logger::LogPtr logger);
    static bool isOpenedBracket(const Cmd &cmd);
    static bool isClosedBracket(const Cmd &cmd);
    static bool isAnyBracket(const Cmd &cmd, Bracket anyBracket);

};

}

