# Eldritch2 Test Repository

As of right now most of the project is still in a preliminary stage and most every interface should be considered in an extremely volatile/unfinished state.

Additionally, the project is not currently buildable from a direct Git clone; the original makes use of several additional libraries that would exist within `/extern/`. As I do not currently have distribution rights for some of them, I have chosen only to upload the relevant source.
If you'd like to get a copy of the executable, (currently tested/running under 64-bit Windows only) let me know and I'll send something your way! Be warned, it's pretty boring right now.


## What is Eldritch2?
It's a classic hobby 3D game engine project, written in C++. Watch out Unreal! Quips aside, there are some interesting things going on here involving pretty cutting-edge work scheduling and cooperative multitasking.
I can production-test interesting research so your project doesn't have to!

Lots of the architectural decisions here are also centered around shooters and action RPGs, at least out of the box.

## External Dependencies
* [PhysX](http://www.geforce.com/hardware/technology/physx)
* [Wren](https://github.com/munificent/wren)
* [Microprofile](https://github.com/jonasmr/microprofile)
* [Recast Navigation/Detour](https://github.com/recastnavigation/recastnavigation)
* [Steamworks](https://partner.steamgames.com/)
* [EASTL](https://github.com/electronicarts/EASTL)
* [SimpleOpt](https://github.com/brofield/simpleopt)
* [Nuklear](https://github.com/vurtun/nuklear)

## Project Structure
At a high level, project directories are arranged in a hierarchy. Shared or engine-related files live in the `/src/` subdirectory, and the current set of planned tools each have their own special sibling folder named according to the pattern `/src_<Tool Name>/`.
Style conventions dictate that while tools may include files from the main project, tools should not include files from other tools; this is enforced to a limited degree by project include path settings and VS property sheet inheritance mechanisms.

## Program Structure

The `main()` method for the any given project lives in `/<Project Source Directory>/<Project Name>.<Platform>.cpp`.