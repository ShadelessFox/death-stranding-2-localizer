#include "Injector.h"

#include "Decima/Core/ExportedSymbolGroup.h"
#include "Decima/Core/StreamingManager.h"
#include "Decima/Core/LocalizedTextResource.h"
#include "Decima/PCore/Ref.h"
#include "Util/Offsets.h"

#include <Windows.h>
#include <detours.h>
#include <nlohmann/json.hpp>

#include <fstream>
#include <unordered_map>
#include <string_view>

using namespace std::string_view_literals;

static void *gMemAlloc(size_t inSize) {
    using MallocFn = void *(*)(size_t inSize);
    return Offsets::CallID<"gExportedMemAlloc", MallocFn>(inSize);
}

static void gMemFree(void *inPtr) {
    using FreeFn = void (*)(void *inPtr);
    return Offsets::CallID<"gExportedMemFree", FreeFn>(inPtr);
}

LocalizedTextResource::SubtitleMode SubtitleModeOf(std::string_view inText) {
    if (inText == "default"sv) return LocalizedTextResource::SubtitleMode::DEFAULT;
    if (inText == "always"sv) return LocalizedTextResource::SubtitleMode::ALWAYS;
    if (inText == "never"sv) return LocalizedTextResource::SubtitleMode::NEVER;
    throw std::runtime_error(std::format("Unexpected subtitle mode string: '{}'", inText));
}

class LocalizationListener final : public IStreamingSystem::Events {
public:
    struct Override {
        std::string text;
        LocalizedTextResource::SubtitleMode subtitle_mode;
    };

    std::unordered_map<GGUUID, Override> localization;

    static LocalizationListener &Get() {
        static LocalizationListener instance;
        return instance;
    }

    void OnFinishLoadGroup(const Array<Ref<RTTIRefObject>> &inObjects) override {
        for (const auto &Object: inObjects) {
            if (Object->GetRTTI()->Name() == "LocalizedTextResource") {
                auto &text = const_cast<LocalizedTextResource &>(reinterpret_cast<const LocalizedTextResource &>(*Object));
                auto entry = localization.find(text.ObjectUUID);

                if (entry == localization.cend()) {
                    // std::println("No localization found for {}", text.ObjectUUID);
                    continue;
                }

                auto actual = text.Text();
                auto replacement = entry->second.text;
                if (actual == replacement)
                    continue;

                // std::println("Replacing text for {}: '{}' -> '{}'", text.ObjectUUID, actual, replacement);

                auto buffer = static_cast<char *>(gMemAlloc(replacement.size()));
                memcpy(buffer, replacement.data(), replacement.size());

                gMemFree(const_cast<char *>(text.mText));
                text.mText = static_cast<const char *>(buffer);
                text.mTextLength = static_cast<uint16_t>(replacement.size());
                text.mSubtitleMode = entry->second.subtitle_mode;
            }
        }
    }
};

static void (*RTTIFactory_RegistersSymbols)(void *);

static void RTTIFactory_RegistersSymbols_Hook(void *inUnk) {
    RTTIFactory_RegistersSymbols(inUnk);

    static auto registered = [] {
        auto [offset, _] = Offsets::GetModule();
        for (auto &[symbol, hash]: ExportedSymbols::Get().mAllSymbols) {
            if (symbol->mKind == ExportedSymbol::Kind::Function) {
                Offsets::MapAddress(symbol->mName, reinterpret_cast<uintptr_t>(symbol->mLanguage[0].mAddress) - offset);
            }
        }
        return true;
    }();
    (void) registered;
}

static StreamingManager *(*StreamingManager_Constructor)(StreamingManager *);

static StreamingManager *StreamingManager_Constructor_Hook(StreamingManager *manager) {
    auto result = StreamingManager_Constructor(manager);
    manager->mStreamingSystem->AddEventListener(&LocalizationListener::Get());
    return result;
}

void Injector::Attach() {
    // @formatter:off
    Offsets::MapAddress("RTTIFactory::sExportedSymbols", Offsets::OffsetFromInstruction("48 8B 3D ? ? ? ? 48 63 0D ? ? ? ? 48 89 BC 24 90 00 00 00 48 8D 04 CF 48 3B F8 0F 83", 3) - 8);
    Offsets::MapAddress("StreamingManager::Instance", Offsets::OffsetFromInstruction("48 89 05 ? ? ? ? E8 ? ? ? ? 33 D2 41 B8 F8 0A 00 00 48 8B C8 48 8B D8 E8", 3));
    Offsets::MapSignature("StreamingManager::StreamingManager", "48 89 5C 24 08 48 89 6C 24 10 48 89 74 24 18 48 89 7C 24 20 41 56 48 83 EC 20 33 ED 48 8D 05");
    Offsets::MapSignature("RTTIFactory::RegisterSymbols", "48 89 4C 24 08 56 48 83 EC 70 48 89 5C 24 68 48 8D 0D ? ? ? ? 48 89");
    // @formatter:on

    StreamingManager_Constructor = Offsets::ResolveID<"StreamingManager::StreamingManager", decltype(StreamingManager_Constructor)>();
    RTTIFactory_RegistersSymbols = Offsets::ResolveID<"RTTIFactory::RegisterSymbols", decltype(RTTIFactory_RegistersSymbols)>();

    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourAttach(reinterpret_cast<PVOID *>(&StreamingManager_Constructor), static_cast<PVOID>(StreamingManager_Constructor_Hook));
    DetourAttach(reinterpret_cast<PVOID *>(&RTTIFactory_RegistersSymbols), static_cast<PVOID>(RTTIFactory_RegistersSymbols_Hook));
    DetourTransactionCommit();

    std::ifstream stream{"localization.json"};
    if (stream.fail()) {
        // std::println("Failed to open localization file 'localization.json'");
        return;
    }

    const auto json = nlohmann::json::parse(stream);

    auto &localization = LocalizationListener::Get().localization;
    for (auto it = json.cbegin(); it != json.cend(); ++it) {
        auto guid = GGUUID::Parse(it.key());
        auto text = it.value()["text"].get<std::string>();
        auto mode = it.value()["mode"].get<std::string>();
        localization.emplace(guid, LocalizationListener::Override{std::move(text), SubtitleModeOf(mode)});
    }

    // std::println("Loaded {} localization entries", localization.size());
}

void Injector::Detach() {
    DetourTransactionBegin();
    DetourUpdateThread(GetCurrentThread());
    DetourDetach(reinterpret_cast<PVOID *>(&RTTIFactory_RegistersSymbols), static_cast<PVOID>(RTTIFactory_RegistersSymbols_Hook));
    DetourTransactionCommit();
}
