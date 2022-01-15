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
    void registerLogger(logger::LogPtr logger);
    void start();

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
    void openLog();
    void closeLog();

private:
    std::istream &m_is;
    size_t m_bulkSize;
    std::queue<Cmd> m_cmds;
    std::stack<Bracket> m_brackets;
    std::list<logger::LogPtr> m_loggers;
    StateBasePtr m_state;

    static bool isOpenedBracket(const Cmd &cmd);
    static bool isClosedBracket(const Cmd &cmd);
    static bool isAnyBracket(const Cmd &cmd, Bracket anyBracket);

};

}

