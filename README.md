The Dead Forest
=====

it is 2d top view cross platform shooting game based on [cocos2d-x](http://cocos2d-x.org).
graphic concept is [darkwood](http://store.steampowered.com/app/274520/) or gta2 and gaming concept is similar to [left 4 dead](http://store.steampowered.com/agecheck/app/550/), [h1z1](http://store.steampowered.com/app/295110/), [dayz](http://store.steampowered.com/agecheck/app/221100/) and so on. this game contains map editor (such as starcraft's campaign editor) and editor can make many datas such as entity's position, power, types, game logic (that is called trigger). if you have ever use starcraft campaign editor, you know you know what this sound.

3rd party library
- [boost](http://boost.org)
- [imgui](https://github.com/ocornut/imgui)

![architecture](http://i.imgur.com/1ZDGMvw.png)

Architecture of a game is largely has three configurations. **Editor**, **Client** and **Server**.

**Editor** helps ensure that the game is designed to be data-driven. In other words, it allows almost all parts of data from a simple variable to game logic.

![Editor1](http://i.imgur.com/T0qvX9g.png)

![Editor2](http://i.imgur.com/oLbEKhg.png)

![Editor3](http://i.imgur.com/8SU7Xjb.png)

**Client** run real-time rendering to the actual game data using the built in editor. [cocos2d-x](http://cocos2d-x.org) game engine is helps this work. Since the game is a large game world we were using various optimization techniques(such as cell space partition, deferred rendering)

![CellSpacePartition](http://i.imgur.com/tCV8nCm.jpg)

License
-------

