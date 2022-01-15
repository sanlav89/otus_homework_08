#include "logger.h"
#include <iostream>
#include <ctime>
#include <sstream>

using namespace logger;

Console::Console(std::ostream &os) : m_os(os)
{
}

void Console::open()
{
    // nothing to do
}

void Console::close()
{
    // nothing to do
}

void Console::write(const std::string &log)
{
    m_os << log;
}

void LogFile::open()
{
    auto result = std::time(nullptr);
    std::ostringstream ossFilename;
    std::ostream &osFilename = ossFilename;
    osFilename << "bulk" << result << ".log";
    m_logFileName = ossFilename.str();
}

void LogFile::close()
{
    if (m_logFile.is_open()) {
        m_logFile.close();
    }
}

void LogFile::write(const std::string &log)
{
    m_logFile.open(m_logFileName);
    if (m_logFile.is_open()) {
        m_logFile << log;
    } else {
        throw "Error! File " + m_logFileName + "is not opened";
    }
}
