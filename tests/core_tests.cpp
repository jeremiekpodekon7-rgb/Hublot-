#include "cosmos/isometric.hpp"
#include "cosmos/portal.hpp"
#include "cosmos/world_clock.hpp"

#include <cstdlib>
#include <iostream>

namespace {

int failures = 0;

void check(bool condition, const char* description) {
    if (!condition) {
        std::cerr << "FAIL: " << description << '\n';
        ++failures;
    }
}

void test_isometric_projection() {
    using cosmos::math::GridPoint;
    using cosmos::math::WorldPosition;
    using cosmos::math::project;

    const auto origin = project(GridPoint{0, 0});
    check(origin.x == 0 && origin.y == 0, "grid origin projects to screen origin");

    const auto cell = project(GridPoint{3, 2});
    check(cell.x == 16 && cell.y == 40, "32x16 isometric grid projection is 2:1");

    const auto fixed = project(WorldPosition{256, 0});
    check(fixed.x == 16 && fixed.y == 8, "Q8.8 position projects one world cell correctly");
}

void test_portal_state_machine() {
    using cosmos::world::Portal;
    using cosmos::world::PortalState;
    using cosmos::world::PortalTiming;

    const PortalTiming timing{2, 1, 3, 2};
    Portal portal{7, 12, timing};

    check(portal.state() == PortalState::Dormant, "portal starts dormant");
    check(!portal.enter(), "dormant portal cannot be entered");
    check(portal.activate(), "dormant portal can be activated");
    check(portal.state() == PortalState::Activating, "activation state is visible");

    portal.advance_frames(1);
    check(portal.state() == PortalState::Activating, "activation respects its duration");
    portal.advance_frames(1);
    check(portal.is_active(), "portal becomes active after activation duration");
    check(portal.destination_region_id() == 12, "destination id is retained");

    check(portal.enter(), "active portal can begin entry");
    portal.advance_frames(6);
    check(portal.is_active(), "entry, transition and arrival complete in order");
}

void test_zero_duration_is_safe() {
    using cosmos::world::Portal;
    using cosmos::world::PortalState;
    using cosmos::world::PortalTiming;

    Portal portal{1, 2, PortalTiming{0, 0, 0, 0}};
    check(portal.activate(), "zero-duration portal can be activated");
    portal.advance_frames(1);
    check(portal.is_active(), "zero duration is clamped to a safe one-frame phase");
}

void test_world_clock() {
    using cosmos::world::DayPhase;
    using cosmos::world::WeatherCondition;
    using cosmos::world::WorldClock;
    using cosmos::world::WorldClockConfig;

    WorldClock clock{WorldClockConfig{400}};
    check(clock.day_phase() == DayPhase::Dawn, "day starts at dawn");
    clock.advance(100);
    check(clock.day_phase() == DayPhase::Day, "quarter day reaches daytime");
    clock.advance(100);
    check(clock.day_phase() == DayPhase::Dusk, "half day reaches dusk");
    clock.advance(100);
    check(clock.day_phase() == DayPhase::Night, "three-quarter day reaches night");
    clock.advance(100);
    check(clock.day_phase() == DayPhase::Dawn, "clock wraps at the next day");

    clock.set_weather(WeatherCondition::Cosmic);
    check(clock.weather() == WeatherCondition::Cosmic, "weather can be changed deterministically");
}

}  // namespace

int main() {
    test_isometric_projection();
    test_portal_state_machine();
    test_zero_duration_is_safe();
    test_world_clock();

    if (failures != 0) {
        std::cerr << failures << " test(s) failed\n";
        return EXIT_FAILURE;
    }

    std::cout << "COSMOS core tests: all passed\n";
    return EXIT_SUCCESS;
}
