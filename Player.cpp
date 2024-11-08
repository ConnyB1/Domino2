#include "Player.h"
#include <algorithm>
#include <random>
#include <ctime>
#include "raymath.h"
#include <iostream>

Player::Player(std::vector<Domino>& initialHand, int y) : handY(y), selectedDomino(nullptr) {
    for (int i = 0; i < 7 && i < initialHand.size(); i++) {
        hand.push_back(initialHand[i]);
    }
    UpdateDominoPositions();
}

void Player::DrawHand(Camera2D& camera, Color color) {
    for (auto& domino : hand) {
        domino.Draw(color);
    }
}

void Player::SelectDomino(Domino& domino) {
    selectedDomino = &domino;
    selectedDomino->Select();
    Vector2 mousePos = GetMousePosition();
    selectedDominoOffset = Vector2Subtract(selectedDomino->GetPosition(), mousePos);
}

void Player::DeselectDomino() {
    if (selectedDomino) {
        selectedDomino->Deselect();
        selectedDomino = nullptr;
    }
}

bool Player::HasSelectedDomino() const {
    return selectedDomino != nullptr;
}

void Player::UpdateSelectedDominoPosition() {
    if (selectedDomino) {
        Vector2 mousePos = GetMousePosition();
        Vector2 newPosition = Vector2Add(mousePos, selectedDominoOffset);
        selectedDomino->SetPosition(newPosition);
    }
}

void Player::SetInitialMousePos(Vector2 pos) {
    initialMousePos = pos;
}

std::vector<Domino>& Player::GetHand() {
    return hand;
}

Domino* Player::GetSelectedDomino() {
    return selectedDomino;
}

void Player::RemoveDomino(Domino& domino) {
    auto it = std::find(hand.begin(), hand.end(), domino);
    if (it != hand.end()) {
        hand.erase(it);
        UpdateDominoPositions();
    }
}

void Player::EatDomino(std::vector<Domino>& dominoStack) {
    if (!dominoStack.empty()) {
        Domino newDomino = dominoStack.back();
        dominoStack.pop_back();
        hand.push_back(newDomino);
        UpdateDominoPositions();
    }
}

bool Player::HasNoDominos() const {
    return hand.empty();
}

void Player::UpdateDominoPositions() {
    if (hand.empty()) {
        return;
    }

    float dominoWidth = hand[0].GetWidth();
    float dominoHeight = hand[0].GetHeight();
    float margin = 10.0f;
    int maxDominosPerRow = 11;
    int rowCount = 0;

    float x = 10;
    float y = handY;

    for (size_t i = 0; i < hand.size(); ++i) {
        if (i % maxDominosPerRow == 0 && i != 0) {
            rowCount++;
            x = 10;
        }

        if (handY > 11) {
            y = handY - (rowCount * (dominoHeight + margin));
        }
        else {
            y = handY + (rowCount * (dominoHeight + margin));
        }

        hand[i].SetPosition({ x, y });
        x += dominoWidth + margin;
    }
}