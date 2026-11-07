#pragma once

#include <cstdint>

namespace cosmos::math {

// Screen projection for a 2:1 isometric grid.
// A logical cell is 32x16 pixels; coordinates use logical world cells.
struct GridPoint {
    std::int32_t x{};
    std::int32_t y{};
};

struct ScreenPoint {
    std::int32_t x{};
    std::int32_t y{};
};

// Continuous world coordinates use signed Q8.8 fixed-point values:
// 256 units represent one logical world cell.
struct WorldPosition {
    std::int32_t x_q8{};
    std::int32_t y_q8{};
};

constexpr std::int32_t kWorldUnitsPerCell = 256;

constexpr ScreenPoint project(GridPoint point) noexcept {
    return {
        (point.x - point.y) * 16,
        (point.x + point.y) * 8,
    };
}

constexpr ScreenPoint project(WorldPosition position) noexcept {
    return {
        ((position.x_q8 - position.y_q8) * 16) / kWorldUnitsPerCell,
        ((position.x_q8 + position.y_q8) * 8) / kWorldUnitsPerCell,
    };
}

}  // namespace cosmos::math
