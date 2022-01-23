#pragma once

#include <fstream>
#include <memory>
#include <iostream>
#include <queue>

#include "assignments.h"

namespace logger {

class ILogger
{
public:
    virtual void process(const std::queue<bulk::Cmd> &cmds) = 0;
};

class Console : public ILogger
{
public:
    Console(std::ostream &os = std::cout);
//    void pushCmd(const bulk::Cmd &cmd) override;
    void process(const std::queue<bulk::Cmd> &cmds) override;

private:
    std::ostream &m_os;
    std::queue<bulk::Cmd> m_cmds;
};

class LogFile : public ILogger
{
public:
    LogFile() = default;
//    void pushCmd(const bulk::Cmd &cmd) override;
    void process(const std::queue<bulk::Cmd> &cmds) override;

private:
    std::ofstream m_logFile;
    std::string m_logFileName;
    std::queue<bulk::Cmd> m_cmds;
};

using LogPtr = std::unique_ptr<ILogger>;

}

