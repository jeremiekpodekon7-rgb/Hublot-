#pragma once

#include <cstdint>

namespace cosmos::world {

enum class DayPhase : std::uint8_t {
    Dawn,
    Day,
    Dusk,
    Night,
};

enum class WeatherCondition : std::uint8_t {
    Clear,
    Rain,
    Fog,
    Snow,
    Storm,
    Cosmic,
};

struct WorldClockConfig {
    // Prototype tuning value only. The final duration will be balanced later.
    std::uint32_t ticks_per_day{2400};
};

class WorldClock final {
public:
    explicit WorldClock(WorldClockConfig config = {}) noexcept;

    void advance(std::uint32_t ticks = 1) noexcept;
    void set_weather(WeatherCondition weather) noexcept { weather_ = weather; }

    std::uint32_t total_ticks() const noexcept { return total_ticks_; }
    std::uint32_t tick_of_day() const noexcept;
    DayPhase day_phase() const noexcept;
    WeatherCondition weather() const noexcept { return weather_; }

private:
    WorldClockConfig config_{};
    std::uint32_t total_ticks_{};
    WeatherCondition weather_{WeatherCondition::Clear};
};

}  // namespace cosmos::world
