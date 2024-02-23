#include "World.hpp"

World& World::get() {
    static auto inst = World();
    return inst;
}

World::World() {}

void World::loadWorld(std::string_view name) {
    
}

Chunk& World::getChunk(const ChunkPos& pos) {
    
}
