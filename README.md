# c_sdl3_game_engine

A 3D game engine written from scratch in C, built on SDL3 and OpenGL 3.3 Core. Entities are composed from components, and scenes, prefabs and materials are defined in JSON, so simulation parameters can be changed without recompiling.

The engine currently runs a rigid body simulation with sphere-sphere and sphere-AABB collision response, textured meshes and a material system.

![simulation](gifs/simulation.gif)

![sphere-AABB collisions](gifs/AABB_SPHERE_COLLISIONS.gif)
![sphere-sphere collisions](gifs/sphere_sphere_colliisons.gif)

## Features

**Component architecture.** A `GameObject` is a container for components with an `awake` / `start` / `update` / `destroy` lifecycle. Component types register themselves in a registry keyed by string id, so JSON deserialisation resolves types through a lookup rather than a hardcoded switch.

**Rigid body physics.** Force accumulation, gravity, per-body mass and kinematic bodies. Collision detection covers sphere-sphere and sphere-AABB pairs, producing a contact normal and penetration depth; the response derives restitution and friction from the physics material assigned to each body.

**Broad phase.** A uniform spatial grid partitions the world so narrow-phase tests run only on candidate pairs rather than every object pair.

**Rendering.** OpenGL 3.3 Core with meshes and materials loaded from JSON, diffuse, specular and normal maps, per-fragment lighting, and a debug renderer for visualising collision volumes.

**Asset pipeline.** Prefabs, scenes, materials and physics materials are separate JSON files. An id-to-path index is regenerated on every build, and resources are loaded by id rather than by path.

**Input.** Gamepad support through SDL3, covering buttons, axes and deadzone handling.

## Current limitations

`soft_body_component` and `spring_component` are present in the tree but the soft body simulation is unfinished. There is no keyboard input — the camera is driven by the right analogue stick. Audio and UI are not implemented.

## Built with

[SDL3](https://www.libsdl.org/) for windowing, GL context and gamepad input, [glad](https://glad.dav1d.de/) as the OpenGL loader, [cglm](https://github.com/recp/cglm) for math, [cJSON](https://github.com/DaveGamble/cJSON) for parsing and [stb_image](https://github.com/nothings/stb) for texture loading. Everything except SDL3 is vendored in the repository.
