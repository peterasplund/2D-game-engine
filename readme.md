SDL Game engine

TODO:

Tile map:
- Entity position seems bugged
- Handle multiple neighbours at the same direction. We need this since some rooms can be taller/wider

Gameplay:
- Clear entities on level transition (Rewrite entity system?)

- Slopes
- Build macro to map class property to imgui controls (in a seperate tab). This will speed up debugging player controls.
- Finish Dialogue system
- Separate Font drawing from dialogue system to be able to print menus and such
- Map manager with transition
- Decided to use grid-based or free movement in overworld

deps:
- SDL2-2.0.10
- SDL2_image-2.0.5
