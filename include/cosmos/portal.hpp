#pragma once

#include <cstdint>

namespace cosmos::world {

enum class PortalState : std::uint8_t {
    Dormant,
    Activating,
    Active,
    Entering,
    Transitioning,
    Arriving,
};

struct PortalTiming {
    std::uint16_t activation_frames{24};
    std::uint16_t entry_frames{12};
    std::uint16_t transition_frames{24};
    std::uint16_t arrival_frames{12};
};

// Deterministic visual/travel state machine. Destination resolution and
// interaction rules deliberately remain outside this class.
class Portal final {
public:
    Portal(std::uint16_t portal_id,
           std::uint16_t destination_region_id,
           PortalTiming timing = {}) noexcept;

    bool activate() noexcept;
    bool enter() noexcept;
    void advance_frames(std::uint32_t frames = 1) noexcept;

    PortalState state() const noexcept { return state_; }
    std::uint16_t portal_id() const noexcept { return portal_id_; }
    std::uint16_t destination_region_id() const noexcept {
        return destination_region_id_;
    }
    std::uint16_t frames_remaining() const noexcept { return frames_remaining_; }
    bool is_active() const noexcept { return state_ == PortalState::Active; }

private:
    void set_timed_state(PortalState state, std::uint16_t duration) noexcept;
    void advance_state() noexcept;
    bool is_timed_state() const noexcept;

    std::uint16_t portal_id_{};
    std::uint16_t destination_region_id_{};
    PortalTiming timing_{};
    PortalState state_{PortalState::Dormant};
    std::uint16_t frames_remaining_{};
};

}  // namespace cosmos::world
