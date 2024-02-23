#include "Client.hpp"

#include "utils.hpp"
#include "StreamReader.hpp"
#include <spdlog/spdlog.h>
using spdlog::info, spdlog::critical, spdlog::warn, spdlog::error;

Client::Client(sockpp::tcp_socket&& socket) : m_socket(std::move(socket)), m_isConnected(true) {}

void Client::update() {
    uint8_t buf[256];
    memset(buf, 0, sizeof(buf));

    auto res = m_socket.read(buf, sizeof(buf));
    if (res.is_error() && res.error() != std::errc::resource_unavailable_try_again) {
        m_isConnected = false;
        return;
    }

    processPackets(buf, res.value());

    const auto& bytes = m_writer.getBytes();
    auto bytesLen = bytes.size();
    if (bytesLen > 0) {
        info("Writing back {} bytes", bytesLen);
        m_socket.write(bytes.data(), bytesLen);
        m_writer.clear();
    }
}

inline StreamWriter& Client::getWriter() {
    return m_writer;
}

void Client::sendPacket(std::vector<uint8_t> bytes) {
    m_writer.writeRaw(bytes);
}

void Client::sendPacket(PacketID id, std::vector<uint8_t> bytes) {
    m_writer.write((uint8_t)id);
    m_writer.writeRaw(bytes);
}

void Client::sendPacket(std::initializer_list<uint8_t> bytes) {
    m_writer.writeRaw(bytes);
}

void Client::sendPacket(PacketID id, std::initializer_list<uint8_t> bytes) {
    m_writer.write((uint8_t)id);
    m_writer.writeRaw(bytes);
}

void Client::sendPacketID() {
    m_writer.write(m_currentPacketID);
}

void Client::processPackets(uint8_t* buf, size_t len) {
    auto reader = StreamReader(buf, len);

    while (reader.len() > 0) {
        auto packetID = reader.read<PacketID>();
        m_currentPacketID = packetID;
        info("Processing packet {}", (int)packetID);

        switch (packetID) {
        case PacketID::KeepAlive: {
            //
        } break;
        case PacketID::Login: {
            auto protocol = reader.read<int>();
            auto s1 = reader.readStr();
            auto s2 = reader.readStr();
            if (protocol != 2) {
                warn("Protocol of the client {} (user {}) doesnt match with the server protocol ({} != 2)", m_socket.address().to_string(), s1, protocol);
                m_isConnected = false;
                break;
            }
            info("Client {} logged in as {}", m_socket.address().to_string(), s1);
            // sendPacket(PacketID::Login, {})

            sendPacketID();
            m_writer.write(0);
            m_writer.writeStr("");
            m_writer.writeStr("");

            m_writer.write(PacketID::SpawnPosition);
            m_writer.write(0);
            m_writer.write(0);
            m_writer.write(0);
        } break;
        case PacketID::Handshake: {
            auto userName = reader.readStr();
            info("Connecting user {}...", userName);
            sendPacketID();
            m_writer.writeStr("-");
        } break;
        case PacketID::Chat: {
            reader.readStr();
        } break;
        case PacketID::UpdateTime: {
            reader.read<long long>();
        } break;
        case PacketID::PlayerInventory: {
            reader.read<int>();
            auto x = reader.read<short>();
            for (auto i = 0; i < x; i++) {
                auto x1 = reader.read<short>();
                if (x1 >= 0) {
                    reader.read<char>();
                    reader.read<short>();
                }
            }
        } break;
        case PacketID::SpawnPosition: {
            reader.read<int>();
            reader.read<int>();
            reader.read<int>();
        } break;
        case PacketID::Flying: {
            reader.read<char>();
        } break;
        case PacketID::PlayerPosition: {
            reader.read<double>();
            reader.read<double>();
            reader.read<double>();
            reader.read<double>();
            reader.read<char>();
        } break;
        case PacketID::PlayerLook: {
            reader.read<float>();
            reader.read<float>();
            reader.read<char>();
        } break;
        case PacketID::PlayerLookMove: {
            reader.read<double>();
            reader.read<double>();
            reader.read<double>();
            reader.read<double>();
            reader.read<float>();
            reader.read<float>();
            reader.read<char>();
        } break;
        case PacketID::BlockDig: {
            reader.read<char>();
            reader.read<int>();
            reader.read<char>();
            reader.read<int>();
            reader.read<char>();
        } break;
        case PacketID::Place: {
            reader.read<short>();
            reader.read<int>();
            reader.read<char>();
            reader.read<int>();
            reader.read<char>();
        } break;
        case PacketID::BlockItemSwitch: {
            reader.read<int>();
            reader.read<short>();
        } break;
        case PacketID::AddToInventory: {
            reader.read<short>();
            reader.read<char>();
            reader.read<short>();
        } break;
        case PacketID::ArmAnimation: {
            reader.read<int>();
            reader.read<char>();
        } break;
        case PacketID::NamedEntitySpawn: {
            reader.read<int>();
            reader.readStr();
            reader.read<int>();
            reader.read<int>();
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
            reader.read<short>();
        } break;
        case PacketID::PickupSpawn: {
            reader.read<int>();
            reader.read<short>();
            reader.read<char>();
            reader.read<int>();
            reader.read<int>();
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
            reader.read<char>();
        } break;
        case PacketID::Collect: {
            reader.read<int>();
            reader.read<int>();
        } break;
        case PacketID::VehicleSpawn: {
            reader.read<int>();
            reader.read<char>();
            reader.read<int>();
            reader.read<int>();
            reader.read<int>();
        } break;
        case PacketID::MobSpawn: {
            reader.read<int>();
            reader.read<char>();
            reader.read<int>();
            reader.read<int>();
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
        } break;
        case PacketID::DestroyEntity: {
            reader.read<int>();
        } break;
        case PacketID::Entity: {
            reader.read<int>();
        } break;
        case PacketID::RelEntityMove: {
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
            reader.read<char>();
        } break;
        case PacketID::EntityLook: {
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
        } break;
        case PacketID::RelEntityMoveLook: {
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
            reader.read<char>();
            reader.read<char>();
            reader.read<char>();
        } break;
        case PacketID::EntityTeleport: {
            reader.read<int>();
            reader.read<int>();
            reader.read<int>();
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
        } break;
        case PacketID::PreChunk: {
            reader.read<int>();
            reader.read<int>();
            reader.read<char>();
        } break;
        case PacketID::MapChunk: {
            reader.read<int>();
            reader.read<short>();
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
            reader.read<char>();
            auto len = reader.read<int>();
            reader.skip(len); // gzip data
        } break;
        case PacketID::MultiBlockChange: {
            reader.read<int>();
            reader.read<int>();
            auto x = reader.read<short>();
            reader.skip(x * 2); // shorts
            reader.skip(x); // char[]
            reader.skip(x); // char[]
        } break;
        case PacketID::BlockChange: {
            reader.read<int>();
            reader.read<char>();
            reader.read<int>();
            reader.read<char>();
            reader.read<char>();
        } break;
        case PacketID::ComplexEntity: {
            reader.read<int>();
            reader.read<short>();
            reader.read<int>();
            auto len = reader.read<short>();
            reader.skip(len); // char[] nbt
        } break;
        case PacketID::KickDisconnect: {
            reader.readStr();
        } break;
        default: {
            error("Unhandled packet {}", (int)packetID);
        } break;
        }
    }
}
