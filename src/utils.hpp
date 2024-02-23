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
