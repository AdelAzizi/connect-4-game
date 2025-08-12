#include "Player.h"

// Constructor
Player::Player(const std::string& name, char id, const sf::Color& color, bool isComputer)
    : name(name), id(id), score(0), color(color), computerFlag(isComputer) {}

// --- Getters ---
std::string Player::getName() const {
    return name;
}

char Player::getId() const {
    return id;
}

int Player::getScore() const {
    return score;
}

sf::Color Player::getColor() const {
    return color;
}

bool Player::isComputer() const {
    return computerFlag;
}

// --- Setters ---
void Player::setName(const std::string& newName) {
    name = newName;
}

void Player::setScore(int newScore) {
    score = newScore;
}
