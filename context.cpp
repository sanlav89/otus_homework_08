#include "context.h"

using namespace bulk;

Context::Context(size_t size) : m_handler{new Handler(size)}
{
}

void Context::receive(const char *data, size_t size)
{
    for (auto i = 0u; i < size; i++) {
        if (data[i] == '\n') {
            m_handler->reveiveCmd(m_buffer);
            m_buffer.clear();
        } else {
            m_buffer.append({data[i]});
        }
    }
}

void Context::disconnect()
{
    m_handler->receiveCmdEof();
    m_buffer.clear();
}
