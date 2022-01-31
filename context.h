#pragma once

#include "handler.h"

namespace bulk {

class Context
{
public:
    Context(size_t size);
    void receive(const char *data, size_t size);
    void disconnect();

private:
    std::string m_buffer;
    HandlerPtr m_handler;

};

}

