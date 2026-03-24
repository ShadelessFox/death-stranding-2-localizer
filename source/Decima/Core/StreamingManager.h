#pragma once

#include "Decima/Core/IStreamingManager.h"
#include "Decima/Core/IStreamingSystem.h"
#include "Util/Assert.h"

#include <cstdint>

class StreamingManager : public IStreamingManager, IStreamingSystem::Events {
public:
    static StreamingManager &Get() {
        return **Offsets::ResolveID<"StreamingManager::Instance", StreamingManager **>();
    }

    uint8_t mUnk10[0x568];
    IStreamingSystem *mStreamingSystem;
    uint8_t mUnkAC0[0xA0060];
};

assert_size(StreamingManager, 0xA05E0);
assert_offset(StreamingManager, mStreamingSystem, 0x578);
