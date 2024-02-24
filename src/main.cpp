#include "Server.hpp"

#include <spdlog/spdlog.h>
#include "Random.hpp"
#include "main.h"

int main() {
    // spdlog::set_pattern("[%H:%M:%S] [%L] %v");
    // Server::get().run();

    Random random = Random(123456);

    for (auto i = 0; i < 500; i++) {
        auto num = random.nextBoolean();
        std::cout << "B " << std::boolalpha << num << std::endl;
    }

    std::cout << "----" << std::endl;

    for (auto i = 0; i < 50; i++) {
        auto bytes = new uint8_t[32];
        random.nextBytes({bytes, 32});
        std::cout << "BY ";
        for (auto j = 0; j < 32; j++) {
            std::cout << std::format("{:02X} ", bytes[j]);
        }
        std::cout << std::endl;
        delete[] bytes;
    }

    std::cout << "----" << std::endl;

    for (auto i = 0; i < 500; i++) {
        auto num = random.nextDouble();
        std::cout << std::format("D {}", num) << std::endl;
        // std::cout << "D " << num << std::endl;
    }

    std::cout << "----" << std::endl;

    for (auto i = 0; i < 500; i++) {
        auto num = random.nextFloat();
        std::cout << std::format("F {}", num) << std::endl;
        // std::cout << "F " << num << std::endl;
    }

    std::cout << "----" << std::endl;

    for (auto i = 0; i < 500; i++) {
        auto num = random.nextGaussian();
        std::cout << std::format("GA {}", num) << std::endl;
        // std::cout << "GA " << num << std::endl;
    }

    std::cout << "----" << std::endl;

    for (auto i = 0; i < 500; i++) {
        auto num = random.nextInt();
        std::cout << "I " << num << std::endl;
    }

    std::cout << "----" << std::endl;

    for (auto i = 0; i < 500; i++) {
        auto num = random.nextInt(11223344);
        std::cout << "I2 " << num << std::endl;
    }

    std::cout << "----" << std::endl;

    for (auto i = 0; i < 500; i++) {
        auto num = random.nextLong();
        std::cout << "L " << num << std::endl;
    }

    std::cout << "----" << std::endl;

    return 0;
}