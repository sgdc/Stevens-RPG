Stevens RPG
===========

Well, you can read the docs and see the code... the docs will probably help you the most. <-- Said by nearly every teacher on campus

To Build
--------

Make sure to get the submodule [GamePlay 3D](https://github.com/rcmaniac25/GamePlay) needed for running the code.

If you can't get the submodule, run around in circles until someone questions what you are doing and has the potential to help get a git submodule.

Switch to the "twoD-additions" branch of GamePlay. This is usually bad practice for a production game, but the framework is being worked on while the game is being worked on.

Within GamePlay, compile encoder (Visual Studio project file located within tools/encoder folder) with FBX support.

You need dependencies, so run the install.bat (or .sh) file to get them.

Next, compile GamePlay itself (it will take a good 30+ min). A Visual Studio solution file is located in the root of the GamePlay directory. You only need to compile the gameplay project itself, though feel free to play with the sample projects.
### Remember to compile both Debug and Release builds (also remember, it's about 30+ min each)

Now you can go into Code/StevensRPG and compile the project to run it