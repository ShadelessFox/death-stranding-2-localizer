#pragma once

#include "Decima/Core/RTTIObject.h"
#include "Decima/Core/GGUUID.h"

class RTTIRefObject : public RTTIObject {
public:
    virtual void RTTIRefObject_Unk01() = 0;

    virtual void RTTIRefObject_Unk02() = 0;

public:
    uint32_t RefCount;
    uint32_t mUnk0C;
    GGUUID ObjectUUID;
};

assert_size(RTTIRefObject, 32);
assert_offset(RTTIRefObject, RefCount, 8);
assert_offset(RTTIRefObject, ObjectUUID, 16);