#include "async.h"
#include "context.h"
#include <sstream>

namespace async {

handle_t connect(std::size_t bulk)
{
    bulk::Context *context{new bulk::Context(bulk)};
    return context;
}

void receive(handle_t handle, const char *data, std::size_t size)
{
    bulk::Context *context = static_cast<bulk::Context *>(handle);
    context->receive(data, size);
}

void disconnect(handle_t handle)
{
    bulk::Context *context = static_cast<bulk::Context *>(handle);
    context->disconnect();
    delete context;
}

}
