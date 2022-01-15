#include "logger.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace logger;

Console::Console(std::ostream &os) : m_os(os)
{
}

void Console::pushCmd(const bulk::Cmd &cmd)
{
    m_cmds.push(cmd);
}

void Console::process()
{
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

void LogFile::pushCmd(const bulk::Cmd &cmd)
{
    m_cmds.push(cmd);
}

void LogFile::process()
{
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
