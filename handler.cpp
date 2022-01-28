#include "handler.h"

#include <sstream>

using namespace bulk;

Handler::Handler(const size_t &bulkSize, std::istream &is)
    : m_is(is)
    , m_bulkSize(bulkSize)
    , m_state(StateBasePtr{new StateEmpty(this)})
{
}

void Handler::registerLogger(logger::LogPtr logger)
{
    m_loggers.emplace_back(std::move(logger));
}

void Handler::start()
{
    std::string cmd;
    while (std::getline(m_is, cmd)) {
        if (isOpenedBracket(cmd)) {
            m_state->cmdOpenedBracket();
        } else if (isClosedBracket(cmd)) {
            m_state->cmdClosedBracket();
        } else {
            m_state->cmdOther(cmd);
        }
    }
    m_state->cmdEof();
}

void Handler::setState(StateBasePtr state)
{
    m_state = std::move(state);
}

size_t Handler::bulkSize() const
{
    return m_bulkSize;
}

size_t Handler::cmdsSize() const
{
    return m_cmds.size();
}

size_t Handler::bracketsSize() const
{
    return m_brackets.size();
}

void Handler::pushOpenedBracket()
{
    m_brackets.push('{');
}

void Handler::popOpenedBracket()
{
    m_brackets.pop();
}

void Handler::pushCmd(const Cmd &cmd)
{
    m_cmds.push(cmd);
}

void Handler::processBulk()
{
    for (const auto &logger : m_loggers) {
        logger->process(m_cmds);
    }
    while (!m_cmds.empty()) {
        m_cmds.pop();
    }
}

bool Handler::isOpenedBracket(const Cmd &cmd)
{
    return isAnyBracket(cmd, '{');
}

bool Handler::isClosedBracket(const Cmd &cmd)
{
    return isAnyBracket(cmd, '}');
}

bool Handler::isAnyBracket(const Cmd &cmd, Bracket anyBracket)
{
    if (cmd.size() == 1) {
        return cmd.at(0) == anyBracket;
    } else {
        return false;
    }
}
