# Simple2D Extended Edition

A simple game ""engine"" build based on my original Simple2D engine.

Changes made to Simple2D:
* Programs with gameobjects now staticaly link to Simple2D.lib instead of each gameobject being compiled into a shared library and then being loaded.
* Due to changes in loading, game object which are going to be created dynamically are loaded from a prefab xml file.
* Added Support for proper text rendering !!!!

## Dependencies
SDL2, GLEW, GLU, OpenGL, stbi, FreeType, rapidxml
