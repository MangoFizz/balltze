# Map textures preloading

Preloads the textures of the map when the game is loading. This feature can help to reduce 
the stuttering that occurs when the game loads textures during gameplay, especially on maps 
with a lot of high-resolution textures.

## Configuration

The map textures preloading can be configured by editing the field `preload_map_textures`.
The available options are:

- `enabled`: `boolean` - If the map textures preloading is enabled or not. Default is `false`.

- `min_map_size`: `number` - The minimum size of the map in MB to preload the textures, if
the map size is less than this value, the textures will not be preloaded. Default is `384`.

### Example of config file

```json title="My Games\Halo CE\balltze\config\settings.json"
{
    "preload_map_textures": {
        "enabled": true,
        "min_map_size": 384
    }
}
```
