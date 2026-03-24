#pragma once

#include "Decima/Core/RTTIRefObject.h"
#include "Decima/PCore/Array.h"
#include "Decima/PCore/Ref.h"

class IStreamingSystem {
public:
    class Events {
    protected:
        ~Events() = default;

    public:
        virtual void OnFinishLoadGroup(const Array<Ref<RTTIRefObject>>& inObjects) {};

        virtual void OnBeforeUnloadGroup(const Array<Ref<RTTIRefObject>>& inObjects) {};

        virtual void OnLoadAssetGroup(const Array<Ref<RTTIRefObject>> &inObjects) {};
    };

    virtual ~IStreamingSystem() = 0;

    virtual void IStreamingSystem_1() = 0;

    virtual void IStreamingSystem_2() = 0;

    virtual void AddEventListener(Events *) = 0;

    virtual void RemoveEventListener(Events *) = 0;

    virtual void IStreamingSystem_5() = 0;

    virtual void IStreamingSystem_6() = 0;

    virtual void IStreamingSystem_7() = 0;

    virtual void IStreamingSystem_8() = 0;

    virtual void IStreamingSystem_9() = 0;

    virtual void IStreamingSystem_10() = 0;

    virtual void IStreamingSystem_11() = 0;

    virtual void IStreamingSystem_12() = 0;

    virtual void IStreamingSystem_13() = 0;

    virtual void IStreamingSystem_14() = 0;

    virtual void IStreamingSystem_15() = 0;

    virtual void IStreamingSystem_16() = 0;
};