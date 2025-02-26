# Physics Engine

This repository contains a physics engine designed to simulate various physical phenomena. The `Simulation` class is a core component of the engine, allowing users to define custom simulations by inheriting from it and overriding specific virtual methods.

## Overridable Functions

When creating a custom simulation, users need to override the following functions:

1. **`protected virtual double get_max_distance() = 0;`**  
   Returns the distance of the furthest object from the origin, used to set the simulation scale.

2. **`public virtual void step() = 0;`**  
   Defines the logic for advancing the simulation state in each frame.

3. **`public virtual void draw_bodies() = 0;`**  
   Specifies how bodies or objects are rendered on the screen.

4. **`public virtual void draw();`** *(Optional override)*  
   Controls how the entire scene, including objects, orbits, and scale indicators, is drawn.

By implementing these functions, users can customize the simulation behavior while utilizing the engine's core functionalities for rendering, physics calculations, and user interactions.

