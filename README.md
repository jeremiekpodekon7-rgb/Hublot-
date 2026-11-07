# COSMOS — Legacy of the Eternal

**A native Game Boy Advance isometric, turn-based cosmic-fantasy RPG.**

COSMOS is being built as an offline-playable `.gba` ROM for real GBA hardware and compatible emulators. This branch starts from the repository's existing GBA C/C++ template and keeps the original `master` branch untouched.

## Project status

- [x] GBA C/C++ toolchain scaffold retained
- [x] 2:1 isometric projection and continuous fixed-point coordinates
- [x] Initial portal lifecycle state machine
- [x] Deterministic day/night and weather clock foundation
- [x] Host-side tests for the platform-independent core
- [x] GBA build workflow
- [ ] Exploration movement and collision
- [ ] Tactical grid combat and individual initiative
- [ ] Character/class/stat progression systems
- [ ] Save system and persistent world events
- [ ] Final pixel-art assets, animation sets, music and sound
- [ ] Complete ROM playtest and hardware verification

This is a project foundation and technical test scene, **not a finished game or release ROM**.

## Build

The GBA build requires devkitARM and the GBA development tools. The repository includes the current `libgba` headers, library archive and linker support files.

```sh
make clean
make
```

The output is `cosmos.gba`.

Run the platform-independent core tests on a normal desktop C++17 compiler:

```sh
make test
```

The CI workflow runs the host tests and builds the GBA ROM inside the official `devkitpro/devkitarm` container.

## Technical targets

- GBA display: 240 × 160 pixels, 15-bit colour.
- CPU: ARM7TDMI; native C++17 with exceptions and RTTI disabled.
- Isometric projection: 2:1; one logical ground cell is 32 × 16 pixels, assembled from 8 × 8 hardware tiles.
- Standard character art target: 32 × 40 pixels, eight directions; the ground/collision footprint remains independent of sprite height.
- Exploration: continuous movement; combat: grid-based tactical movement.
- Combat: per-unit action-point reserves and individual speed-based initiative.
- Offline-first: no network dependency in the game runtime.

## Documentation

- `docs/PROJECT_SPEC.md` — consolidated gameplay and platform decisions.
- `docs/GRAPHICS_SPEC.md` — visual direction and GBA graphics constraints.
- `docs/SESSION_RECONCILIATION.md` — cross-session history, accepted decisions, and boundaries with separate RPG projects.
- `docs/ARCHITECTURE.md` — implementation boundaries and build stages.

## License note

This branch retains the repository's existing GPL-3.0 license and the bundled libgba license notices. Review the license obligations before distributing a modified build.
