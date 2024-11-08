#define _CRT_SECURE_NO_WARNINGS
#include "raylib.h"
#include "raymath.h"
#include "Domino.h"
#include <vector>
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>
#include "Player.h"
#include <string>
#include <iostream>

bool gameOver = false, jugando = false, play = false;
bool firstDominoPlaced = false;

bool IsAdjacent(const Domino& placed, const Vector2& newPosition) {
    float dx = fabs(placed.GetPosition().x - newPosition.x);
    float dy = fabs(placed.GetPosition().y - newPosition.y);

    float placedWidth = (placed.GetOrientation() == 0) ? placed.GetWidth() : placed.GetHeight();
    float placedHeight = (placed.GetOrientation() == 0) ? placed.GetHeight() : placed.GetWidth();

    return (dx == placedWidth && dy == 0) || (dx == 0 && dy == placedHeight);
}

bool CanPlaceDomino(const Domino& placed, const Domino& newDomino, const Vector2& newPosition) {
    float dx = fabs(placed.GetPosition().x - newPosition.x);
    float dy = fabs(placed.GetPosition().y - newPosition.y);

    float placedWidth = (placed.GetOrientation() == 0) ? placed.GetWidth() : placed.GetHeight();
    float placedHeight = (placed.GetOrientation() == 0) ? placed.GetHeight() : placed.GetWidth();
    float newDominoWidth = (newDomino.GetOrientation() == 0) ? newDomino.GetWidth() : newDomino.GetHeight();
    float newDominoHeight = (newDomino.GetOrientation() == 0) ? newDomino.GetHeight() : newDomino.GetWidth();

    if ((dx == placedWidth && dy == 0) || (dx == 0 && dy == placedHeight)) {
        if (dx == placedWidth) {
            if (placed.GetPosition().x < newPosition.x) {
                return placed.GetSide2() == newDomino.GetSide1();
            }
            else {
                return placed.GetSide1() == newDomino.GetSide2();
            }
        }
        else {
            if (placed.GetPosition().y < newPosition.y) {
                return placed.GetSide2() == newDomino.GetSide1();
            }
            else {
                return placed.GetSide1() == newDomino.GetSide2();
            }
        }
    }
    return false;
}

static bool CheckCollisionRecs(Rectangle rec1, Rectangle rec2) {
    return (rec1.x < rec2.x + rec2.width && rec1.x + rec1.width > rec2.x && rec1.y < rec2.y + rec2.height && rec1.y + rec1.height > rec2.y);
}


