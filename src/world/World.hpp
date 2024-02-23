#pragma once
#include "utils.hpp"
#include "Chunk.hpp"
#include <unordered_map>
#include <string_view>

class World {
  public:
    static World& get();
    World();
    void loadWorld(std::string_view name);

    // if the chunk doesnt exist, generates it
    Chunk& getChunk(const ChunkPos& pos);

  private:
    std::unordered_map<ChunkPos, Chunk> m_chunks;
};