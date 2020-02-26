#pragma once
#include <GGEngine/Engine/Utilities/Math/Vec2.h>

enum class InputDivice : uint8_t { Keyboard,
                                   Mouse,
                                   Count };

enum class KeyState : uint8_t {
    Released, // when key is released
    Pressed,  // when key is pressed for the first time
    HeldDown  // when key is pressed for more than one tick
};

using RawKeyCode = int8_t;

struct RawKey {
    RawKey() = default;

    RawKey(RawKeyCode keyCode, InputDivice inputDivice);

    bool operator==(const RawKey rhs) const;

    bool operator!=(const RawKey rhs) const;

public:
    RawKeyCode code = -1;

    InputDivice divice = InputDivice::Keyboard;

    KeyState state = KeyState::Released;
};

namespace std {
template <>
struct hash<RawKey> {
    size_t operator()(const RawKey& key) const
    {
        return hash_combine(hash<size_t>()((size_t)key.code),
                            hash<size_t>()((size_t)key.divice));
    }
};
} // namespace std
