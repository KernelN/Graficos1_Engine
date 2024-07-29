#include "Game.h"
#include <iostream>
#include "Utility/KeyCodes.h"
#include "Utility/Color.h"
#include "Utility/TimeSingleton.h"

unsigned int spriteNum = 0;
float timer = 0;

Game::Game()
{
    float s1Colors[4][4] =
    {
        {WHITE},
        {RED},
        {RED},
        {WHITE}
    };

    float s2Colors[4][4] =
    {
        {YELLOW},
        {GREEN},
        {GREEN},
        {YELLOW}
    };

    //player = new Square(s1Colors, true);
    player = new Sprite("res/link_walk.png", 8, 4);
    player->Scale(25, 25);
    player->Translate(150, 0);

    //enemy = new Square(s2Colors, true);
    enemy = new Sprite("res/WolfiesGrowl.png", 5, 0);
    enemy->Scale(25, 25);
    enemy->Translate(-150, 0);


    float linkWSpeed = 0.4f;
    linkWRightAnim = new Animation(linkWSpeed, 8, 0, 1);
    linkWUpAnim = new Animation(linkWSpeed, 8, 2, 3);
    linkWLeftAnim = new Animation(linkWSpeed, 8, 4, 5);
    linkWDownAnim = new Animation(linkWSpeed, 8, 6, 7);

    //Animation* wolfieWalkAnim = new Animation(1, 5);
    Animation* wolfieGrowlAnim = new Animation(1, 5);

    static_cast<Sprite*>(player)->SetAnim(linkWDownAnim);
    static_cast<Sprite*>(enemy)->SetAnim(wolfieGrowlAnim);

    tilemap = new TileMap();
    tilemap->importTileMap("res/tilemap/tilemap.tmx");
    tilemap->scaleTiles(20);

    scaleMod = 1;
}

Game::~Game()
{
    delete player;
    delete enemy;
}

bool Game::IsRunning()
{
	return BaseEngine::IsRunning();
}

void Game::Loop()
{ 
	BaseEngine::Loop();
}

void Game::OnLoop()
{
    static_cast<Sprite*>(enemy)->UpdateFrame();

    if (timer > 0) timer -= time->GetDelta();

    if (IsKeyPressed(KEY_W))
    {
        static_cast<Sprite*>(player)->SetAnim(linkWUpAnim);
        verticalMoveMod = 1;
    }
    else if (IsKeyPressed(KEY_S))
    {
        static_cast<Sprite*>(player)->SetAnim(linkWDownAnim);
        verticalMoveMod = -1;
    }
    else
    {
        verticalMoveMod = 0;
    }

    if (IsKeyPressed(KEY_A))
    {
        static_cast<Sprite*>(player)->SetAnim(linkWLeftAnim);
        horizontalMoveMod = -1;
    }
    else if (IsKeyPressed(KEY_D))
    {
        static_cast<Sprite*>(player)->SetAnim(linkWRightAnim);
        horizontalMoveMod = 1;
    }
    else
    {
        horizontalMoveMod = 0;
    }

    if (verticalMoveMod == 0 && horizontalMoveMod == 0) return;

    float verticalMove = 100.0f * verticalMoveMod * time->GetDelta();
    float horizontalMove = 100.0f * horizontalMoveMod * time->GetDelta();

    static_cast<Sprite*>(player)->UpdateFrame();

    player->Translate(0, verticalMove);
    player->Translate(horizontalMove, 0);

    // Colisionan player y enemy
    while (CollisionManager::CheckCollision(player, enemy))
    {
        player->Translate(0, -verticalMove);
        player->Translate(-horizontalMove, 0);
    }
}

void Game::Draw()
{
    static_cast<TileMap*>(tilemap)->Draw();
    static_cast<Sprite*>(player)->Draw();
    static_cast<Sprite*>(enemy)->Draw();
}