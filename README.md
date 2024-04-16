# maya-find-collision

Example of a Maya 2022 `Python` script (using OpenMaya 2.0) being adapted to work as a `C++` plug-in.

The C++ plug-in is made to be built on "Visual Studio 15 2017".

To build it:
1. ```cmd
	cd cpp\findCollision
	```
2. ```cmd
	cmake -H. -Bbuild -G "Visual Studio 15 2017"
	```
3. ```cmd
	cmake --build build
	```

If all goes well and the build returns a 0 status, a `findCollision.mll` plug-in file will be built.

To run the plug-in:
1. Load the `findCollision.mll` file.
2. Select a `mesh`.
3. Shift + select a `locator` (or any other transform object). The plug-in will check if & where the `locator` collides with the `mesh`.
4. Run the `FindCollision` MEL command in Maya.
   * If there is a collision, a new `locator` called "intersection_point" will be created at the point of collision.
   * If there is no collision, a message will be printed in the `Output Window`.
