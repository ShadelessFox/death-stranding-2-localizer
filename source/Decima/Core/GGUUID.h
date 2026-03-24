#pragma once

#include <cstdint>
#include <array>
#include <format>
#include <string>
#include <string_view>
#include <stdexcept>

class GGUUID final {
public:
    template<typename T, size_t Digits = sizeof(T) * 2>
    constexpr static T HexToBytes(const char *inData) {
        auto charToByte = [](char C) constexpr -> uint32_t {
            if (C >= 'A' && C <= 'F')
                return C - 'A' + 10;
            if (C >= 'a' && C <= 'f')
                return C - 'a' + 10;
            if (C >= '0' && C <= '9')
                return C - '0';

            throw std::invalid_argument("Invalid hexadecimal digit");
        };

        T value{};

        for (size_t i = 0; i < Digits; i++)
            value |= charToByte(inData[i]) << (4 * (Digits - i - 1));

        return value;
    }

    static GGUUID Parse(const std::string_view &inUUID) {
        return Parse(inUUID.data(), inUUID.length());
    }

    static GGUUID Parse(const char *inUUID, const size_t inLength) {
        if (inLength != 36)
            throw std::invalid_argument("Invalid GUID length specified");

        GGUUID guid{};
        guid.mData1 = HexToBytes<uint32_t>(inUUID + 0);
        guid.mData2 = HexToBytes<uint16_t>(inUUID + 9);
        guid.mData3 = HexToBytes<uint16_t>(inUUID + 14);
        guid.mData4[0] = HexToBytes<uint8_t>(inUUID + 19);
        guid.mData4[1] = HexToBytes<uint8_t>(inUUID + 21);

        for (int i = 0; i < 6; i++)
            guid.mData4[i + 2] = HexToBytes<uint8_t>(inUUID + 24 + i * 2);

        return guid;
    }

    GGUUID() = default;

    GGUUID(const GGUUID &) = default;

    GGUUID &operator=(const GGUUID &) = default;

    bool operator==(const GGUUID &inRHS) const {
        return std::equal(mData.cbegin(), mData.cend(), inRHS.mData.cbegin(), inRHS.mData.cend());
    };

private:
    friend struct std::formatter<GGUUID>;
    friend struct std::hash<GGUUID>;

    union {
        struct {
            uint32_t mData1;
            uint16_t mData2;
            uint16_t mData3;
            uint8_t mData4[8];
        };

        std::array<uint8_t, 16> mData;
    };
};

template<>
struct std::formatter<GGUUID> {
    static constexpr auto parse(std::format_parse_context &ctx) {
        return ctx.begin();
    }

    static auto format(const GGUUID &value, std::format_context &ctx) {
        return std::format_to(
            ctx.out(),
            "{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}{:02x}",
            value.mData[3], value.mData[2], value.mData[1], value.mData[0],
            value.mData[5], value.mData[4],
            value.mData[7], value.mData[6],
            value.mData[8], value.mData[9],
            value.mData[10], value.mData[11], value.mData[12], value.mData[13], value.mData[14], value.mData[15]
        );
    }
};

template<>
struct std::hash<GGUUID> {
    template<class T>
    static void hash_combine(std::size_t &seed, const T &v) {
        std::hash<T> hasher;
        seed ^= hasher(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
    }

    std::size_t operator()(const GGUUID &value) const noexcept {
        std::size_t hash;
        for (auto b: value.mData)
            hash_combine(hash, b);
        return hash;
    }
};
