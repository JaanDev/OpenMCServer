#pragma once
#include <cstdint>
#include <string>
#include <vector>

enum class PacketID : uint8_t {
    KeepAlive = 0,
    Login = 1,
    Handshake = 2,
    Chat = 3,
    UpdateTime = 4,
    PlayerInventory = 5,
    SpawnPosition = 6,
    Flying = 10,
    PlayerPosition = 11,
    PlayerLook = 12,
    PlayerLookMove = 13,
    BlockDig = 14,
    Place = 15,
    BlockItemSwitch = 16,
    AddToInventory = 17,
    ArmAnimation = 18,
    NamedEntitySpawn = 20,
    PickupSpawn = 21,
    Collect = 22,
    VehicleSpawn = 23,
    MobSpawn = 24,
    DestroyEntity = 29,
    Entity = 30,
    RelEntityMove = 31,
    EntityLook = 32,
    RelEntityMoveLook = 33,
    EntityTeleport = 34,
    PreChunk = 50,
    MapChunk = 51,
    MultiBlockChange = 52,
    BlockChange = 53,
    ComplexEntity = 59,
    KickDisconnect = 255
};

enum class BlockID : uint8_t {
    Air = 0,
    Stone,
    Grass,
    Dirt,
    Cobblestone,
    Planks,
    Sapling,
    Bedrock,
    WaterStill,
    WaterMoving,
    LavaStill,
    LavaMoving,
    Sand,
    Gravel,
    OreGold,
    OreIron,
    OreCoal,
    Wood,
    Leaves,
    Sponge,
    Glass,
    Cloth,
    PlantYellow,
    PlantRed,
    MushroomBrown,
    MushroomRed,
    BlockGold,
    BlockSteel,
    StairDouble,
    StairSingle,
    Brick,
    Tnt,
    Bookshelf,
    CobblestoneMossy,
    Obsidian,
    TorchWood,
    Fire,
    MobSpawner,
    StairCompact_wood,
    Crate,
    RedstoneWire,
    OreDiamond,
    BlockDiamond,
    Workbench,
    Crops,
    TilledField,
    StoneOvenIdle,
    StoneOvenActive,
    PressurePlateWoodActive,
    DoorWood,
    Ladder,
    MinecartTrack,
    StairCompactStone,
    PressurePlateWoodIdle,
    Lever,
    PressurePlateStone,
    DoorSteel,
    PressurePlateWood,
    OreRedstone,
    OreRedstoneGlowing,
    TorchRedstoneIdle,
    TorchRedstoneActive,
    Button,
    Snow,
    Ice,
    BlockSnow,
    Cactus,
    BlockClay,
    Reed,
    Jukebox,
    Fence
};

struct ChunkPos {
    int x;
    int z;

    bool operator==(const ChunkPos& other) const = default;
};

template <>
struct std::hash<ChunkPos> {
    size_t operator()(const ChunkPos& val) const noexcept {
        using std::hash;

        return hash<int>()(val.x) ^ (hash<int>()(val.z) << 1);
    }
};

template <typename T>
struct Vec3 {
    T x;
    T y;
    T z;
};

template <typename T>
struct Vec2 {
    T x;
    T y;
};

using Vec3f = Vec3<float>;
using Vec3i = Vec3<int>;
using Vec2f = Vec2<float>;
using Vec2i = Vec2<int>;