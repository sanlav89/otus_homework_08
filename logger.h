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
    virtual void process(const std::queue<bulk::Cmd> &cmds) = 0;
};

class Console : public ILogger
{
public:
    Console(std::ostream &os = std::cout);
    ~Console();
    void process(const std::queue<bulk::Cmd> &cmds) override;

private:
    std::ostream &m_os;
    std::queue<bulk::Cmd> m_cmds;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::thread m_thread;
    std::atomic<bool> m_stopped;

    void worker();
};

class LogFile : public ILogger
{
public:
    LogFile();
    ~LogFile();
    void process(const std::queue<bulk::Cmd> &cmds) override;

private:
    std::ofstream m_logFile;
    std::string m_logFileName;
    std::queue<bulk::Cmd> m_cmds;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::thread m_thread1;
    std::thread m_thread2;
    std::atomic<bool> m_stopped;

    void worker();
};

using LogPtr = std::unique_ptr<ILogger>;

}

