#include <cassert>
#include "async.h"
#include "handler.h"

int main(int argc, char*argv[])
{
    assert(argc == 2);
    int bulkSize = std::atoi(argv[1]);
    assert(bulkSize > 0);

    bulk::Handler handler(bulkSize);

    bulk::Cmd cmd;
    while (std::getline(std::cin, cmd)) {
        handler.reveiveCmd(cmd);
    }
    handler.receiveCmdEof();

    // wait for threads start
    std::this_thread::sleep_for(std::chrono::milliseconds(10));

//    std::size_t bulk = 5;

//    auto h = async::connect(bulk);
//    auto h2 = async::connect(bulk);

//    async::receive(h, "1", 1);
////    async::receive(h2, "1\n", 2);
//    async::receive(h, "\n2\n3\n4\n5\n6\n{\na\n", 15);
//    async::receive(h, "b\nc\nd\n}\n89\n", 11);

//    async::disconnect(h);
//    async::disconnect(h2);

    return 0;
}
