The Dead Forest
=====

It is 2d top view cross platform shooting game based on [cocos2d-x](http://cocos2d-x.org).
Graphic concept is [darkwood](http://store.steampowered.com/app/274520/) or gta2 and gaming concept is similar to [left 4 dead](http://store.steampowered.com/agecheck/app/550/), [h1z1](http://store.steampowered.com/app/295110/), [dayz](http://store.steampowered.com/agecheck/app/221100/) and so on. This game contains map editor (such as starcraft's campaign editor) and editor can make many datas such as entity's position, power, types, game logic (that is called trigger). If you have ever use starcraft campaign editor, you know what this sound.


3rd party libraries
- [boost](http://boost.org)
- [imgui](https://github.com/ocornut/imgui)
- [clipper](http://www.angusj.com/delphi/clipper.php)
- [tinyxml2](http://www.grinninglizard.com/tinyxml2/)
- [flatbuffers](https://google.github.io/flatbuffers/)

![architecture](http://i.imgur.com/1ZDGMvw.png)

Architecture of a game is largely has three configurations. **Editor**, **Client** and **Server**.

**Editor** helps ensure that the game is designed to be data-driven. In other words, it allows almost all parts of data from a simple variable to game logic.

![Editor1](http://i.imgur.com/T0qvX9g.png)

![Editor2](http://i.imgur.com/oLbEKhg.png)

![Editor3](http://i.imgur.com/LRVqhhA.png)

You can test and play with real-time.

**Client** run real-time rendering to the actual game data using the built in editor. [cocos2d-x](http://cocos2d-x.org) game engine is helps this works. Since the game is a large game world we were using various optimization techniques(such as cell space partition, deferred rendering)

**Server** has not yet been developed.

How to build
-------

1. Download and build [cocos2d-x](http://cocos2d-x.org) project, [boost](http://boost.org) library.
2. Clone the repository.
3. Paste to Classes, Resource, cocos2d folders.
4. play.


License
-------
it is licensed under the GNU License, see LICENSE for more information.

