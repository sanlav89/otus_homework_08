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
    ~Logger();

    void process(const bulk_t &cmds) override;
    virtual void worker() override;
    void stop() override;

protected:
    std::queue<bulk_t> m_bulks;
    std::condition_variable m_cv;
    std::mutex m_mutex;
    std::atomic<bool> m_stopped;
    std::vector<std::thread> m_threads;

    void processOne(std::ostream &os, bulk_t &bulk);

};

class Console : public Logger
{
public:
    Console(std::ostream &os = std::cout);

private:
    std::ostream &m_os;
    void worker() override;

    void processBulk(std::ostream &os, bulk_t &bulk);

};

class LogFile : public Logger
{
public:
    LogFile();

private:
    std::ofstream m_logFile;
    std::string m_logFileName;

    void worker() override;

    void openNewLogFile();
};

using LogPtr = std::unique_ptr<Logger>;

}

