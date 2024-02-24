#pragma once
#include <cstdint>
#include <span>
#include <atomic>
#include <mutex>

class Random {
  public:
    Random();
    Random(long long seed);
    void setSeed(long long seed);
    void nextBytes(std::span<uint8_t> bytes);
    int nextInt();
    int nextInt(int bound);
    long long nextLong();
    bool nextBoolean();
    float nextFloat();
    double nextDouble();
    double nextGaussian();

  private:
    static long long seedUniquifier();
    static long long initialScramble(long long seed);
    int next(int bits);

    std::atomic_llong m_seed;
    double m_nextNextGaussian;
    bool m_haveNextNextGaussian;
    std::mutex m_mutex;

    inline static std::atomic_llong g_seedUniquifier = 8682522807148012L;
};

// std::atomic_llong Random::g_seedUniquifier = 8682522807148012L;