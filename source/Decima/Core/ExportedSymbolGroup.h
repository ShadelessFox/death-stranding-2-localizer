#pragma once

#include "Decima/Core/RTTI.h"
#include "Decima/Core/RTTIObject.h"
#include "Decima/PCore/Array.h"
#include "Decima/PCore/HashMap.h"
#include "Decima/PCore/String.h"

struct ExportedSymbol {
    enum class Kind : uint8_t {
        Atom = 0,
        Enum = 1,
        Class = 2,
        Struct = 3,
        Typedef = 4,
        Function = 5,
        Variable = 6,
        Container = 7,
        Reference = 8,
        Pointer = 9,
        Unk10 = 10
    };

    struct Signature {
        const char *mName;
        const char *mModifiers;
        RTTI *mType;
        void *mUnk18;
        uint8_t mUnk20;
    };

    struct Language {
        void *mAddress;
        const char *mName;
        void *mUnk10;
        void *mUnk18;
        Array<Signature> mSignature;
        void *mUnk30;
        void *mUnk38;
    };

    Kind mKind;
    const RTTI *mType;
    const char *mNamespace;
    const char *mName;
    void *mUnk20;
    uint8_t mUnk28;
    Language mLanguage[2];
};

assert_size(ExportedSymbol, 0xB0);
assert_size(ExportedSymbol::Kind, 0x1);
assert_size(ExportedSymbol::Signature, 0x28);
assert_size(ExportedSymbol::Language, 0x40);

class ExportedSymbolGroup : public RTTIObject {
public:
    ExportedSymbolGroup() = delete;

    virtual void RegisterSymbols() = 0;

public:
    uint32_t mExportMask;
    const char *mNamespace;
    Array<ExportedSymbol> mSymbols;
    Array<const RTTI *> mDependencies;
};

assert_size(ExportedSymbolGroup, 0x38);

struct ExportedSymbols {
    Array<ExportedSymbolGroup *> mGroups;
    Array<const RTTI *> mDependenciesUnk1;
    Array<const RTTI *> mDependenciesUnk2;
    HashMap<ExportedSymbol *, uint32_t> mAllSymbols;
    HashMap<String, ExportedSymbol *> mTypeSymbols;

    static const ExportedSymbols &Get() {
        return *Offsets::ResolveID<"RTTIFactory::sExportedSymbols", ExportedSymbols *>();
    }
};