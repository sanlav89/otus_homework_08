#include "async.h"
#include "handler.h"

namespace async {

logger::LogPtr m_console = nullptr;
logger::LogPtr m_logfile = nullptr;

handle_t connect(std::size_t bulk)
{
    if (m_console == nullptr) {
        m_console = logger::LogPtr{new logger::Console(std::cout)};
    }
    if (m_logfile == nullptr) {
        m_logfile = logger::LogPtr{new logger::LogFile};
    }

    bulk::Handler *context{new bulk::Handler(bulk)};
    context->registerLogger(m_console);
    context->registerLogger(m_logfile);

    return context;
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    bulk::Handler *context = static_cast<bulk::Handler *>(handle);
    context->receive(data, size);
}

void disconnect(handle_t handle)
{
    bulk::Handler *context = static_cast<bulk::Handler *>(handle);
    context->receiveEof();
    delete context;
}

}
