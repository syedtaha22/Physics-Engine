# Physics Engine

## Overview

The **Physics Engine** is a simplified 2D physics engine designed for personal use and learning. It allows users to simulate basic physics interactions in 2D environments, focusing on ease of use and simplicity. The engine provides essential features for simulating rigid body dynamics and basic collision detection.

This project is intended to serve as a learning tool and as a means of experimenting with physics simulations in a straightforward, easy-to-understand manner. The engine works in tandem with a custom **SFML wrapper** to handle the graphical rendering of objects in the simulation, making it easy to visualize the physical behavior of objects on the screen.

## Purpose

The main goal of the **Physics Engine** is to provide a basic, accessible framework for simulating 2D physics. It was designed with simplicity in mind, making it ideal for personal projects, experiments, and learning about core physics principles like motion, collision, and force.

Rather than offering a complex or feature-rich engine, this project focuses on the fundamental aspects of physics simulation, providing a straightforward interface for anyone interested in exploring basic 2D physics.

## Project Structure
The project is organized into several modules that encapsulate specific functionalities, divided mainly into **Flat**, **FlatPhysics**, and **FlatUtils**. Each module contains relevant headers and source files for its respective purpose.


### **Flat *(namespace)***
Contains code to handle window management, input, and basic graphics using SFML. It serves as a wrapper around SFML to simplify its usage within the project.
  - `FlatKeyboard`: Handles keyboard input events and states.
  - `FlatMouse`: Deals with mouse input, including position and button state.
  - `FlatWindow`: Manages the SFML window, providing methods for window operations like opening, closing, and handling events. Also provides methods for drawing shapes on to the window.
  <br>
  - `FlatColor`: Represents a color in RGBA format and provides functions to create and manage colors.
  - `FlatShapes`: Contains structures for basic shapes like circles and rectangles.

### **FlatPhysics *(namespace)*** 
Contains the physics engine, focusing on collision detection, vector math, bodies, and transformations. All components are encapsulated within the `FlatPhysics` namespace. 

  - `FlatBody`: Defines physical bodies (e.g., shapes or objects) in the simulation.
  - `FlatCollisions`: Handles collision detection and resolution between bodies.
  - `FlatConverter`: Contains functions to convert between `FlatVector` and SFML vector types.
  - `FlatMath`: Provides functions to operate on vectors, such as dot product, cross product, and normalization.
  - `FlatWorld`: The main simulation world, containing bodies and handling physics updates.
  <br>
  - `FlatAABB`: Represents an Axis-Aligned Bounding Box (AABB) for objects
  - `FlatManifold`: Represents the contact points and data related to collisions between bodies.
  - `FlatTransformation`: Implements transformations such as translation, rotation, and scaling for physical bodies.
  - `FlatVector`: Defines a 2D vector with x and y components and provides overloaded operators for vector operations.

### **FlatUtils *(namespace)***
Contains utility classes and functions that provide general-purpose functionality used across the project.

  - `Random`: Provides utilities for generating random numbers, boolean values, and colors.
  - `StopWatch`: Implements a stopwatch/timer class to measure elapsed time during simulations or processes.

### **FlatTester.cpp**
A test file or entry point for testing and validating the components of the project.

## Core Features

- **Rigid Body Simulation**: Provides a basic 2D simulation of rigid bodies such as circles and boxes.
- **Collision Detection**: Handles simple collisions between physics bodies.
- **Movement**: Simulates velocity and force-based movement.
- **Simple Integration**: The SFML wrapper makes it easy to visualize physics simulations by providing simple methods for drawing shapes and handling basic user input (e.g., keyboard and mouse events).

## Conclusion

The **Physics Engine** is a minimalistic 2D physics engine that serves as a personal learning tool for experimenting with basic physics simulations. It is not designed to be a production-grade engine, but rather a simple and accessible way to simulate the core concepts of physics in 2D environments. The engine is designed to be easy to understand, making it an ideal starting point for anyone interested in learning about physics engines or creating small-scale simulations and games.