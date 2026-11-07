#include "cosmos/portal.hpp"

namespace cosmos::world {

Portal::Portal(std::uint16_t portal_id,
               std::uint16_t destination_region_id,
               PortalTiming timing) noexcept
    : portal_id_(portal_id),
      destination_region_id_(destination_region_id),
      timing_(timing) {
    // A timed animation must always advance, even when a caller supplies 0.
    if (timing_.activation_frames == 0) timing_.activation_frames = 1;
    if (timing_.entry_frames == 0) timing_.entry_frames = 1;
    if (timing_.transition_frames == 0) timing_.transition_frames = 1;
    if (timing_.arrival_frames == 0) timing_.arrival_frames = 1;
}

bool Portal::activate() noexcept {
    if (state_ != PortalState::Dormant) return false;
    set_timed_state(PortalState::Activating, timing_.activation_frames);
    return true;
}

bool Portal::enter() noexcept {
    if (state_ != PortalState::Active) return false;
    set_timed_state(PortalState::Entering, timing_.entry_frames);
    return true;
}

void Portal::advance_frames(std::uint32_t frames) noexcept {
    while (frames > 0 && is_timed_state()) {
        if (frames < frames_remaining_) {
            frames_remaining_ = static_cast<std::uint16_t>(frames_remaining_ - frames);
            return;
        }

        frames -= frames_remaining_;
        frames_remaining_ = 0;
        advance_state();
    }
}

void Portal::set_timed_state(PortalState state, std::uint16_t duration) noexcept {
    state_ = state;
    frames_remaining_ = duration == 0 ? 1 : duration;
}

void Portal::advance_state() noexcept {
    switch (state_) {
        case PortalState::Activating:
            state_ = PortalState::Active;
            break;
        case PortalState::Entering:
            set_timed_state(PortalState::Transitioning, timing_.transition_frames);
            break;
        case PortalState::Transitioning:
            set_timed_state(PortalState::Arriving, timing_.arrival_frames);
            break;
        case PortalState::Arriving:
            state_ = PortalState::Active;
            break;
        case PortalState::Dormant:
        case PortalState::Active:
            break;
    }
}

bool Portal::is_timed_state() const noexcept {
    return state_ == PortalState::Activating ||
           state_ == PortalState::Entering ||
           state_ == PortalState::Transitioning ||
           state_ == PortalState::Arriving;
}

}  // namespace cosmos::world
