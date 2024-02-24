#pragma once
#include "utils.hpp"
#include "Chunk.hpp"
#include <unordered_map>
#include <string_view>
#include <filesystem>

class World {
  public:
    static World& get();
    World();
    void loadWorld(std::string_view name);

    // if the chunk doesnt exist, generates it
    Chunk& getChunk(const ChunkPos& pos);
    Chunk& generateChunk(const ChunkPos& pos);

  private:
    std::unordered_map<ChunkPos, Chunk> m_chunks;
    std::filesystem::path m_worldPath;
    long long m_worldSeed;
    Vec3i m_spawn;
    long long m_worldTime;
    long long m_sizeOnDisk;
    bool m_snowCovered;
};