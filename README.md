# Snake Game in C++

## Project Overview
This project is a modern implementation of the classic Snake game developed in **C++** as a part of our college assignment. The game showcases advanced use of **linked lists**, **dynamic memory management**, and platform-independent console handling techniques.

## Authors
- Karan Shukla (202401479)
- Digvijay Parmar (202401444)
- Shlok Gohel (202401476)
- Kush Patel (202401448)

  ---
  
### Key Features
- **Smooth Gameplay:** The snake moves smoothly with dynamic growth upon consuming food.
- **Special Food:** Occasionally, special food appears, granting higher points.
- **Difficulty Levels:** Choose from three modes: Easy, Medium, and Hard.
- **Collision Handling:** Game ends when the snake collides with the wall or itself.
- **High Score Tracking:** Track your highest score throughout the session.
- **Platform Support:** Works on both **Windows** and **Unix-based** systems.

---

## How to Play
- **Controls:**
  - **W:** Move Up
  - **S:** Move Down
  - **A:** Move Left
  - **D:** Move Right
- **Objective:** Guide the snake to consume food (@) and special food ($) to earn points and grow longer.
- **Game Over Conditions:**
  - Collision with the game boundary.
  - Collision with the snake's body.

### Scoring
- Regular Food: **10 points**
- Special Food: **50 points**

### Difficulty Levels
- **Easy:** Slow snake speed.
- **Medium:** Moderate snake speed.
- **Hard:** Fast snake speed.

---

## Installation and Setup
### Windows
1. Ensure that **MinGW** or any compatible C++ compiler is installed.
2. Compile and run the game:
   ```bash
   g++ snake_game.cpp -o snake_game.exe
   .\snake_game.exe
   ```

### Unix-based Systems (Linux/Mac)
1. Open the terminal.
2. Compile and run the game:
   ```bash
   g++ snake_game.cpp -o snake_game
   ./snake_game
   ```

---

## Project Structure
- `Node`: Represents a segment of the snake.
- `Snake`: Manages the snake's movement, growth, and collision handling.
- `Game`: Handles game logic, rendering, and player input.
- `main()`: Entry point for the game.

---

## Technical Highlights
- **Dynamic Linked List:** The snake is implemented using a linked list where each node represents a segment.
- **Platform-Independent Console Handling:** Utilizes platform-specific functions to handle keyboard inputs and screen clearing.
- **Randomized Special Food Generation:** The special food is placed at random intervals and positions.
- **Memory Management:** Proper dynamic memory allocation and deallocation ensure efficient resource usage.

---

## Future Enhancements
- Add **obstacles** to increase gameplay difficulty.
- Implement **sound effects** for events.
- Create a **graphical version** using libraries like **SFML**.
- Introduce **multiplayer mode**.

---

## Acknowledgments
We express our gratitude to our professor and peers for their valuable guidance and support throughout this project.

---
