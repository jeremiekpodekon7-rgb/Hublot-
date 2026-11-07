# COSMOS architecture

## Design rule

Keep the game domain portable and deterministic. The GBA layer translates hardware input, timing, video, audio, and cartridge storage into calls to the game core. The core must be host-testable without emulating GBA registers.

## Current modules

- `cosmos::math` — 2:1 projection and Q8.8 world coordinates.
- `cosmos::world::Portal` — deterministic activation/entry/transition/arrival state machine. It stores a destination region ID but does not resolve maps or impose portal-discovery policy.
- `cosmos::world::WorldClock` — deterministic day phase and explicit weather state. Day length is a tunable prototype parameter.
- `cosmos::world::Ecosystem` — deterministic 15×10 local region patch with plants, grazers, predators, rainfall, population counts, and snapshot/double-buffer updates.
- `src/main.cpp` — GBA hardware entry point and temporary pixel diagnostic scene.
- `tests/core_tests.cpp` and `tests/ecology_tests.cpp` — host tests for portable logic and bounded ecological state.

## Planned module boundaries

- `platform/gba` — register access, input polling, VBlank timing, DMA, VRAM/OAM/palette management.
- `render` — isometric tile renderer, camera, depth sorting, occlusion, sprite animation, UI, palette and blending effects.
- `world` — region/chunk loading, objects, portals, weather, day/night, event persistence, collision map.
- `exploration` — continuous movement, interactions, species encounter policies.
- `combat` — tactical grid, individual initiative, per-unit AP, telegraphs, range and terrain effects.
- `character` — independent stats, general progression, class mastery, class-specific and universal skills.
- `persistence` — versioned save data and validation.
- `audio` — music and sound effects with an explicit channel budget.

## Dependency direction

`platform` → adapts hardware for `render` and the game loop. The portable core must not depend on GBA registers, emulator APIs, Android, network services, or a hosted backend. Rendering reads world state; it does not own authoritative world state. Combat and exploration call domain systems rather than directly editing hardware memory.

## Performance policy

- Maintain explicit budgets for VRAM, palette RAM, OAM, per-scanline objects, EWRAM/IWRAM, stack, and frame time.
- Chunk regions and load only nearby map/asset data.
- Suspend off-screen decorative animation where safe; never suspend the logical update that persists an event.
- Keep simulation state fixed-size and deterministic; the first ecology patch is statically budgeted at no more than 512 bytes and updates through a snapshot/double buffer.
- Separate sprite dimensions from collision footprint and ground sorting position.
- Keep combat range and threat overlays visually dominant over decorative VFX.

## Verification levels

1. Host tests for deterministic core math, world state machines, and ecosystem behavior.
2. Cross-compile the GBA target with devkitARM/libgba.
3. Run the ROM in a GBA emulator and inspect boot, input, video timing, transitions, and saves.
4. Test on physical GBA-compatible hardware before declaring a release candidate.

## Build structure

The repository retains the original `libgba` dependency and linker files. The Makefile builds `cosmos.gba`; `make test` compiles the portable core and ecosystem tests with a desktop C++17 compiler.
