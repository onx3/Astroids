#  Astroids

"Astroids" is a 2D game engine project inspired by the timeless arcade classic "Asteroids." Built using C++ with SFML for rendering and Box2D for collision simulation, this project emphasizes a custom architecture that revolves around a "GameManager" for centralized control and a flexible "GameObject" system. These systems form the backbone of the engine, enabling modular, reusable components to handle behaviors like rendering, physics, and input. By designing these foundational systems, I gained hands-on experience with game engine architecture, prioritizing scalability and maintainability over shortcuts offered by prebuilt engines like Unity or Unreal. This project reflects my commitment to understanding the core mechanics of game development from the ground up. Below are several screenshots from the game—and yes, the "Astroids" do have muscular arms. A typo turned into a feature, and I decided to embrace the idea of buff, gym-loving space rocks.

## Pictures
![Game Screenshot](Astroids/Art/Astroids.png)
![Explosions](Astroids/Art/AstroidsExplosions.png)
![Game Over](Astroids/Art/GameOver.png)

## Features

- **Rendering**: Utilizes [SFML (Simple and Fast Multimedia Library)](https://www.sfml-dev.org/) for graphics rendering.
- **Physics**: Integrates [Box2D](https://box2d.org/) for realistic physics simulation.
- **Collision Detection**: Implements precise collision detection for the player's ship, projectiles, and asteroids.
- **User Interface**: Basic UI elements for displaying score and lives, developed with SFML and ImGui.
- **Parallaxing**: Adds a dynamic background effect to create the illusion of depth.

## Development Process

The development of this project involved several key steps:

1. **Setting Up the Development Environment**: Configured the project with SFML and Box2D libraries for seamless integration.
2. **Implementing Core Mechanics**: Developed essential game mechanics, including player controls, asteroid generation, and movement systems.
3. **Rendering Graphics**: Utilized SFML to render game objects, manage animations, and handle the dynamic background.
4. **Building a Flexible Architecture**: Designed a component-based system to manage game object behaviors and promote code reuse.
5. **User Interface**: Implemented UI elements to display critical game information such as scores and lives.

## Challenges and Solutions

- **Flexible GameObject Design**  
  - **Challenge**: Managing duplication of logic across multiple object types like Player, Enemy, and Projectile.  
  - **Solution**: Implemented a single `GameObject` class that can represent any entity in the world (e.g., player, asteroid, or projectile) by composing independent components.

- **Decoupled Behaviors**  
  - **Challenge**: Designing a system for handling various behaviors (e.g., rendering, physics, input, health) without creating tightly coupled code.  
  - **Solution**: Adopted a component-based architecture where each `GameObject` contains modular components that can be reused and extended.

- **Debugging Challenges**  
  - **Challenge**: Debugging GameObjects and their components on a frame-by-frame basis was cumbersome.  
  - **Solution**: Integrated ImGui to display debug information, allowing real-time inspection and modification of GameObjects and their components during runtime.

- **Hierarchical Relationships**  
  - **Challenge**: Handling scenarios where a GameObject (e.g., a projectile) should logically be tied to its parent (e.g., the player). Without hierarchy, it was difficult to manage dependencies like destroying child objects when their parent is removed.  
  - **Solution**: Implemented a hierarchical structure where each GameObject can have child objects, managed by a root `GameObject` held by the `GameManager`.

- **Efficient Collision Detection**  
  - **Challenge**: Custom collision detection was slow and inefficient.  
  - **Solution**: Replaced custom collision logic with Box2D’s built-in collision detection system for robustness and performance.

- **Asset Loading Stutters**  
  - **Challenge**: Spawning multiple GameObjects (e.g., 25 enemies from a Bomb drop) caused stuttering due to loading assets directly from disk during gameplay.  
  - **Solution**: Implemented a Resource Manager to preload assets, enabling GameObjects to share textures and reduce runtime overhead.

- **Flat Visuals**  
  - **Challenge**: The game world felt static and lacked depth.  
  - **Solution**: Added parallaxing effects to create a dynamic and visually engaging background.
