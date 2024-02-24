#include "World.hpp"

#include <nbtpp.hpp>
#include <spdlog/spdlog.h>
#include <chrono>

namespace fs = std::filesystem;

World& World::get() {
    static auto inst = World();
    return inst;
}

World::World()
    : m_random(), m_worldSeed(0), m_spawn({0, 0, 0}), m_worldTime(0), m_snowCovered(false),
      m_startTime(std::chrono::high_resolution_clock::now().time_since_epoch().count()) {}

void World::loadWorld(std::string_view name) {
    fs::create_directory(name);
    m_worldPath = fs::canonical(name);
    std::cout << m_worldPath << std::endl;
    
    if (fs::exists(m_worldPath / "level.dat")) {
        try {
            auto level = nbt::loadFromCompressedFile(m_worldPath / "level.dat");

            auto& data = level.getItems()["Data"]->asCompound()->getItems();
            m_worldSeed = data["RandomSeed"]->asSimple()->as<long long>();
            m_spawn.x = data["SpawnX"]->asSimple()->as<int>();
            m_spawn.y = data["SpawnY"]->asSimple()->as<int>();
            m_spawn.z = data["SpawnZ"]->asSimple()->as<int>();
            m_worldTime = data["Time"]->asSimple()->as<long long>();
            m_snowCovered = data["SnowCovered"]->asSimple()->as<char>();
        } catch (const std::runtime_error& e) {
            spdlog::critical("Failed to load level.dat ({})", e.what());
            exit(1);
        }
    } else {
        m_snowCovered = m_random.nextInt(4) == 0;
    }

    if (m_worldSeed == 0) {
        m_worldSeed = Random().nextLong();

        m_spawn.x = 0;
        m_spawn.y = 64;
        m_spawn.z = 0;
        spdlog::warn("TODO SPAWN Z");
        // for (m_spawn.z = 0; getHighestBlockFromGround({m_spawn.x, m_spawn.z}) != BlockID::Sand;
        //      m_spawn.z += m_random.nextInt(64) - m_random.nextInt(64)) {
        //     m_spawn.x += m_random.nextInt(64) - m_random.nextInt(64);
        // }
    }

    spdlog::info("{} {} {} {} {} {}", m_worldSeed, m_spawn.x, m_spawn.y, m_spawn.z, m_worldTime, m_snowCovered);
}

Chunk& World::getChunk(const ChunkPos& pos) {
    if (auto search = m_chunks.find(pos); search != m_chunks.end()) {
        return search->second;
    } else {
        return generateChunk(pos);
    }
}

Chunk& World::generateChunk(const ChunkPos& pos) {
    auto chunk = Chunk();
    m_chunks[pos] = chunk;
    return m_chunks[pos];
}

BlockID World::getBlock(const Vec3i& pos) {
    if (pos.x < -32000000 || pos.z < -32000000 || pos.x >= 32000000 || pos.z >= 32000000 || pos.y < 0 || pos.y >= 128)
        return BlockID::Air;

    return getChunk({pos.x >> 4, pos.z >> 4}).getBlock({pos.x & 0xF, pos.y, pos.z & 0xF});
}

BlockID World::getHighestBlockFromGround(const Vec2i& pos) {
    int y;
    for (y = 63; getBlock({pos.x, y + 1, pos.y}) != BlockID::Air; y++) {
    }
    return getBlock({pos.x, y, pos.y});
}
