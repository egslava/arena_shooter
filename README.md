# arena_shooter
[![https://www.youtube.com/watch?v=cfY2wrKqNZE](README.gif)](https://www.youtube.com/watch?v=cfY2wrKqNZE)

(Click to watch on YouTube)

Feel free to steal some code parts (CMake? :) )

The game was done in OpenGL 4.0 by me from complete scratch within 2 months as a part of gaining my C++ skills. I had to do everything from scratch (that was a requirements for this assessment). Things are implemented here:
1. The model format, the converter and the loader.
2. The resource pipeline, built with CMake. It compiles resources (models and images) to an internal format incrementally, taking into an account the file hierarchy of an arbitrary deepness . Thus, it doesn't recompile the same things again and again, causing slow buildings.
3. Math problems: collisions, jumps, navigation.
4. GPU particle system.
5. Bots, that can navigate throw the whole level.
6. Simple dynamic lighting (that's under the character).
7. It's cross-platform and tested under Windows 8 and Ubuntu 18.04.
8. Level "Design"  :)

Libraries and tools used:
1. SDL 2 (cross-platform graphics initialization, work with input)
2. CMake (for the resource pipeline and building everything up)
3. Standard C++ library (shared_ptr and vector only)
4. Python (the model converter is written in Python 3)
5. PVRTexToolCLI for converting images in the resource pipeline.

In `docs-src` you can see docs in [Sphinx](https://www.sphinx-doc.org/en/master/) format 
