#include "logger.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace logger;

Logger::Logger() : m_paused(true)
{
}

Console::Console(std::ostream &os) : Logger(), m_os(os)
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
    m_paused = false;
    m_cv.notify_one();
}

void Console::worker()
{
    while (true) {
        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this]() { return !m_paused; });

        // process all cmds in queue
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

        m_paused = true;
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
    m_cmds = cmds;
    m_paused = false;
    m_cv.notify_all();
}

void LogFile::worker()
{
    while (true) {

        std::unique_lock<std::mutex> lk(m_mutex);
        m_cv.wait(lk, [this]() { return !m_paused; });

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
                if (!m_cmds.empty()) {
                    m_logFile << "bulk: ";
                }
            } else {
                throw "Error! File " + m_logFileName + "is not opened";
            }
        }

        // process next cmd
        m_logFile << m_cmds.front();
        m_cmds.pop();

        // in it need, close the file
        if (!m_cmds.empty()) {
            m_logFile << ", ";
        } else {
            m_logFile.close();
            m_paused = true;
        }
    }
}


