#pragma once
#include <sockpp/tcp_acceptor.h>
#include <mutex>

class Server {
  public:
    static Server& get();
    Server();
    void run();
  
  private:
    void acceptThread();
    void runLoop();
    void loadSpawnArea();
    void update();
  
    sockpp::tcp_acceptor m_acceptor;
    bool m_running;
};
