# COSMOS — Cross-session reconciliation

**Purpose:** keep every relevant COSMOS conversation connected to one project contract without silently merging requirements from different games.

## 1. Source classification

### Direct COSMOS direction

The prior session titled **“Créer un RPG Wakfu cosmique”** established the high-level theme: an isometric, turn-based GBA RPG in a cosmic setting, with Wakfu as an inspiration. It did not establish permission to copy Wakfu characters, factions, locations, stories, art, music, UI, or protected assets.

The follow-up COSMOS design sessions established the detailed platform, gameplay, character progression, and graphics decisions recorded in `PROJECT_SPEC.md` and `GRAPHICS_SPEC.md`.

### Related but separate RPG material — not imported as COSMOS requirements

The Library file `momo.txt` describes a distinct project centred on Kamel, magic and the Ombrum/shadow system. Its requested technical target is Godot 4 / Android / Java with extensive procedural SDF worlds. It also describes many separate systems (shadow extraction, a fairy companion, large-scale planetary travel, crafting, NPC chat, and an ATB bar that continues filling in real time).

Those ideas are valuable historical context, but they are **not automatically COSMOS decisions**:
- The platform and rendering assumptions conflict with a native GBA ROM.
- Its ATB timing rules conflict with COSMOS's accepted individual speed-based initiative and per-unit AP activation model.
- Its scale claims (dozens of planets, tens of thousands of species, hundreds of cities per planet) are not accepted COSMOS scope and would be incompatible with the GBA's memory, storage, and CPU limits if interpreted literally.
- Its protagonist, shadow-extraction mechanics, fairy system, and numerical progression formulas belong to that separate RPG unless explicitly adopted later.

The repository `MOMO-RPG-Engine` is likewise an Android Java RPG engine, not the source base for COSMOS.

### Earlier GBA ecosystem prototype

The Library archive `verdant-gba-project.zip` and Git bundle contain a separate native GBA prototype called **Verdant**. Its source models a small ecosystem of plants, grazers and predators with deterministic seeds and rain events. Its archived ROM header identifies it as `VERDANT`; it is not a COSMOS build. The archive's own report says the ROM was not playtested in an emulator or on physical hardware.

The ecosystem concept is relevant to COSMOS's approved living-environment direction. The portable logic has therefore been **reimplemented in COSMOS C++**, rather than importing Verdant's GBA renderer or copying its source files. The COSMOS version adds snapshot/double-buffer updates so a creature moved to a later cell is not processed a second time during the same simulation tick. The initial patch is deliberately bounded to 15×10 logical cells and a 512-byte state budget. This is a local ecology prototype, not a claim of a complete ecosystem or full fauna simulation.

The THEOS-0 / GENESIS / THEOS Fantasy SDF research projects are also separate digital-simulation projects. Their code, SDF engine, physics model, ThetaB2 controller, and AI systems are not part of COSMOS.

Earlier Eclypsia and other RPG concept images are reference material from different projects, not COSMOS asset approvals.

## 2. What is consolidated into COSMOS

The following decisions are the current COSMOS contract:

### Product and platform
- Name: **COSMOS — Legacy of the Eternal**.
- Deliverable: a real, offline-playable Nintendo Game Boy Advance `.gba` ROM.
- Native implementation: C/C++ on ARM7TDMI using the retained GBA/libgba toolchain.
- This is not an Android APK, Godot project, web game, or 3D/SDF renderer.
- All game-specific characters, places, story, tiles, sprites, sound, and music must be original.

### Game structure
- Free continuous movement during exploration.
- Visible enemies in the exploration world.
- Species/context-specific encounter initiation: contact, pursuit/aggression, ranged trigger, or territorial/scripted behavior.
- Tactical turn-based battles on a grid.
- Initiative is individual and ordered by speed.
- Every unit owns its AP reserve. Unused AP expire at the end of its activation and are restored at its next activation.

