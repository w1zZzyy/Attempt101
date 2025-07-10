#pragma once

#include "boost/asio.hpp"
#include <thread>
#include <vector>

namespace match
{


namespace net = boost::asio;


class IOManager
{
public:

    static void Run() {io.run();}
    static void RunThreads(int threads) {
        workers.resize(threads);
        for(auto& w : workers) {
            w = std::thread([](){io.run();});
        }
    }
    static void Stop() {
        if(!io.stopped()) {io.stop();}
        for(auto& w : workers) {if(w.joinable()) w.join();}
    }


    static net::io_context io;

private:

    static std::vector<std::thread> workers;

};


}