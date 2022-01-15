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

    return 0;
}
