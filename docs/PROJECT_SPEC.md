# COSMOS — consolidated project specification

## Product identity

COSMOS — Legacy of the Eternal is an original cosmic-fantasy RPG for the Nintendo Game Boy Advance. The target deliverable is a real `.gba` ROM, not an Android application, web game, or mockup. The game is designed to work offline.

All characters, maps, world history, sprites, music, and sound effects will be original. Other games may inspire broad genre conventions, but their protected characters, art, maps, names, and music must not be copied.

## Core gameplay

- Hybrid structure: free exploration and tactical turn-based battles.
- Exploration movement is continuous rather than tile-stepped.
- Tactical combat uses a grid.
- Enemies are visible in the exploration world.
- Encounter initiation varies by species and context: contact, pursuit/aggression, ranged trigger, or territorial/scripted behavior.
- Initiative is individual and ordered by speed; it is not a simple team-wide turn.
- Every unit has its own action-point (AP) reserve. Unused AP expire at the end of that unit's activation and are restored at its next activation.

## Character and progression rules

- Stats are specialized and independent: raising one does not automatically raise unrelated stats.
- Every implemented stat must have a measurable gameplay effect, a defined range, and testable interactions.
- Character growth is hybrid: natural class growth plus freely allocated stat points.
- Stats outside a class's specialty remain improvable, but at higher cost.
- Respecialization is conditional (for example, through a master, resource, or quest); the exact conditions are not yet finalized.
- Classes can be changed, but only one class is active at a time. There is no simultaneous dual-class system.
- Class changes happen outside combat. Normal menu changes are allowed; some zones or situations may impose restrictions. Sanctuaries can reveal or unlock special classes.
- Class change is free: no currency, consumable, AP, or preparation-time cost.
- Unlocking a new class may require combined progression, exploration, quests, or trials.
- A character has a general level and a separate mastery level for each class. Class mastery persists when changing class.
- Class-specific skills are unavailable while another class is active, but their learned progress is retained.
- Universal skills have mastery independent of class and grow through meaningful use. Repetition without useful novelty should provide little or no progress.
- Instructive failures may provide knowledge or progress; trivial repeated failure should not be farmed.
- Acquired knowledge is permanent; automatic decay is not part of the current design.
- Whether knowledge can be shared between party members remains undecided.

## Stats and formulas

A preliminary catalogue of **48 candidate stats** was proposed (six families of eight). It is not yet an approved final catalogue; every candidate must be checked for overlap, observability, balance, and a distinct formula/effect before implementation.

The current additive baseline model is:

`S_final = S_base + B_class + B_equipment + B_passive + B_temporary`

Conditional effects and caps must be documented separately rather than hidden in unexplained bonuses.

## World and persistence

- The world uses a hybrid evolution model: authored regional identity plus persistent responses to meaningful events.
- Regions are assembled from manually designed layouts and reusable modular tiles/objects. Procedural recomposition is used only where it adds value.
- Large regions are divided into chunks and loaded progressively.
- The environment combines cyclical animation, event-driven reactions, and persistent evolution.
- A first bounded ecological patch now models plants, grazers and predators across 15×10 logical cells, with deterministic seeded updates and rainfall. It is a local prototype, not a full flora/fauna simulation or a claim that every biome is populated.
- Day/night and weather are part of the world model. Weather types may include rain, fog, snow, storms, and cosmic phenomena. Exact durations and gameplay effects remain to be balanced.
- Portal interaction/discovery rules remain unresolved. The initial code models the portal lifecycle without choosing whether every portal is freely traversable, interactively activated, or part of a wider network.

## GBA constraints

- Target resolution: 240 × 160.
- ARM7TDMI target with strict memory and frame-time budgets.
- VRAM: 96 KiB; hardware tile backgrounds and sprites are preferred over repeated full-screen bitmap redraws.
- All gameplay-critical systems must remain deterministic and work without network access.
- Save data and world-state persistence require an explicit, versioned format before implementation.

## Not yet approved

- Final game story, protagonist, party size, region names, and world map.
- Final stat catalogue, numeric formulas, caps, XP curves, class list, and exact respec conditions.
- Portal activation, discovery, and travel-network policy.
- Real-time duration of a day, weather frequency, and weather gameplay modifiers.
- Save-slot count, save triggers, and data layout.


## Cross-session continuity

The original cosmic-Wakfu-inspired RPG direction and the later COSMOS mechanics/graphics studies are reconciled in [SESSION_RECONCILIATION.md](SESSION_RECONCILIATION.md). That record separates direct COSMOS decisions from historical notes belonging to other RPG and simulation projects. Do not silently import Android/Godot, SDF, shadow-extraction, ATB, or oversized planetary-world requirements from those separate projects.
