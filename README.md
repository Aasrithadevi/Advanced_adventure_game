# Advanced_adventure_game
- OOPS and Multithreading
- Description:
Welcome to the Advanced C++ Text Adventure Game! This project is a text-based adventure game where players explore different rooms, encounter enemies, collect items, and experience random events. The game leverages various advanced C++ concepts and design patterns to create a rich and interactive gameplay experience.

## Key Features
- Exploration: Players can explore different types of rooms, such as treasure rooms and monster rooms.
- Combat: Engage in combat with enemies like goblins.
- Inventory Management: Collect and use items, such as health potions.
- Random Events: Experience random events that occur throughout the game.
- C++ Concepts and Design Patterns Used
1. Singleton Pattern
The GameWorld class is implemented as a singleton to ensure there is only one instance of the game world throughout the application. This pattern is used to manage the game state, rooms, and observers.

2. Observer Pattern
The game uses the observer pattern to notify players of random events. The Observer class and the GameEvent class facilitate this pattern.

3. Factory Pattern
The factory pattern is used to create items dynamically. The ItemFactory class and its derived classes (HealthPotionFactory) are used to generate different types of items.

5. Inheritance and Polymorphism
The game uses inheritance and polymorphism to manage different types of characters (Character, Player, Enemy) and rooms (Room, TreasureRoom, MonsterRoom).

5. Multithreading
The game uses multithreading to generate random events at regular intervals, enhancing the dynamic nature of the game.

6. Smart Pointers
Smart pointers (std::unique_ptr and std::shared_ptr) are used for memory management to ensure proper handling and deletion of dynamically allocated objects.

7. Random Number Generation
The game uses random number generation to decide which room the player will enter next and to generate random events.

- How to Play
Clone the repository

Compile the game:

g++ -std=c++17 main.cpp -o TextAdventureGame
./TextAdventureGame


Game Flow
Explore the next room: Move to a randomly selected room (treasure room or monster room).
Check inventory: View the items you have collected.
Use item: Use an item from your inventory to aid in your adventure.
Quit: Exit the game.
