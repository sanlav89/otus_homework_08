#include "logger.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace logger;

Console::Console(std::ostream &os)
    : m_os(os)
    , m_stopped(true)
{
    m_thread = std::thread(&Console::worker, this);
}

Console::~Console()
{
    m_thread.join();
}

void Console::process(const std::queue<bulk::Cmd> &cmds)
{
    std::lock_guard<std::mutex> lk(m_mutex);
    m_cmds = cmds;
    m_stopped = false;
    m_cv.notify_one();
}

void Console::worker()
{
    while (true) {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this]() { return !m_stopped; });

        if (!m_cmds.empty()) {
            m_os << "bulk: ";
            while (!m_cmds.empty()) {
                m_os << m_cmds.front();
                m_cmds.pop();
                if (!m_cmds.empty()) {
                    m_os << ", ";
                }
            }
            m_os << std::endl;
        }

        m_stopped = true;
    }
}

LogFile::LogFile()
    : m_stopped(true)
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
    m_cmds = cmds;
    m_stopped = false;
    m_cv.notify_all();
}

void LogFile::worker()
{
    while (true) {

        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this]() { return !m_stopped; });

        if (!m_logFile.is_open()) {
            auto result = std::time(nullptr);
            std::ostringstream ossFilename;
            std::ostream &osFilename = ossFilename;
            osFilename << "bulk" << result << "_" << std::this_thread::get_id() << ".log";
            m_logFileName = ossFilename.str();
            m_logFile.open(m_logFileName);

            if (m_logFile.is_open()) {

                if (!m_cmds.empty()) {
                    m_logFile << "bulk: ";
                }

            } else {
                throw "Error! File " + m_logFileName + "is not opened";
            }
        }

        m_logFile << m_cmds.front();
        m_cmds.pop();
        if (!m_cmds.empty()) {
            m_logFile << ", ";
        } else {
            m_logFile.close();
            m_stopped = true;
        }

    }
}
