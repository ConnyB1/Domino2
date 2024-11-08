#ifndef PLAYER_H
#define PLAYER_H

#include "raylib.h"
#include "Domino.h"
#include <vector>

class Player {
public:
    Player(std::vector<Domino>& initialHand, int y);
    void DrawHand(Camera2D& camera, Color color);
    void SelectDomino(Domino& domino);
    void DeselectDomino();
    bool HasSelectedDomino() const;
    void UpdateSelectedDominoPosition();
    void SetInitialMousePos(Vector2 pos);
    std::vector<Domino>& GetHand();
    Domino* GetSelectedDomino();
    void RemoveDomino(Domino& domino);
    void EatDomino(std::vector<Domino>& dominoStack);
    bool HasNoDominos() const;
    void UpdateDominoPositions();
private:
    std::vector<Domino> hand;
    int handY;
    Domino* selectedDomino;
    Vector2 selectedDominoOffset;
    Vector2 initialMousePos;
};

#endif
