#include "Server.hpp"

#include <spdlog/spdlog.h>

int main() {
    spdlog::set_pattern("[%H:%M:%S] [%L] %v");
    Server::get().run();
    
    return 0;
}