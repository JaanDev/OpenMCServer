#include "StreamReader.hpp"

StreamReader::StreamReader(uint8_t* data, size_t len) : m_data(data), m_len(len) {}

void StreamReader::read(uint8_t* buf, size_t len) {
    if (m_len < len) {
        spdlog::error("Failed to read {} bytes from the buffer (only {} left)!", len, m_len);
        return;
    }

    memcpy(buf, m_data, len);
    m_len -= len;
    m_data += len;
}

void StreamReader::skip(size_t len) {
    if (len > m_len) {
        spdlog::error("Tried to skip {} bytes (only {} left)!", len, m_len);
        return;
    }

    m_len -= len;
    m_data += len;
}

std::string StreamReader::readStr() {
    auto len = read<uint16_t>();
    char bytes[len + 1];
    read((uint8_t*)bytes, len);
    return std::string(bytes, len);
}
