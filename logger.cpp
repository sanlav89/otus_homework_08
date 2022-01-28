#include "logger.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace logger;

Logger::Logger() : m_stopped(false)
{
}

Logger::~Logger()
{
    stop();
}

void Logger::process(const bulk_t &cmds)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    m_bulks.push(cmds);
    m_cv.notify_all();
}

void Logger::worker()
{
}

void Logger::stop()
{
    if (!m_stopped) {
        m_stopped = true;
        m_cv.notify_all();
        for (auto &thread : m_threads) {
            thread.join();
        }
    }
}

void Logger::processOne(std::ostream &os, bulk_t &bulk)
{
    os << bulk.front();
    bulk.pop();
    if (!bulk.empty()) {
        os << ", ";
    }
}

Console::Console(std::ostream &os) : Logger(), m_os(os)
{
    m_threads.push_back(std::thread(&Console::worker, this));
}

void Console::worker()
{
    while (!m_stopped) {

        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this]() { return m_stopped || !m_bulks.empty(); });

        while (!m_bulks.empty()) {
            processBulk(m_os, m_bulks.front());
            m_bulks.pop();
        }

        if (m_stopped) {
            break;
        }
    }
}

void Console::processBulk(std::ostream &os, bulk_t &bulk)
{
    os << "bulk: ";
    while (!bulk.empty()) {
        processOne(os, bulk);
    }
    os << std::endl;
}

LogFile::LogFile() : Logger()
{
    m_threads.push_back(std::thread(&LogFile::worker, this));
    m_threads.push_back(std::thread(&LogFile::worker, this));
}

void LogFile::worker()
{
    while (!m_stopped) {

        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this]() { return m_stopped || !m_bulks.empty(); });

        if (!m_bulks.empty()) {

            auto &bulk = m_bulks.front();

            if (!m_logFile.is_open()) {
                openNewLogFile();
            }

            processOne(m_logFile, bulk);

            if (bulk.empty()) {
                m_bulks.pop();
                m_logFile.close();
            }
        }

        if (m_stopped) {
            break;
        }
    }
}

void LogFile::openNewLogFile()
{
    static auto fileNum = 0;
    auto result = std::time(nullptr);
    std::ostringstream ossFilename;
    std::ostream &osFilename = ossFilename;
    osFilename << "bulk" << result
               << "_" << std::this_thread::get_id()
               << "_" << fileNum++
               << ".log";
    m_logFileName = ossFilename.str();
    m_logFile.open(m_logFileName);

    // write some service info to file
    if (m_logFile.is_open()) {
        m_logFile << "bulk: ";
    } else {
        throw "Error! File " + m_logFileName + "is not opened";
    }
}


