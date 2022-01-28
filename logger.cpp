#include "logger.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace logger;

Logger::Logger() : m_stopped(false)
{
}

void Logger::processBulk(std::ostream &os, bulk_t &bulk)
{
    os << "bulk: ";
    while (!bulk.empty()) {
        os << bulk.front();
        bulk.pop();
        if (!bulk.empty()) {
            os << ", ";
        }
    }
    os << std::endl;
}

Console::Console(std::ostream &os) : Logger(), m_os(os)
{
    std::ios::sync_with_stdio(false);
    m_thread = std::thread(&Console::worker, this);
}

Console::~Console()
{
    stop();
}

void Console::process(const std::queue<bulk::Cmd> &cmds)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    m_bulks.push(cmds);
    m_cv.notify_all();
}

void Console::worker()
{
    while (!m_stopped) {

        std::unique_lock<std::mutex> lk(m_mutex);
        while (!m_stopped && m_bulks.empty()) {
            m_cv.wait(lk);
        }
//        m_cv.wait(lk, [this]() { return m_stopped || !m_cmds.empty(); });

        while (!m_bulks.empty()) {
            processBulk(m_os, m_bulks.front());
            m_bulks.pop();
        }

        if (m_stopped) {
            break;
        }
    }
}

void Console::stop()
{
    if (!m_stopped) {
        m_stopped = true;
        m_cv.notify_all();
        m_thread.join();
    }
}

LogFile::LogFile() : Logger()
{
    m_thread1 = std::thread(&LogFile::worker, this);
    m_thread2 = std::thread(&LogFile::worker, this);
}

LogFile::~LogFile()
{
    m_thread1.join();
    m_thread2.join();
}

void LogFile::process(const std::queue<bulk::Cmd> &cmds)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    m_stopped = false;
    m_bulks.push(cmds);
    m_cv.notify_all();
}

void LogFile::worker()
{
    while (true) {

        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this]() { return !m_stopped; });

        if (!m_logFile.is_open()) {

            // Create & open a new file
            auto result = std::time(nullptr);
            std::ostringstream ossFilename;
            std::ostream &osFilename = ossFilename;
            osFilename << "bulk" << result << "_" << std::this_thread::get_id() << ".log";
            m_logFileName = ossFilename.str();
            m_logFile.open(m_logFileName);

            // write some service info to file
            if (m_logFile.is_open()) {
                if (!m_bulks.empty()) {
                    m_logFile << "bulk: ";
                }
            } else {
                throw "Error! File " + m_logFileName + "is not opened";
            }
        }

        // process next cmd
//        m_logFile << m_cmds.front();
        m_bulks.pop();

        // in it need, close the file
        if (!m_bulks.empty()) {
            m_logFile << ", ";
        } else {
            m_logFile.close();
            m_stopped = true;
        }
    }
}