### Characters and progression
- Stats are specialized and independent; every shipped stat needs a measurable effect, defined range, and tests.
- Character growth combines natural class growth and freely allocated stat points.
- Off-specialty stats remain improvable at a higher cost.
- Only one class is active at a time.
- Class changes occur outside combat and are free; zones or situations may restrict them.
- General character level and per-class mastery are separate; class mastery persists after changing class.
- Class-specific skill progress is retained but those skills are unavailable while another class is active.
- Universal skills have separate mastery and progress through meaningful use.
- Instructive failures may teach; trivial repeated failure must not be farmable.
- Acquired knowledge does not decay automatically.
- The 48-stat catalogue is provisional, not a final approved stat list.
- Respecialization conditions and party knowledge sharing remain unresolved.

### Visual and world direction
- Cosmic fantasy with luminous, ancient astral, and dark-cosmic regional identities.
- 2:1 isometric projection; logical ground cell 32×16 pixels; hardware tiles 8×8.
- Standard humanoid target 32×40 pixels, with eight directions and 4–8 frames for ordinary actions.
- Camera scrolling follows the player; cinematic reframing is used for important events.
- Living environments, animated effects, reactive terrain, regional palettes, simulated localized lighting, ecosystem updates, and readable tactical overlays.
- Authored regions combined with reusable modules; procedural recomposition only where useful; chunked loading.
- Day/night, weather, cosmic atmospheric events, and portal transitions belong to the world model. Their final timings and gameplay effects remain to be balanced.

## 3. Wakfu-inspired direction — inspiration, not imitation

The Wakfu reference should guide broad creative qualities rather than reproduce protected content:
- a world whose ecosystems, creatures, environments, and civilizations feel connected;
- visually distinct regions with recognizable identities;
- a sense of adventure, discovery, humour or wonder where appropriate, and meaningful consequences;
- elemental and cosmic phenomena expressed through original mechanics and visual language.

No specific Wakfu class, character, faction, location, lore, sprite, animation, soundtrack, map, or user-interface asset is approved for reuse.

## 4. Hardware scope guardrails

The GBA has a 240×160 display and tight CPU, VRAM, OAM, palette, and cartridge-storage budgets. COSMOS therefore uses:
- compact authored regions rather than a literal unlimited universe;
- reusable tile/object sets and data-driven encounters;
- chunk/area transitions and selective loading;
- bounded entity, animation, and effect budgets;
- deterministic offline simulation and versioned local saves.

A cosmic or multi-world feeling can be delivered through carefully designed regions, portals, maps, skyboxes/background layers, and progression. A specific planet count, city count, species count, or content volume is **not** approved by analogy with another project's notes.

## 5. Open decisions to carry forward

- Main character, party size, story, cosmology, region names, and world-map structure.
- Whether COSMOS ultimately contains multiple planets/worlds, and how many compact regions are feasible.
- Final class roster, stat catalogue, numeric formulas, XP curves, caps, and respecialization conditions.
- Portal discovery, activation, destination selection, and network rules.
- Final day duration, weather schedules, and weather effects on gameplay.
- Save format, slot count, save triggers, and death/recovery rules.
- Whether acquired knowledge or universal skill progress can be shared between party members.
- The exact lifecycle policy for environment animations and persistent reactions.
- Whether to expand the ecology patch with additional plants, food webs, terrain interactions, or regional species after defining their hardware budgets.
- Any adoption of a fairy companion, shadow/summon system, title collection, crafting volume, or NPC simulation from separate RPG notes. These require an explicit COSMOS design decision and a GBA feasibility review.

## 6. Implementation authority

The active implementation owner is this branch:

`feature/cosmos-gba-foundation`

The original `master` branch remains the preserved GBA template. The current code foundation is deliberately small: projection math, portal lifecycle, world clock/weather state, a bounded ecological patch, a temporary GBA test scene, host-side tests, and CI. This document does not claim that exploration, combat, story, assets, saves, or the complete game have been implemented.
