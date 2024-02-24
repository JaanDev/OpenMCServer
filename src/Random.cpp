#include "Random.hpp"

#include <chrono>
#include <math.h>
#include <iostream>

// https://developer.classpath.org/doc/java/lang/StrictMath-source.html

double strictMathLog(double x) {
    if (x == 0)
        return -INFINITY;
    if (x < 0)
        return INFINITY;
    if (!(x < INFINITY))
        return x;

    constexpr double TWO_54 = 0x40000000000000L;
    constexpr double SQRT_2 = 1.4142135623730951;
    constexpr double TWO_20 = 0x100000;
    constexpr double LN2_H = 0.6931471803691238;
    constexpr double LN2_L = 1.9082149292705877e-10;

    constexpr double LG1 = 0.6666666666666735, // Long bits 0x3fe5555555555593L.
        LG2 = 0.3999999999940942,              // Long bits 0x3fd999999997fa04L.
        LG3 = 0.2857142874366239,              // Long bits 0x3fd2492494229359L.
        LG4 = 0.22222198432149784,             // Long bits 0x3fcc71c51d8e78afL.
        LG5 = 0.1818357216161805,              // Long bits 0x3fc7466496cb03deL.
        LG6 = 0.15313837699209373,             // Long bits 0x3fc39a09d078c69fL.
        LG7 = 0.14798198605116586;             // Long bits 0x3fc2f112df3e5244L.

    // Normalize x.
    long long bits = *(long long*)&x;
    int exp = (int)(bits >> 52);
    if (exp == 0) // Subnormal x.
    {
        x *= TWO_54;
        bits = *(long long*)&x;
        exp = (int)(bits >> 52) - 54;
    }
    exp -= 1023; // Unbias exponent.
    bits = (bits & 0x000fffffffffffffL) | 0x3ff0000000000000L;
    x = *(double*)&bits;
    if (x >= SQRT_2) {
        x *= 0.5;
        exp++;
    }
    x--;
    if (abs(x) < 1 / TWO_20) {
        if (x == 0)
            return exp * LN2_H + exp * LN2_L;
        double r = x * x * (0.5 - 1 / 3.0 * x);
        if (exp == 0)
            return x - r;
        return exp * LN2_H - ((r - exp * LN2_L) - x);
    }
    double s = x / (2 + x);
    double z = s * s;
    double w = z * z;
    double t1 = w * (LG2 + w * (LG4 + w * LG6));
    double t2 = z * (LG1 + w * (LG3 + w * (LG5 + w * LG7)));
    double r = t2 + t1;
    if (bits >= 0x3ff6174a00000000L && bits < 0x3ff6b85200000000L) {
        double h = 0.5 * x * x; // Need more accuracy for x near sqrt(2).
        if (exp == 0)
            return x - (h - s * (h + r));
        return exp * LN2_H - ((h - (s * (h + r) + exp * LN2_L)) - x);
    }
    if (exp == 0)
        return x - s * (x - r);
    return exp * LN2_H - ((s * (x - r) - exp * LN2_L) - x);
}

double strictMathSqrt(double x) {
    if (x < 0)
        return NAN;
    if (x == 0 || !(x < INFINITY))
        return x;

    constexpr double TWO_54 = 0x40000000000000L;

    // Normalize x.
    long long bits = *(long long*)&x;
    int exp = (int)(bits >> 52);
    if (exp == 0) // Subnormal x.
    {
        x *= TWO_54;
        bits = *(long long*)&x;
        exp = (int)(bits >> 52) - 54;
    }
    exp -= 1023; // Unbias exponent.
    bits = (bits & 0x000fffffffffffffL) | 0x0010000000000000L;
    if ((exp & 1) == 1) // Odd exp, double x to make it even.
        bits <<= 1;
    exp >>= 1;

    // Generate sqrt(x) bit by bit.
    bits <<= 1;
    long long q = 0;
    long long s = 0;
    long long r = 0x0020000000000000L; // Move r right to left.
    while (r != 0) {
        long long t = s + r;
        if (t <= bits) {
            s = t + r;
            bits -= t;
            q += r;
        }
        bits <<= 1;
        r >>= 1;
    }

    // Use floating add to round correctly.
    if (bits != 0)
        q += q & 1;
    long long val_ = (q >> 1) + ((exp + 1022L) << 52);
    return *(double*)&val_;
}

Random::Random()
    : Random(seedUniquifier() ^
             std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now().time_since_epoch())
                 .count()) {}

Random::Random(long long seed) : m_haveNextNextGaussian(false), m_seed(initialScramble(seed)), m_nextNextGaussian(0.0) {}

void Random::setSeed(long long seed) {
    std::unique_lock<std::mutex> lock(m_mutex);
    m_seed = initialScramble(seed);
    m_haveNextNextGaussian = false;
}

void Random::nextBytes(std::span<uint8_t> bytes) {
    int i = 0;
    int len = bytes.size();

    while (i < len) {
        int rnd = nextInt();

        for (int n = std::min(len - i, 4); n-- > 0; rnd >>= 8) {
            bytes[i++] = (uint8_t)rnd;
        }
    }
}

int Random::nextInt() {
    return next(32);
}

int Random::nextInt(int bound) {
    if (bound < 0) {
        throw std::runtime_error("bound must be positive");
    } else {
        int r = next(31);
        int m = bound - 1;
        if ((bound & m) == 0) {
            r = (int)((long long)bound * (long long)r >> 31);
        } else {
            for (int u = r; u - (r = u % bound) + m < 0; u = next(31)) {
            }
        }

        return r;
    }
}

bool Random::nextBoolean() {
    return next(1) != 0;
}

float Random::nextFloat() {
    return (float)next(24) * 5.9604645E-8F;
}

double Random::nextDouble() {
    return (double)(((long long)next(26) << 27) + (long long)next(27)) * 1.1102230246251565E-16;
}

double Random::nextGaussian() {
    std::unique_lock<std::mutex> lock(m_mutex);
    if (m_haveNextNextGaussian) {
        m_haveNextNextGaussian = false;
        return m_nextNextGaussian;
    } else {
        double v1;
        double v2;
        double s;
        do {
            do {
                v1 = 2.0 * nextDouble() - 1.0;
                v2 = 2.0 * nextDouble() - 1.0;
                s = v1 * v1 + v2 * v2;
            } while (s >= 1.0);
        } while (s == 0.0);

        double multiplier = strictMathSqrt(-2.0 * strictMathLog(s) / s);
        m_nextNextGaussian = v2 * multiplier;
        m_haveNextNextGaussian = true;
        return v1 * multiplier;
    }
}

long long Random::seedUniquifier() {
    auto next = g_seedUniquifier.load() * 1181783497276652981L;
    g_seedUniquifier = next;

    return next;
}

long long Random::initialScramble(long long seed) {
    return (seed ^ 25214903917L) & 281474976710655L;
}

long long Random::nextLong() {
    return ((long long)next(32) << 32) + (long long)next(32);
}

int Random::next(int bits) {
    auto nextseed = m_seed.load() * 25214903917L + 11L & 281474976710655L;
    m_seed = nextseed;

    return (int)((unsigned long long)nextseed >> (48 - bits));
}