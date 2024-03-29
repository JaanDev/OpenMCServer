#include "Server.hpp"

#include "utils.hpp"
#include "StreamReader.hpp"
#include "World.hpp"

#include <thread>
#include <chrono>
#include <spdlog/spdlog.h>
using spdlog::info, spdlog::critical, spdlog::warn, spdlog::error;
using namespace std::chrono_literals;

Server& Server::get() {
    static auto inst = Server();
    return inst;
}

Server::Server() : m_running(true) {}

void Server::run() {
    sockpp::initialize();

    sockpp::error_code ec;
    m_acceptor = sockpp::tcp_acceptor(9090, 4, ec);
    if (ec) {
        critical("Failed to create a socket server. Error: {}", ec.message());
        exit(1);
    }

    info("Started on {}", m_acceptor.address().to_string());

    // auto worldName = "/mnt/C/Dev/MCRewrite/server/world";
    auto worldName = "world";
    info("Loading world \"{}\"", worldName); // forced for now
    World::get().loadWorld(worldName);
    
    // info("Loading spawn area...");
    // loadSpawnArea();
    
    // // std::thread(&Server::acceptThread, this).detach();
    // runLoop();
}

void Server::acceptThread() {
    // while (m_running) {
    //     sockpp::inet_address peer;

    //     // Accept a new client connection
    //     if (auto res = m_acceptor.accept(&peer); !res) {
    //         error("Failed to accept an incoming connection {}. Error: {}", peer.to_string(), res.error_message());
    //     } else {
    //         info("Received a new connection at {}", peer.to_string());

    //         sockpp::tcp_socket sock = res.release();
    //         sock.set_non_blocking();

    //         std::lock_guard<std::mutex> guard(m_clientsMutex);
    //         m_clients.push_back(Client(std::move(sock)));
    //     }
    // }
}

void Server::runLoop() {
    const auto updateDelay = 50ms; // 20 tps

    auto prevTime = std::chrono::high_resolution_clock::now();
    auto timeSinceLastUpdate = 0ns;
    while (m_running) {
        auto nowTime = std::chrono::high_resolution_clock::now();
        auto dt = nowTime - prevTime;

        if (dt >= 200ms) {
            warn("Can't keep up! Did the system time change, or is the server overloaded?");
            dt = 200ms;
        }

        if (dt.count() < 0) {
            warn("Time ran backwards! Did the system time change?");
            dt = 0ms;
        }

        timeSinceLastUpdate += dt;
        while (timeSinceLastUpdate >= updateDelay) {
            update();
            timeSinceLastUpdate -= updateDelay;
        }

        std::this_thread::sleep_for(1ms);

        prevTime = nowTime;
    }
}

void Server::loadSpawnArea() {
    auto distance = 5;
    auto& w = World::get();
    for (auto x = -distance; x < distance; x++) {
        for (auto z = -distance; z < distance; z++) {
            w.getChunk({x, z});
        }
    }
}

void Server::update() {
    
}
