# Gamepad pause button

Allows you to bind a gamepad button to pause the game. By default, the game is 
paused by pressing the `ESC` key on the keyboard, but with this feature, you can bind it to 
any button on your gamepad.

## Configuration

The gamepad pause button can be configured at the field `pause_button` in the gamepad config 
file, which is located in the `My Games\Halo CE\balltze\config\gamepad` folder. The active
gamepad config can be selected in the `settings.json` at the field `gamepad`.

### Predefined gamepad models

At the moment, config files for the following gamepad models are already available:

- `xbox_360_windows`

- `dual_shock_1_windows`

You can choose one of these config files or create your own gamepad config file by 
creating a new JSON file in the `gamepad` folder and then setting the `pause_button` field 
to the button index that you want to use. 

Then you can set the your gamepad config in the `settings.json` file.

### Example of config files

```json title="My Games\Halo CE\balltze\config\settings.json"
{
    "gamepad": "xbox_360_windows"
}
```

```json title="My Games\Halo CE\balltze\config\gamepad\xbox_360_windows.json"
{
    "pause_button": 3
}
```

!!! note
    The value of the `pause_button` field must be a number that represents the button index. The
    possible values for the button index depend on the gamepad model. 

You can see some examples at the [default config files](https://github.com/MangoFizz/balltze/tree/master/data).
