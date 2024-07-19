#include <iostream>
#include <string>
#include <vector>
#include <random>
#include <thread>
#include <mutex>
#include <chrono>
#include <memory>
#include <algorithm>

// Forward declarations
class Room;
class Character;

// Observer Pattern: GameEvent and Observer
class GameEvent {
public:
    virtual ~GameEvent() = default;
    virtual std::string getDescription() const = 0;
};

class Observer {
public:
    virtual ~Observer() = default;
    virtual void onNotify(const GameEvent& event) = 0;
};

// Singleton Pattern: GameWorld
class GameWorld {
public:
    static GameWorld& getInstance() {
        static GameWorld instance;
        return instance;
    }

    void addObserver(Observer* observer) {
        observers.push_back(observer);
    }

    void removeObserver(Observer* observer) {
        observers.erase(std::remove(observers.begin(), observers.end(), observer), observers.end());
    }

    void notifyObservers(const GameEvent& event) {
        for (auto observer : observers) {
            observer->onNotify(event);
        }
    }

    void addRoom(std::shared_ptr<Room> room) {
        rooms.push_back(room);
    }

    std::vector<std::shared_ptr<Room>>& getRooms() {
        return rooms;
    }

private:
    GameWorld() = default;
    std::vector<Observer*> observers;
    std::vector<std::shared_ptr<Room>> rooms;
};

// Factory Pattern: ItemFactory
class Item {
public:
    Item(std::string name, std::string description) : name(name), description(description) {}
    virtual ~Item() = default;
    virtual void use(Character& character) = 0;
    std::string getName() const { return name; }
    std::string getDescription() const { return description; }

protected:
    std::string name;
    std::string description;
};

class ItemFactory {
public:
    virtual std::unique_ptr<Item> createItem() = 0;
};

class HealthPotionFactory : public ItemFactory {
public:
    class HealthPotion : public Item {
    public:
        HealthPotion() : Item("Health Potion", "Restores 20 HP") {}
        void use(Character& character) override;
    };

    std::unique_ptr<Item> createItem() override {
        return std::unique_ptr<Item>(new HealthPotion());
    }
};

// Character classes
class Character {
public:
    Character(std::string name, int health, int damage) 
        : name(name), health(health), maxHealth(health), damage(damage) {}
    virtual ~Character() = default;
    virtual void attack(Character& target) {
        std::cout << name << " attacks " << target.getName() << " for " << damage << " damage!" << std::endl;
        target.takeDamage(damage);
    }
    void takeDamage(int amount) {
        health -= amount;
        if (health < 0) health = 0;
    }
    void heal(int amount) {
        health = std::min(health + amount, maxHealth);
    }
    std::string getName() const { return name; }
    int getHealth() const { return health; }
    bool isAlive() const { return health > 0; }

protected:
    std::string name;
    int health;
    int maxHealth;
    int damage;
};

class Player : public Character, public Observer {
public:
    Player(std::string name) : Character(name, 100, 10) {}
    void addItem(std::unique_ptr<Item> item) {
        inventory.push_back(std::move(item));
        std::cout << "You picked up " << inventory.back()->getName() << "!" << std::endl;
    }
    void showInventory() const {
        std::cout << "Inventory:" << std::endl;
        for (size_t i = 0; i < inventory.size(); ++i) {
            std::cout << i + 1 << ". " << inventory[i]->getName() << ": " << inventory[i]->getDescription() << std::endl;
        }
    }
    void useItem(int index) {
        if (index >= 0 && index < static_cast<int>(inventory.size())) {
            inventory[index]->use(*this);
            inventory.erase(inventory.begin() + index);
        } else {
            std::cout << "Invalid item index." << std::endl;
        }
    }
    void onNotify(const GameEvent& event) override {
        std::cout << "Player " << name << " observed: " << event.getDescription() << std::endl;
    }

private:
    std::vector<std::unique_ptr<Item>> inventory;
};

class Enemy : public Character {
public:
    Enemy(std::string name, int health, int damage) : Character(name, health, damage) {}
};

// Room class
class Room {
public:
    Room(std::string description) : description(description) {}
    virtual void enter(Player& player) = 0;
    std::string getDescription() const { return description; }

protected:
    std::string description;
};

class TreasureRoom : public Room {
public:
    TreasureRoom() : Room("A room filled with treasure!") {}
    void enter(Player& player) override {
        std::cout << "You enter " << getDescription() << std::endl;
        player.addItem(HealthPotionFactory().createItem());
    }
};

class MonsterRoom : public Room {
public:
    MonsterRoom() : Room("A dark room with a lurking monster.") {}
    void enter(Player& player) override {
        std::cout << "You enter " << getDescription() << std::endl;
        Enemy goblin("Goblin", 30, 5);
        while (player.isAlive() && goblin.isAlive()) {
            player.attack(goblin);
            if (goblin.isAlive()) goblin.attack(player);
        }
        if (player.isAlive()) {
            std::cout << "You defeated the " << goblin.getName() << "!" << std::endl;
        }
    }
};

// Event classes
class RandomEvent : public GameEvent {
public:
    std::string getDescription() const override {
        return "A random event occurred in the game world!";
    }
};

// Function to generate random events
void randomEventGenerator() {
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(1, 10);

    while (true) {
        std::this_thread::sleep_for(std::chrono::seconds(5));
        if (dis(gen) == 1) {
            GameWorld::getInstance().notifyObservers(RandomEvent());
        }
    }
}

// Implementation of HealthPotion::use
void HealthPotionFactory::HealthPotion::use(Character& character) {
    std::cout << character.getName() << " uses a Health Potion and restores 20 HP!" << std::endl;
    character.heal(20);
}

int main() {
    std::cout << "Welcome to the Advanced C++ Text Adventure!" << std::endl;
    std::cout << "What's your name, adventurer? ";
    std::string playerName;
    std::getline(std::cin, playerName);

    Player player(playerName);
    GameWorld::getInstance().addObserver(&player);

    // Create rooms
    GameWorld::getInstance().addRoom(std::make_shared<TreasureRoom>());
    GameWorld::getInstance().addRoom(std::make_shared<MonsterRoom>());

    // Start random event generator thread
    std::thread eventThread(randomEventGenerator);
    eventThread.detach();

    std::cout << "Welcome, " << player.getName() << "! Your advanced adventure begins..." << std::endl;

    while (player.isAlive()) {
        std::cout << "\nWhat would you like to do?" << std::endl;
        std::cout << "1. Explore the next room" << std::endl;
        std::cout << "2. Check inventory" << std::endl;
        std::cout << "3. Use item" << std::endl;
        std::cout << "4. Quit" << std::endl;

        int choice;
        std::cin >> choice;

        switch (choice) {
            case 1: {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(0, GameWorld::getInstance().getRooms().size() - 1);
                GameWorld::getInstance().getRooms()[dis(gen)]->enter(player);
                break;
            }
            case 2:
                player.showInventory();
                break;
            case 3: {
                player.showInventory();
                std::cout << "Enter the item number to use (or 0 to cancel): ";
                int itemIndex;
                std::cin >> itemIndex;
                if (itemIndex > 0) {
                    player.useItem(itemIndex - 1);
                }
                break;
            }
            case 4:
                std::cout << "Thanks for playing!" << std::endl;
                return 0;
            default:
                std::cout << "Invalid choice. Try again." << std::endl;
        }

        std::cout << "Your health: " << player.getHealth() << std::endl;
    }

    std::cout << "Game Over! You died." << std::endl;
    return 0;
}