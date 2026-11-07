# COSMOS graphics specification

This document records the user's approved visual decisions. Values are logical design targets; every asset and effect must be profiled on GBA hardware/emulation.

## Approved direction

1. Projection: 2:1 isometric.
2. Logical ground cell: 32 × 16 pixels, assembled from four-by-two 8 × 8 hardware tiles.
3. Art direction: evolving fusion of luminous cosmic fantasy, ancient astral fantasy, and dark cosmic fantasy; regional variation retains a shared identity.
4. Colour: common cosmic identity with regional palettes that evolve by environment and story.
5. Characters: detailed, recognizable silhouettes; equipment, transformations, auras, class and power can affect appearance.
6. Camera: scrolling camera follows the player in exploration; cinematic reframing is allowed for important events. Zoom is simulated through framing and sprite changes, not assumed to be continuous hardware zoom.
7. Environment: animated water, waterfalls, vegetation, crystals, cosmic particles, weather, and atmospheric phenomena. Off-screen or distant low-priority animations may be suspended.
8. Depth: dynamic sorting by ground position, with occlusion masks for tall objects. Collision is independent of the visual height of a sprite.
9. Battles: an isometric tactical grid, distinct action animations, cosmic powers, and reactive terrain. Range, impact zones, obstacles, and unit positions must remain legible.
10. UI: compact fusion of fantasy and cosmic visual language, with astronomy symbols, geometry, and restrained luminous details. Readability at 240 × 160 is mandatory.
11. Ordinary action animation: 4–8 frames for walking, attacks, dodges, defence, spells, and damage reactions. Special transformations, ultimates, and bosses may receive exceptional treatment.
12. Standard humanoid sprite target: 32 × 40 pixels. Larger creatures and bosses may use individual sizes. Sprite height does not determine ground collision footprint.
13. Character directions: eight directions (N, NE, E, SE, S, SW, W, NW).
14. Effects: reusable independent effects for auras, transformations, projectiles, impacts, explosions, cosmic particles, elemental magic, and environmental events.
15. Lighting: simulated localized lighting using palettes, sprites, layers, and GBA blending effects; no modern 3D dynamic-lighting assumption.
16. Maps: authored regional layouts combined with reusable modules; procedural recomposition only where useful; chunked loading.
17. Environment animation: cycles, event-driven reactions, and persistent world changes.
18. Time and atmosphere: day/night plus weather and cosmic atmospheric events, with region-aware palettes and effects.
19. Transitions: animated portals and physical passages. The portal's exact activation/discovery policy remains a product decision still awaiting confirmation.

## Hardware implementation rules

- Display: 240 × 160 pixels.
- Logical isometric cell: 32 × 16; hardware tile: 8 × 8.
- Standard character target: 32 × 40, eight directions.
- 4bpp/8bpp tile and sprite formats must be selected per asset family with palette costs tracked.
- Prefer tile backgrounds, hardware scrolling, and OAM sprites for the main world. Mode 3 is permitted for bounded diagnostics or special effects, not as the default full-world renderer.
- Effects are budgeted by visible scanline and frame cost, not only by total sprite count.
- When hardware budgets are exceeded, preserve gameplay readability and player control before decorative effects.

## Projection

For integer logical-cell coordinates `(x, y)` relative to a map origin:

`screen_x = (x - y) * 16`

`screen_y = (x + y) * 8`

Continuous exploration coordinates use signed Q8.8 fixed-point values, where 256 units equal one logical world cell. Projection remains integer-only for deterministic performance.

## Temporary implementation note

The first GBA scene uses primitive pixel geometry to validate the projection and portal glow loop. It is not final pixel art. The current player marker is a placeholder and will be replaced by sprite sheets without changing the collision-footprint contract.
