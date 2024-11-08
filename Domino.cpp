#include "Domino.h"
#include <algorithm>
#include <iostream>
Domino::Domino(int side1, int side2, Vector2 position)
    : side1(side1), side2(side2), position(position), selected(false), placed(false), orientation(0), width(60), height(30) {}

void Domino::Draw(Color color) const {
    Color drawColor = placed ? DARKGRAY : (selected ? RED : color);
    float w = (orientation == 0) ? width : height;
    float h = (orientation == 0) ? height : width;
    DrawRectangle(position.x, position.y, w, h, drawColor);
    DrawText(TextFormat("%d", side1), position.x + 5, position.y + 5, 20, WHITE);
    DrawText(TextFormat("%d", side2), position.x + w - 25, position.y + 5, 20, WHITE);
}

bool Domino::IsMouseOver() const {
    Vector2 mousePos = GetMousePosition();
    float dominoWidth = 60.0f; 
    float dominoHeight = 30.0f; 
    return (mousePos.x >= position.x && mousePos.x <= position.x + dominoWidth &&
        mousePos.y >= position.y && mousePos.y <= position.y + dominoHeight);
}
void Domino::Select() {
    selected = true;
}

void Domino::Deselect() {
    selected = false;
}

bool Domino::IsSelected() const {
    return selected;
}

bool Domino::IsPlaced() const {
    return placed;
}

void Domino::Place() {
    placed = true;
}

Vector2 Domino::GetPosition() const {
    return position;
}

void Domino::SetPosition(Vector2 position) {
    this->position = position;
}

int Domino::GetSide1() const {
    return side1;
}

int Domino::GetSide2() const {
    return side2;
}

bool Domino::operator==(const Domino& other) const {
    return side1 == other.side1 && side2 == other.side2 && position.x == other.position.x && position.y == other.position.y;
}

void Domino::Rotate() {
    std::swap(side1, side2);  
    orientation = (orientation == 0) ? 90 : 0;  
    std::swap(width, height); 
}

float Domino::GetWidth() const {
    return width;
}

float Domino::GetHeight() const {
    return height;
}

int Domino::GetOrientation() const {
    return orientation;
}
