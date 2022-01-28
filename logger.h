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

using bulk_t = std::queue<bulk::Cmd>;

class ILogger
{
public:
    virtual void process(const bulk_t &bulk) = 0;
    virtual void worker() = 0;
    virtual void stop() = 0;
};

class Logger : public ILogger
{
public:
    Logger();
    virtual ~Logger() = default;

protected:
    std::queue<bulk_t> m_bulks;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::atomic<bool> m_stopped;
    std::vector<std::thread> m_threads;

    static void processBulk(std::ostream &os, bulk_t &bulk);

};

class Console : public Logger
{
public:
    Console(std::ostream &os = std::cout);
    ~Console();
    void process(const std::queue<bulk::Cmd> &cmds) override;
    void stop() override;

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
    void process(const std::queue<bulk::Cmd> &cmds) override;

private:
    std::ofstream m_logFile;
    std::string m_logFileName;
    std::thread m_thread1;
    std::thread m_thread2;

    void worker() override;
};

//using LogPtr = std::unique_ptr<ILogger>;
using LogPtr = std::unique_ptr<Logger>;
//using LogPtr = Logger*;

}

