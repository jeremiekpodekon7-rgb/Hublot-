#include "cosmos/world_clock.hpp"

namespace cosmos::world {

WorldClock::WorldClock(WorldClockConfig config) noexcept : config_(config) {
    if (config_.ticks_per_day == 0) config_.ticks_per_day = 1;
}

void WorldClock::advance(std::uint32_t ticks) noexcept {
    total_ticks_ += ticks;
}

std::uint32_t WorldClock::tick_of_day() const noexcept {
    return total_ticks_ % config_.ticks_per_day;
}

DayPhase WorldClock::day_phase() const noexcept {
    const std::uint32_t quarter = (tick_of_day() * 4U) / config_.ticks_per_day;
    switch (quarter) {
        case 0: return DayPhase::Dawn;
        case 1: return DayPhase::Day;
        case 2: return DayPhase::Dusk;
        default: return DayPhase::Night;
    }
}

}  // namespace cosmos::world
