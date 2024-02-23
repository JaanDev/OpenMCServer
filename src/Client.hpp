#pragma once
#include <sockpp/tcp_acceptor.h>
#include "utils.hpp"
#include "StreamWriter.hpp"

class Client {
  public:
    Client(sockpp::tcp_socket&& socket);

    void update();
    
    inline sockpp::tcp_socket& getSocket() { return m_socket; }
    inline bool isConnected() { return m_isConnected; }
    inline StreamWriter& getWriter();

    void sendPacket(std::vector<uint8_t> bytes);
    void sendPacket(PacketID id, std::vector<uint8_t> bytes);
    void sendPacket(std::initializer_list<uint8_t> bytes);
    void sendPacket(PacketID id, std::initializer_list<uint8_t> bytes);

    void sendPacketID();

  private:
    void processPackets(uint8_t* buf, size_t len);
  
    sockpp::tcp_socket m_socket;
    // std::vector<uint8_t> m_bytesToWrite;
    StreamWriter m_writer;
    bool m_isConnected;
    PacketID m_currentPacketID;
};