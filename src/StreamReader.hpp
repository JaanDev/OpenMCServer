#pragma once
#include <cstdint>
#include <cstddef>

#include <spdlog/spdlog.h>

class StreamReader {
  public:
    StreamReader(uint8_t* data, size_t len);
    template <typename T>
    StreamReader& operator>>(T& other) {
        auto size = sizeof(other);
        if (m_len < size) {
            spdlog::error("Failed to read {} bytes from the buffer (only {} left)!", size, m_len);
            return *this;
        }

        other = *((T*)m_data);
        if (size > 1)
            std::reverse((uint8_t*)&other, (uint8_t*)&other + size);
        m_len -= size;
        m_data += size;

        return *this;
    }

    template <typename T>
    T read() {
        T val;
        *this >> val;
        return val;
    }

    inline size_t len() { return m_len; }
    void read(uint8_t* buf, size_t len);
    void skip(size_t len);

    std::string readStr();

  private:
    uint8_t* m_data;
    size_t m_len;
};