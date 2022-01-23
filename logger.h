#pragma once

#include <fstream>
#include <memory>
#include <iostream>
#include <queue>
#include <condition_variable>
#include <thread>
#include <atomic>
#include "assignments.h"

namespace logger {

class ILogger
{
public:
    virtual void process() = 0;
    virtual void pushCmd(const bulk::Cmd &cmd) = 0;
    virtual void worker() = 0;
};

class Logger : public ILogger
{
public:
    Logger();
    virtual ~Logger() = default;

    void pushCmd(const bulk::Cmd &cmd) override;

protected:
    std::queue<bulk::Cmd> m_cmds;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::atomic<bool> m_paused;
};

class Console : public Logger
{
public:
    Console(std::ostream &os = std::cout);
    ~Console();
    void process() override;

private:
    std::ostream &m_os;
    std::thread m_thread;

    void worker() override;

};

class LogFile : public Logger
{
public:
    LogFile();
    ~LogFile();
    void process() override;

private:
    std::ofstream m_logFile;
    std::string m_logFileName;
    std::thread m_thread1;
    std::thread m_thread2;

    void worker() override;
};

using LogPtr = std::unique_ptr<Logger>;

}

