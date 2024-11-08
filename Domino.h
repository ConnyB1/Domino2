#pragma once

#include "raylib.h"

class Domino {
public:
    Domino(int side1, int side2, Vector2 position);

    void Draw(Color color) const;
    bool IsMouseOver(Vector2 mousePos) const;
    bool IsMouseOver() const;
    void Select();
    void Deselect();
    bool IsSelected() const;
    bool IsPlaced() const;
    void Place();
    Vector2 GetPosition() const;
    void SetPosition(Vector2 position);
    int GetSide1() const;
    int GetSide2() const;
    bool operator==(const Domino& other) const;
    void Rotate();

    float GetWidth() const;
    float GetHeight() const;
    int GetOrientation() const;

private:
    int side1;
    int side2;
    Vector2 position;
    bool selected = false;
    bool placed = false;
    int orientation; 
    float width;
    float height;
};
