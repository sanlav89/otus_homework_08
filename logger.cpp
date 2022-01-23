#include "logger.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace logger;

Console::Console(std::ostream &os) : m_os(os)
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
    m_cv.notify_one();
}

void Console::worker()
{
    while (true) {
        std::unique_lock<std::mutex> lk(m_mutex);

        while (m_cmds.empty()) {
            m_cv.wait(lk);
        }

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
    }
}

void LogFile::process(const std::queue<bulk::Cmd> &cmds)
{
    m_cmds = cmds;
    auto result = std::time(nullptr);
    std::ostringstream ossFilename;
    std::ostream &osFilename = ossFilename;
    osFilename << "bulk" << result << ".log";
    m_logFileName = ossFilename.str();

    m_logFile.open(m_logFileName);
    if (m_logFile.is_open()) {

        if (!m_cmds.empty()) {
            m_logFile << "bulk: ";
            while (!m_cmds.empty()) {
                m_logFile << m_cmds.front();
                m_cmds.pop();
                if (!m_cmds.empty()) {
                    m_logFile << ", ";
                }
            }
            m_logFile << std::endl;
        }

    } else {
        throw "Error! File " + m_logFileName + "is not opened";
    }

    m_logFile.close();
}
