# Physics Engine

## Overview

The **Physics Engine** is a simplified 2D physics engine designed for personal use and learning. It allows users to simulate basic physics interactions in 2D environments, focusing on ease of use and simplicity. The engine provides essential features for simulating rigid body dynamics and basic collision detection.

This project is intended to serve as a learning tool and as a means of experimenting with physics simulations in a straightforward, easy-to-understand manner. The engine works in tandem with a custom **SFML wrapper** to handle the graphical rendering of objects in the simulation, making it easy to visualize the physical behavior of objects on the screen.

## Purpose

The main goal of the **Physics Engine** is to provide a basic, accessible framework for simulating 2D physics. It was designed with simplicity in mind, making it ideal for personal projects, experiments, and learning about core physics principles like motion, collision, and force.

Rather than offering a complex or feature-rich engine, this project focuses on the fundamental aspects of physics simulation, providing a straightforward interface for anyone interested in exploring basic 2D physics.

## Project Structure

The project is organized into several directories, with core components split between the **Flat** (SFML wrapper) and **FlatPhysics** (physics engine) modules:

```
.
├── Flat
│   ├── FlatColor.hpp
│   ├── FlatKeyboard.hpp
│   ├── FlatMouse.hpp
│   ├── FlatShapes.hpp
│   ├── FlatWindow.hpp
│   └── core.hpp
├── FlatPhysics
│   ├── core.hpp
│   ├── headers
│   │   ├── FlatBody.hpp
│   │   ├── FlatCollisions.hpp
│   │   ├── FlatConverter.hpp
│   │   ├── FlatMath.hpp
│   │   ├── FlatTransformation.hpp
│   │   ├── FlatVector.hpp
│   │   └── FlatWorld.hpp
│   └── src
│       ├── FlatBody.cpp
│       ├── FlatCollisions.cpp
│       ├── FlatConverter.cpp
│       ├── FlatMath.cpp
│       ├── FlatTransformation.cpp
│       ├── FlatVector.cpp
│       └── FlatWorld.cpp
├── FlatUtils
│   ├── Random.hpp
│   └── StopWatch.hpp
├── FlatTester.cpp
└── README.md
```

- **Flat**: Contains the **SFML wrapper** components to simplify the creation of windows, handling of user input, and drawing of shapes. This provides a simple interface for visualizing the physics engine.
- **FlatPhysics**: The core physics engine logic, which includes the components for simulating physics bodies, handling collisions, and integrating forces over time.
- **FlatTester.cpp**: A simple test program where the physics engine is integrated with the SFML wrapper to visualize and test the behavior of simulated bodies.

## Core Features

- **Rigid Body Simulation**: Provides a basic 2D simulation of rigid bodies such as circles and boxes.
- **Collision Detection**: Handles simple collisions between physics bodies.
- **Movement**: Simulates velocity and force-based movement.
- **Simple Integration**: The SFML wrapper makes it easy to visualize physics simulations by providing simple methods for drawing shapes and handling basic user input (e.g., keyboard and mouse events).

## Conclusion

The **Physics Engine** is a minimalistic 2D physics engine that serves as a personal learning tool for experimenting with basic physics simulations. It is not designed to be a production-grade engine, but rather a simple and accessible way to simulate the core concepts of physics in 2D environments. The engine is designed to be easy to understand, making it an ideal starting point for anyone interested in learning about physics engines or creating small-scale simulations and games.