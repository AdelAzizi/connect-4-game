#pragma once

#include <string>
#include <SFML/Graphics.hpp>

class Player {
private:
    std::string name;
    char id;
    int score;
    sf::Color color;
    bool computerFlag;

public:
    // Constructor
    Player(const std::string& name = "", char id = ' ', const sf::Color& color = sf::Color::White, bool isComputer = false);

    // Getters
    std::string getName() const;
    char getId() const;
    int getScore() const;
    sf::Color getColor() const;
    bool isComputer() const;

    // Setters
    void setName(const std::string& newName);
    void setScore(int newScore);
};
