#include "StreamWriter.hpp"

void StreamWriter::writeRaw(std::vector<uint8_t> bytes) {
    m_bytes.insert(m_bytes.end(), bytes.begin(), bytes.end());
}

void StreamWriter::writeRaw(std::initializer_list<uint8_t> bytes) {
    m_bytes.insert(m_bytes.end(), bytes);
}

void StreamWriter::writeRaw(uint8_t* bytes, size_t len) {
    m_bytes.insert(m_bytes.end(), bytes, bytes + len);
}

void StreamWriter::writeStr(const std::string& str) {
    auto len = static_cast<uint16_t>(str.length());
    write(len);
    writeRaw((uint8_t*)str.c_str(), len);
}
