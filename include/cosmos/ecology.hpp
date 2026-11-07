#pragma once
#include <array>
#include <cstdint>
namespace cosmos::world {
enum class CellKind : std::uint8_t { Grass, Plant, Grazer, Predator, Water };
struct EcosystemCell { CellKind kind{CellKind::Grass}; std::uint8_t energy{}; std::uint8_t age{}; };
struct PopulationCounts { std::uint16_t grass{}, water{}, plants{}, grazers{}, predators{}; };
// One deterministic local ecosystem patch. Coordinates are logical world cells;
// projection/rendering belongs to the isometric renderer, not this model.
class Ecosystem final {
public:
    static constexpr int kWidth=15, kHeight=10, kCellCount=kWidth*kHeight;
    void initialize(std::uint32_t seed) noexcept;
    void step() noexcept;
    bool seed_plant(int x,int y) noexcept;
    void make_rain(std::uint16_t duration=16) noexcept;
    const EcosystemCell* cell(int x,int y) const noexcept;
    PopulationCounts population() const noexcept;
    bool valid() const noexcept;
    std::uint32_t tick_count() const noexcept { return ticks_; }
    std::uint16_t rain_ticks_remaining() const noexcept { return rain_ticks_; }
    std::uint32_t random_state() const noexcept { return rng_; }
private:
    using CellArray=std::array<EcosystemCell,kCellCount>;
    static bool inside(int x,int y) noexcept;
    static int index_of(int x,int y) noexcept;
    std::uint32_t next_random() noexcept;
    unsigned random_below(unsigned limit) noexcept;
    bool find_neighbour(int x,int y,unsigned start,int& nx,int& ny) const noexcept;
    bool place_random(CellKind kind,std::uint8_t energy) noexcept;
    CellArray cells_{};
    std::uint32_t rng_{0xC001D00Du}, ticks_{};
    std::uint16_t rain_ticks_{};
};
} // namespace cosmos::world
