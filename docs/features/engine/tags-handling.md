# Tags handling

!!! danger
    This feature may crash the game if it is used on maps that are broken or have corrupted tags. 
    Use it with caution. :skull:

!!! note
    This feature is intended for developers, as it is only accessible through the plugins API.

Provides a set of functions for manipulating tag data, allowing you to create fully 
independent tag copies at runtime. These copies can be modified without altering the original tag 
data, so any modifications can be easily reverted by reloading the original values into their 
copies, making it ideal for generating dynamic content. For example, it can be used for creating 
variations of existing weapons, vehicles, or bipeds that can be used as skins.

The above mentioned function also makes possible the most powerful function of this feature: 
the ability to import tags from other maps. As the name suggests, this allows to copy tags from
other maps and use them in the current loaded one. 

### Virtual tag data

One of the most important parts of this feature is the virtual tag data block. This is a 64 MiB 
memory block that is used to store the tag data of the copied tags. This block is managed by 
Balltze and is independent from the original tag data.

Since the tag array in cache files has a fixed size and new tags cannot be added to the end of it
without overwriting existing map tag data, the tags array is reallocated to the virtual tag data 
block, this means that the pointer of the tag array in the tag data header will be updated to point 
to the new tag array. This is important to note, as it could potentially cause issues with other mods or Chimera Lua scripts that assume the tag array remains in its original location.

