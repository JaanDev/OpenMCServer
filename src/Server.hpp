#pragma once
#include <sockpp/tcp_acceptor.h>
#include <mutex>
#include "Client.hpp"

class Server {
  public:
    static Server& get();
    Server();
  
  private:
    void acceptThread();
    // void processPackets(sockpp::tcp_socket& socket, uint8_t* data, size_t len);
    void runLoop();
    void update();
  
    sockpp::tcp_acceptor m_acceptor;
    std::vector<Client> m_clients;
    std::mutex m_clientsMutex;
    bool m_running;
};
