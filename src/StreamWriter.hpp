#pragma once
#include <cstdint>
#include <vector>
#include <string>

class StreamWriter {
  public:
    template <typename T>
    void write(T val) {
        auto len = sizeof(T);
        auto start = (uint8_t*)&val;
        auto end = start + len;
        if (len > 1) {
            std::reverse(start, end);
        }
        m_bytes.insert(m_bytes.end(), start, end);
    }

    void writeRaw(std::vector<uint8_t> bytes);
    void writeRaw(std::initializer_list<uint8_t> bytes);
    void writeRaw(uint8_t* bytes, size_t len);
    void writeStr(const std::string& str);

    inline const std::vector<uint8_t>& getBytes() { return m_bytes; }
    inline void clear() { m_bytes.clear(); }

  private:
    std::vector<uint8_t> m_bytes;
};