#include <cassert>
#include "handler.h"

int main(int argc, char *argv[])
{
    assert(argc == 2);
    int bulkSize = std::atoi(argv[1]);
    assert(bulkSize > 0);

    bulk::Handler handler(bulkSize);
    handler.registerLogger(logger::LogPtr{new logger::Console()});
    handler.registerLogger(logger::LogPtr{new logger::LogFile()});
    handler.start();

    // wait for threads start
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

    return 0;
}
