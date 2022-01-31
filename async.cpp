#include "async.h"
#include "handler.h"
#include <sstream>

namespace async {

handle_t connect(std::size_t bulk)
{
    bulk::Handler *handler{new bulk::Handler(bulk)};
    return handler;
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    bulk::Handler *handler = static_cast<bulk::Handler *>(handle);
    handler->receive(data, size);
}

void disconnect(handle_t handle)
{
    bulk::Handler *handler = static_cast<bulk::Handler *>(handle);
    handler->stop();
    delete handler;
}

}
