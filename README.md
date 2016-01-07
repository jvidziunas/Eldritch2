# Eldritch2 Test Repository

As of right now most of the project is still in a preliminary stage and most every interface should be considered in an extremely volatile/unfinished state.

Additionally, the project is not currently buildable from a direct Git clone; the original makes use of several additional libraries that would exist within `/extern/`. As I do not currently have distribution rights for some of them, I have chosen only to upload the relevant source.
If you'd like to get a copy of the executable, (currently tested/running under 64-bit Windows only) let me know and I'll send something your way! Be warned, it's pretty boring right now.

## External Dependencies
* [Bullet Dynamics Engine] (http://bulletphysics.org/wordpress/), experimental v3, using community multithreading support branch
* [Angelscript](http://www.angelcode.com/angelscript/)
* [Microprofile](https://bitbucket.org/jonasmeyer/microprofile), with Arseny Kapoulkine's extensions
* [Recast Navigation/Detour](https://github.com/memononen/recastnavigation)
* [Boost](http://www.boost.org/), version 1.59
* [Steamworks](https://partner.steamgames.com/)
* [RDESTL](https://github.com/msinilo/rdestl), with customizations/extensions
* [SimpleOpt](https://github.com/brofield/simpleopt)

## Project Structure
At a high level, project directories are arranged in a hierarchy. Shared or engine-related files live in the `/src/` subdirectory, and the current set of planned tools each have their own special sibling folder named according to the pattern `/src_<Tool Name>/`.
Style conventions dictate that while tools may include files from the main project, tools should not include files from other tools; this is enforced to a limited degree by project include path settings and VS property sheet inheritance mechanisms.
Additionally, I'm in the process of having the C++ namespaces reflect the project directory structure, but that's not 100% complete.

## Program Structure

The `main()` method for the any given project lives in `/<Project Source Directory>/<Project Name>.<Platform>.cpp`.