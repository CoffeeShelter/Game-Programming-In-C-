#include "Game.h"

Game::Game()
{
    mWindow = nullptr;
    mRenderer = nullptr;
    mIsRunning = true;
}

bool Game::Initialize()
{
    int sdlResult = SDL_Init(SDL_INIT_VIDEO);
    if (sdlResult != 0)
    {
        SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
        return false;
    }

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)", 100, 100, 1024, 764, 0);
    if (!mWindow)
    {
        SDL_Log("Failed to create window: %s", SDL_GetError());
        return false;
    }

    mRenderer = SDL_CreateRenderer(mWindow, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
    if (!mRenderer)
    {
        SDL_Log("Failed to create renderer: %s", SDL_GetError());
        return false;
    }

    return true;
}

void Game::Shutdown()
{
    SDL_DestroyRenderer(mRenderer);
    SDL_DestroyWindow(mWindow);
    SDL_Quit();
}

void Game::RunLoop()
{
    while (mIsRunning)
    {
        ProcessInput();
        UpdateGame();
        GenerateOutput();
    }
}

void Game::ProcessInput()
{
    SDL_Event event;
    // 큐에 여전히 이벤트가 남아 있는 동안
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
        case SDL_QUIT:
            mIsRunning = false;
            break;

        default:
            break;
        }
    }

    // 키보드의 상태 얻기
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // 이스케이프 키를 눌렀다면 루프 종료
    if (state[SDL_SCANCODE_ESCAPE])
    {
        mIsRunning = false;
    }
}

void Game::UpdateGame()
{
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);
    const int thickness = 15;

    SDL_Rect topWall{0, 0, 1024, thickness};
    SDL_RenderFillRect(mRenderer, &topWall);
    SDL_Rect bottomWall{0, 764 - thickness, 1024, thickness};
    SDL_RenderFillRect(mRenderer, &bottomWall);
    SDL_Rect RightWall{1024 - thickness, 0, thickness, 764};
    SDL_RenderFillRect(mRenderer, &RightWall);

    SDL_RenderPresent(mRenderer);
}