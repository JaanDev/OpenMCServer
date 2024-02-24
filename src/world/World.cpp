#include "World.hpp"

#include <nbtpp.hpp>
#include <spdlog/spdlog.h>

namespace fs = std::filesystem;

World& World::get() {
    static auto inst = World();
    return inst;
}

World::World() {}

void World::loadWorld(std::string_view name) {
    m_worldPath = fs::canonical(fs::path(name));
    nbt::CompoundValue level;
    try {
        level = nbt::loadFromCompressedFile(m_worldPath / "level.dat");
    } catch (const std::runtime_error& e) {
        spdlog::critical("Failed to load ({})", e.what());
        exit(1);
    }

    auto data = level.getItems()["Data"];
    
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
