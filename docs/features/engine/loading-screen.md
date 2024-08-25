# Loading screen 

Adds a loading screen similar to the one from the Original Xbox version of the game.

The loading screen also helps to prevent the game from freezing or timing out on multiplayer
games when loading maps with a lot of tags or when plugins perform heavy operations during the 
loading process. Is recommended to keep this feature enabled.

Thanks to [@SnowyMouse](https://github.com/SnowyMouse) for the [shader](https://gist.github.com/SnowyMouse/eb7558d2e036e4becbc764501d99a703) that was used to create the loading screen effect. :frog:


<iframe width="560" height="315" src="https://www.youtube.com/embed/r3idye9d1vM?si=NTWQcZq6NYkqrgDA" title="YouTube video player" frameborder="0" allow="accelerometer; autoplay; clipboard-write; encrypted-media; gyroscope; picture-in-picture; web-share" referrerpolicy="strict-origin-when-cross-origin" allowfullscreen></iframe>

*Demo video of the loading screen.*

## Commands

### enable_loading_screen

Syntax: `balltze_enable_loading_screen <setting: boolean>`

Set whether to set up loading screen at startup.

### test_loading_screen

Syntax: `balltze_test_loading_screen`

Plays loading screen demo.

## Configuration

The loading screen can be configured by editing the field `loading_screen` in the `settings.json`
in the `My Games\Halo CE\balltze` folder. The available options are:

- `enabled`: `boolean` - If the loading screen is enabled or not. Default is `true`.

### Example of config file

```json title="My Games\Halo CE\balltze\config\settings.json"
{
    "loading_screen": {
        "enabled": true
    }
}
```
