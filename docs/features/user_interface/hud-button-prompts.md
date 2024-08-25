# HUD button prompts

Restores the button prompts that are displayed in the HUD when using a gamepad. It
behaves like in HCEA in MCC, where the button prompts are only displayed in the HUD when using 
a gamepad.

### Demo video

<iframe width="560" height="315" src="https://www.youtube.com/embed/zxXuw4jyv4w?si=OWo0t95_Rz_z_7dl" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>


## Configuration

[Like in the gamepad pause button feature](/features/gamepad-pause-button#configuration), the HUD 
button prompts can be configured in the gamepad config file. The field `icons` maps the gamepad 
buttons to the corresponding icons that will be displayed in the HUD.

### Example of config file

```json title="My Games\Halo CE\balltze\config\gamepad\xbox_360_windows.json"
{
    "icons": {
        "button_0": 0,
        "button_1": 1,
        "button_2": 2,
        "button_3": 3,
        "button_4": 4,
        "button_5": 5,
        "button_6": 13,
        "button_7": 12,
        "button_8": 14,
        "button_9": 15,
        "axis_4+": 6,
        "axis_4-": 7
    },
    "pause_button": 7
}
```

You can see some examples at the [default config files](https://github.com/MangoFizz/balltze/tree/master/data).
