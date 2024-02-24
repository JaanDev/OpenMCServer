#pragma once
#include "utils.hpp"
#include "Chunk.hpp"
#include "Random.hpp"
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

    BlockID getBlock(const Vec3i& pos);

  private:
    BlockID getHighestBlockFromGround(const Vec2i& pos);
  
    std::unordered_map<ChunkPos, Chunk> m_chunks;
    std::filesystem::path m_worldPath;
    Random m_random;
    Vec3i m_spawn;
    long long m_worldSeed;
    long long m_startTime;
    long long m_worldTime;
    bool m_snowCovered;
};