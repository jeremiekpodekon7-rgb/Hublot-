#include <gba.h>

#include "cosmos/isometric.hpp"

namespace {

constexpr int kScreenWidth = 240;
constexpr int kScreenHeight = 160;
constexpr int kOriginX = 120;
constexpr int kOriginY = 78;
constexpr int kPortalRadiusX = 10;
constexpr int kPortalRadiusY = 15;

volatile u16* const kFramebuffer = reinterpret_cast<volatile u16*>(0x06000000u);

constexpr u16 rgb15(unsigned red, unsigned green, unsigned blue) noexcept {
    return static_cast<u16>((red & 31U) | ((green & 31U) << 5U) | ((blue & 31U) << 10U));
}

void put_pixel(int x, int y, u16 color) noexcept {
    if (x < 0 || x >= kScreenWidth || y < 0 || y >= kScreenHeight) return;
    kFramebuffer[y * kScreenWidth + x] = color;
}

void clear_screen(u16 color) noexcept {
    for (int index = 0; index < kScreenWidth * kScreenHeight; ++index) {
        kFramebuffer[index] = color;
    }
}

void draw_diamond(int center_x, int center_y, u16 fill, u16 edge) noexcept {
    for (int y = -7; y <= 8; ++y) {
        const int distance_y = y < 0 ? -y : y;
        const int half_width = 15 - (distance_y * 15) / 8;
        for (int x = -half_width; x <= half_width; ++x) {
            const bool border = (x == -half_width || x == half_width || y == -7 || y == 8);
            put_pixel(center_x + x, center_y + y, border ? edge : fill);
        }
    }
}

void draw_world_floor() noexcept {
    using cosmos::math::GridPoint;
    using cosmos::math::project;

    clear_screen(rgb15(2, 3, 12));
    for (int world_y = -4; world_y <= 4; ++world_y) {
        for (int world_x = -4; world_x <= 4; ++world_x) {
            const auto point = project(GridPoint{world_x, world_y});
            const bool alternate = ((world_x + world_y) & 1) != 0;
            const u16 fill = alternate ? rgb15(5, 9, 24) : rgb15(4, 7, 20);
            draw_diamond(kOriginX + point.x, kOriginY + point.y, fill, rgb15(8, 13, 30));
        }
    }
}

void draw_portal(int center_x, int center_y, u16 glow) noexcept {
    for (int y = -kPortalRadiusY; y <= kPortalRadiusY; ++y) {
        for (int x = -kPortalRadiusX; x <= kPortalRadiusX; ++x) {
            const int ellipse = (x * x * 100) / (kPortalRadiusX * kPortalRadiusX) +
                                (y * y * 100) / (kPortalRadiusY * kPortalRadiusY);
            if (ellipse >= 72 && ellipse <= 128) {
                put_pixel(center_x + x, center_y + y, glow);
            }
        }
    }
}

void draw_player_marker(int center_x, int center_y) noexcept {
    // Temporary collision-footprint marker; final 32x40 directional sprites
    // will replace it without changing the logical 32x16 ground footprint.
    for (int y = -5; y <= 5; ++y) {
        for (int x = -3; x <= 3; ++x) {
            put_pixel(center_x + x, center_y + y, rgb15(25, 24, 31));
        }
    }
    for (int y = -8; y <= -6; ++y) {
        for (int x = -2; x <= 2; ++x) {
            put_pixel(center_x + x, center_y + y, rgb15(31, 24, 16));
        }
    }
}

void wait_vblank() noexcept {
    while (REG_VCOUNT >= 160) {}
    while (REG_VCOUNT < 160) {}
}

}  // namespace

int main() {
    REG_DISPCNT = MODE_3 | BG2_ON;
    draw_world_floor();

    const auto portal_grid = cosmos::math::project(cosmos::math::GridPoint{2, -2});
    const int portal_x = kOriginX + portal_grid.x;
    const int portal_y = kOriginY + portal_grid.y;
    draw_player_marker(kOriginX, kOriginY + 12);

    constexpr u16 glow_cycle[] = {
        rgb15(8, 20, 31), rgb15(12, 25, 31), rgb15(17, 18, 31), rgb15(10, 14, 29),
    };
    unsigned frame = 0;
    for (;;) {
        wait_vblank();
        draw_portal(portal_x, portal_y, glow_cycle[(frame++ >> 3U) & 3U]);
    }
}
