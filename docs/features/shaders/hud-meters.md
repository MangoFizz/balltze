# HUD meters

The purpose of this reimplementation is to restore how the HUD meters are rendered in the original xbox
release of the game. 

To ensure compatibility with existing HUD meters, by default, this reimplementation works just like the original Gearbox implementation. Following the example of how it works in TMCC, the new rendering can be activated by setting the `use xbox shading` flag in the meter flags. Once this flag is set, the 
`translucency` and `opacity` fields in the tag will control the transparency and color opacity 
of the meter. Additionally, the alpha channel of the `empty color` field will now be required to control 
the transparency of the empty color of the meter, and the `flash color` field will be used for the draining 
effect.

!!! note
    Since the `use xbox shading` flag is not present in the original game and in its editing kit tools, it is recommended to use invader-edit from [Invader](https://github.com/SnowyMouse/invader) to set this 
    flag in the meter tags.
    <div class="align-center-container">
        <img src="/assets/hud-meters-use-xbox-shading-flag.png" alt="Demo" width="560" height="315">
        <i class="image-caption">Screenshot of the invader-edit tool showing the `use xbox shading` flag of a meter.</i>
    </div>

## Explanation

The Gearbox implementation of the HUD meters present in the PC releases of the game uses a different shading technique. It utilizes each RGB channel of the texture as its own alpha, meaning that the closer the color is to black, the more transparent it becomes. Therefore, you cannot, for example, have a fully black color in the texture because it would be completely transparent. This is not the case in the original Xbox release of the game, where the transparency is controlled by the translucency field in the meter settings. 

Due to this difference in how the transparency is controlled, you cannot use opaque colors in the texture and expect them to be opaque in the game, causing the meters to look too transparent and in some cases, like in bright environments, almost invisible. Here is a comparison of how the meters looks on a bright environment between Gearbox, TMCC, and this reimplementation:

<div class="align-center-container">
    <img src="/assets/hud-meters.png" alt="Demo" width="560" height="315">
    <i class="image-caption">HUD meters comparison between Gearbox, TMCC, and this reimplementation.</i>
</div>

## Commands

### hud_meters_force_xbox_shading

Syntax: `balltze_hud_meters_force_xbox_shading <setting: boolean>`

Set whether to force the rendering of every HUD meter using the xbox shading.