int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Domino Game");

    Camera2D cam = { 0 };
    cam.zoom = 1;
    cam.offset.x = GetScreenWidth() / 2.0f;
    cam.offset.y = GetScreenHeight() / 2.0f;

    Vector2 prevMousePos = GetMousePosition();

    

    
    

    Image menu = LoadImage("assets/menu.png");
    ImageResize(&menu, screenWidth, screenHeight);
    Texture2D menuTexture = LoadTextureFromImage(menu);
    UnloadImage(menu);

    Image fondo = LoadImage("assets/fondo.png");
    ImageResize(&fondo, screenWidth * 3, screenHeight * 3);
    Texture2D fondoTexture = LoadTextureFromImage(fondo);
    UnloadImage(fondo);

    Image boton = LoadImage("assets/play.png");
    ImageResize(&boton, 200, 200);
    Texture2D botonTexture = LoadTextureFromImage(boton);
    UnloadImage(boton);

    Image boton2 = LoadImage("assets/EXIT.png");
    ImageResize(&boton2, 200, 200);
    Texture2D botonTexture2 = LoadTextureFromImage(boton2);
    UnloadImage(boton2);

    Image intru = LoadImage("assets/intru.png");
    ImageResize(&intru, screenWidth, screenHeight);
    Texture2D intruTexture = LoadTextureFromImage(intru);
    UnloadImage(intru);

    InitAudioDevice();
    Sound intro = LoadSound("assets/Audios/Musica/intro.mp3");
    Sound Button = LoadSound("assets/buttonfx.wav");
    Sound ganar = LoadSound("assets/ganar.wav");
    Sound musica = LoadSound("assets/intro.mp3");
    Sound gameover = LoadSound("assets/gameover.mp3");
    Sound poner = LoadSound("assets/eating.mp3");
    Sound girar = LoadSound("assets/girar.mp3");
    Sound place = LoadSound("assets/place.wav");
    Sound click = LoadSound("assets/click.wav");
    Sound tuto = LoadSound("assets/tutorial.wav");

    SetSoundVolume(musica, .1);
    SetSoundVolume(gameover, .3);
    SetSoundVolume(intro, .3);
    SetSoundVolume(poner, .3);
    SetSoundVolume(girar, .3);
    SetSoundVolume(click, .2);
    SetSoundVolume(tuto, .2);

    float frameHeight = (float)boton.height / 3;
    float frameHeight2 = (float)boton2.height / 3;

    int empate = 0;
    Rectangle sourceRec = { 0, 0, (float)boton.width, frameHeight };
    Rectangle sourceRec2 = { 0, 0, (float)boton2.width, frameHeight2 };


    Rectangle btnBounds = { (screenWidth / 2.0f) - (boton.width / 2.0f), (screenHeight / 3.0f) - (boton.height / 3 / 2.0f), (float)boton.width, frameHeight };
    Rectangle btnBounds2 = { (screenWidth / 2.0f) - (boton2.width / 2.0f), (screenHeight / 2.0f) - (boton2.height / 3 / 4.0f), (float)boton2.width, frameHeight2 };
    
    int btnState = 0, btnState2 = 0;
    bool btnAction = false, btnAction2 = false, instru = true;
    float size = 5000;
    Color transparentGray = Fade(GRAY, 0.05f);

    Vector2 mousePoint = { 0.0f, 0.0f };

    
    SetTargetFPS(60); 

    while (!WindowShouldClose()) 
    {
        
        play = false;

        std::vector<Domino> dominoStack;
        for (int i = 0; i <= 6; i++) {
            for (int j = i; j <= 6; j++) {
                dominoStack.push_back(Domino(i, j, { screenWidth / 2 - 30, screenHeight / 2 - 60 }));
            }
        }


        std::srand(unsigned(std::time(0)));
        std::random_shuffle(dominoStack.begin(), dominoStack.end());
        std::vector<Domino> player1Hand(dominoStack.begin(), dominoStack.begin() + 14);
        std::vector<Domino> player2Hand(dominoStack.begin() + 14, dominoStack.begin() + 28);
        std::vector<Domino> remainingDominoStack(dominoStack.begin() + 28, dominoStack.end());
        std::vector<Domino> placedDominos;
        Player player1(player1Hand, screenHeight - 40); // Jugador 1, parte inferior
        Player player2(player2Hand, 10); // Jugador 2, parte superior


        Player* currentPlayer = &player1;
        while (!play)
        {
            if (!IsSoundPlaying(intro)) { PlaySound(intro); }
            mousePoint = GetMousePosition();
            btnAction = false;

            BeginDrawing();
            DrawTexture(menuTexture, 0, 0, WHITE);
            DrawTextureRec(botonTexture, sourceRec, { btnBounds.x, btnBounds.y }, WHITE);
            DrawTextureRec(botonTexture2, sourceRec2, { btnBounds2.x, btnBounds2.y }, WHITE);
            EndDrawing();
            if (CheckCollisionPointRec(mousePoint, btnBounds))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState = 2;
                else btnState = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction = true;
            }
            else btnState = 0;

            sourceRec.y = btnState * frameHeight;

            if (btnAction)
            {
                PlaySound(Button);
                play = true;
                StopSound(intro);
            }
            ///otro boton
            if (CheckCollisionPointRec(mousePoint, btnBounds2))
            {
                if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) btnState2 = 2;
                else btnState2 = 1;

                if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) btnAction2 = true;
            }
            else btnState2 = 0;
            sourceRec2.y = btnState2 * frameHeight;

            if (btnAction2)
            {
                PlaySound(Button);
                play = false;
                StopSound(intro);
                CloseWindow();
                return 0;
            }
        }
        while (instru)
        {
            if (!IsSoundPlaying(tuto)) { PlaySound(tuto); }
            BeginDrawing();
            DrawTexture(intruTexture, 0, 0, WHITE);
            EndDrawing();

            // Verificar si se presiona cualquier tecla
            if (IsKeyPressed(KEY_SPACE))
            {
                instru = false;
                StopSound(tuto);
                break;
            }
        }
        while (play) {
            if (!IsSoundPlaying(musica)) { PlaySound(musica); }
            float mouseDelta = GetMouseWheelMove();

            float newZoom = cam.zoom + mouseDelta * 0.01f;
            if (newZoom > 1.20f) {
                newZoom = 1.20f;
            }
            else if (newZoom < 0.82f) {
                newZoom = 0.82f;
            }

            cam.zoom = newZoom;

            Vector2 thisPos = GetMousePosition();
            Vector2 delta = Vector2Subtract(prevMousePos, thisPos);
            prevMousePos = thisPos;

            if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
                PlaySound(click);
                for (auto& domino : currentPlayer->GetHand()) {
                    if (domino.IsMouseOver() && !domino.IsSelected() && !domino.IsPlaced()) {
                        currentPlayer->SelectDomino(domino);
                        currentPlayer->SetInitialMousePos(GetMousePosition());
                        break;
                    }
                }
            }

            if (IsMouseButtonDown(MOUSE_LEFT_BUTTON) && currentPlayer->HasSelectedDomino()) {
                currentPlayer->UpdateSelectedDominoPosition();
            }

            if (IsMouseButtonReleased(MOUSE_LEFT_BUTTON) && currentPlayer->HasSelectedDomino()) {
                Vector2 position = currentPlayer->GetSelectedDomino()->GetPosition();
                position = GetScreenToWorld2D(position, cam);
                position.x = floorf(position.x / 10) * 10.0f;
                position.y = floorf(position.y / 10) * 10.0f;
                bool canPlace = placedDominos.empty();
                for (const auto& domino : placedDominos) {
                    if (CheckCollisionRecs({ position.x, position.y, currentPlayer->GetSelectedDomino()->GetWidth(), currentPlayer->GetSelectedDomino()->GetHeight() }, { domino.GetPosition().x, domino.GetPosition().y, domino.GetWidth(), domino.GetHeight() })) {
                        canPlace = false;
                        break;
                    }
                    if (IsAdjacent(domino, position) && CanPlaceDomino(domino, *currentPlayer->GetSelectedDomino(), position)) {
                        canPlace = true;
                        break;
                    }
                }

                if (canPlace) {
                    PlaySound(place);
                    currentPlayer->GetSelectedDomino()->SetPosition(position);
                    currentPlayer->GetSelectedDomino()->Place();
                    placedDominos.push_back(*currentPlayer->GetSelectedDomino());
                    currentPlayer->RemoveDomino(*currentPlayer->GetSelectedDomino());
                    if (currentPlayer->HasNoDominos()) {
                        std::string winnerText = (currentPlayer == &player1) ? "Player 1 Wins!!!" : "Player 2 Wins!!!";
                        PlaySound(ganar);
                        for (int i = 0; i < 300; i++) {
                            BeginDrawing();
                            ClearBackground(RAYWHITE);
                            DrawTexture(menuTexture, 0, 0, WHITE);
                            DrawText(winnerText.c_str(), screenWidth / 2 - MeasureText(winnerText.c_str(), 20) / 2, screenHeight / 2, 20, WHITE);
                            EndDrawing();
                        }
                        play = false;
                        break; 
                    }
                    empate = 0;
                    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
                }
                else {
                    currentPlayer->GetSelectedDomino()->Deselect();
                }
                currentPlayer->DeselectDomino();
            }

            if (IsMouseButtonPressed(MOUSE_RIGHT_BUTTON)) {
                if (currentPlayer->HasSelectedDomino()) {
                    currentPlayer->DeselectDomino();
                }
            }

            if (IsMouseButtonDown(MOUSE_RIGHT_BUTTON)) {
                cam.target = GetScreenToWorld2D(Vector2Add(cam.offset, delta), cam);
            }

            if (IsKeyPressed(KEY_E)) {
                PlaySound(poner);
                currentPlayer->EatDomino(dominoStack);
                
                if (dominoStack.empty()) {
                    empate++;
                    currentPlayer = (currentPlayer == &player1) ? &player2 : &player1;
                    
                    
                    for (int i = 0; i < 120; i++)
                    {
                        BeginDrawing();
                        DrawText(("Turno del Jugador " + std::to_string(currentPlayer == &player1 ? 1 : 2)).c_str(),
                            screenWidth / 2 - MeasureText("Turno del Jugador 1", 20) / 2,
                            screenHeight / 2, 40, BLACK);
                        EndDrawing();
                    }
                    
                    if (empate == 3)
                    {
                        StopSound(musica);
                        PlaySound(gameover);
                        for (int i = 0; i < 300; i++) {
                            BeginDrawing();
                            ClearBackground(RAYWHITE);
                            DrawTexture(menuTexture, 0, 0, WHITE);
                            DrawText("Empate", screenWidth / 2 - MeasureText("Empate", 20) / 2, screenHeight / 2, 40, WHITE);
                            EndDrawing();
                        }
                        play = false;
                        break;
                    }
                }
            }

            if (IsKeyPressed(KEY_R)) {
                PlaySound(girar);
                if (currentPlayer->HasSelectedDomino()) {
                    currentPlayer->GetSelectedDomino()->Rotate();
                }
            }

            if (IsKeyDown(KEY_W)) cam.target.y -= 10.0f;
            if (IsKeyDown(KEY_S)) cam.target.y += 10.0f;
            if (IsKeyDown(KEY_A)) cam.target.x -= 10.0f;
            if (IsKeyDown(KEY_D)) cam.target.x += 10.0f;

            BeginDrawing();
            ClearBackground({ 243, 228, 193, 255 });

            BeginMode2D(cam);
            DrawTexture(fondoTexture, -screenWidth, -screenHeight, WHITE);
            
            for (float i = -size; i <= size; i += 10) {
                DrawLine(i, -size, i, size, transparentGray);
                DrawLine(-size, i, size, i, transparentGray);
            }
            DrawLine(-size, 0, size, 0, RED);
            DrawLine(0, -size, 0, size, RED);
           
            for (const auto& domino : placedDominos) {
                domino.Draw(WHITE);
            }
            
            EndMode2D();

            player1.DrawHand(cam, BLUE);
            player2.DrawHand(cam, RED);

            if (IsKeyPressed(KEY_ESCAPE)) {
                StopSound(musica);
                PlaySound(gameover);
                for (int i = 0; i < 300; i++) { 
                    BeginDrawing();
                    ClearBackground(RAYWHITE);
                    DrawTexture(menuTexture, 0, 0, WHITE);
                    DrawText("Saliendo:c", screenWidth / 2 - MeasureText("Saliendo:c", 20) / 2, screenHeight / 2, 40, WHITE);
                    EndDrawing();
                }
                play = false;
                break;
            }
            EndDrawing();

            
        }
        StopSound(musica);
    }
    UnloadTexture(menuTexture);
    UnloadTexture(fondoTexture);
    UnloadTexture(botonTexture);
    UnloadTexture(botonTexture2);

    UnloadSound(intro);
    UnloadSound(Button);
    UnloadSound(ganar);
    UnloadSound(musica);
    UnloadSound(gameover);
    UnloadSound(poner);
    UnloadSound(girar);
    UnloadSound(place);
    UnloadSound(click);

    CloseAudioDevice();

    CloseWindow();

    return 0;
}
