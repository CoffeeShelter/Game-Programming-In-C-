#include "Game.h"

const int thickness = 15;
const float paddleH = 100.0f;

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

    mWindow = SDL_CreateWindow("Game Programming in C++ (Chapter 1)", 100, 100, WINDOW_WIDTH, WINDOW_HEIGHT, 0);
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

    mPaddlePos.x = 10.0f;
    mPaddlePos.y = WINDOW_HEIGHT / 2.0f;
    mBallPos.x = WINDOW_WIDTH / 2.0f;
    mBallPos.y = WINDOW_HEIGHT / 2.0f;
    mBallVel.x = -200.0f;
    mBallVel.y = 235.0f;
    mPaddleDir = 0;
    mTicksCount = 0;

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
    mPaddleDir = 0;
    if (state[SDL_SCANCODE_W])
    {
        mPaddleDir -= 1;
    }
    if (state[SDL_SCANCODE_S])
    {
        mPaddleDir += 1;
    }
}

void Game::UpdateGame()
{
    // 마지막 프레임 이후로 16ms가 경과할 때까지 대기
    while (!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
        ;

    // 델타 시간은 현재 프레임 틱값과 이전 프레임 틱값의 차다.
    // (차를 초 단위로 변환)
    float deltaTime = (SDL_GetTicks() - mTicksCount) / 1000.0f;
    if (deltaTime > 0.05f)
    {
        deltaTime = 0.05f;
    }

    // 다음 프레임을 위해 틱값을 갱신
    mTicksCount = SDL_GetTicks();

    if (mPaddleDir != 0)
    {
        mPaddlePos.y += mPaddleDir * 300.0f * deltaTime;
        // 패들이 화면 영역을 벗어나는지 검증
        if (mPaddlePos.y < (paddleH / 2.0f) + thickness)
        {
            mPaddlePos.y = paddleH / 2.0f + thickness;
        }
        else if (mPaddlePos.y > WINDOW_HEIGHT - paddleH / 2.0f - thickness)
        {
            mPaddlePos.y = WINDOW_HEIGHT - paddleH / 2.0f - thickness;
        }
    }

    mBallPos.x += mBallVel.x * deltaTime;
    mBallPos.y += mBallVel.y * deltaTime;

    float diff = mPaddlePos.y - mBallPos.y;
    diff = (diff > 0.0f) ? diff : -diff;
    if (diff <= paddleH / 2.0f && mBallPos.x <= 25.0f && mBallPos.x >= 20.0f && mBallVel.x < 0.0f)
    {
        mBallVel.x *= -1.0f;
    }
    // 공이 화면 밖으로 나갔는지 검사
    else if (mBallPos.x <= 0.0f)
    {
        mIsRunning = false;
    }
    // 공이 우측 벽에 닿았는지 확인
    else if (mBallPos.x >= (WINDOW_WIDTH - thickness) && mBallVel.x > 0.0f)
    {
        mBallVel.x *= -1.0f;
    }

    // 공이 상단 벽에 닿았는지 확인
    if (mBallPos.y <= thickness && mBallVel.y < 0.0f)
    {
        mBallVel.y *= -1.0f;
    }
    // 공이 하단 벽에 닿았는지 확인
    else if (mBallPos.y >= (768 - thickness) && mBallVel.y > 0.0f)
    {
        mBallVel.y *= -1.0f;
    }
}

void Game::GenerateOutput()
{
    SDL_SetRenderDrawColor(mRenderer, 0, 0, 255, 255);
    SDL_RenderClear(mRenderer);

    SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

    SDL_Rect topWall{0, 0, WINDOW_WIDTH, thickness};
    SDL_RenderFillRect(mRenderer, &topWall);
    SDL_Rect bottomWall{0, WINDOW_HEIGHT - thickness, WINDOW_WIDTH, thickness};
    SDL_RenderFillRect(mRenderer, &bottomWall);
    SDL_Rect RightWall{WINDOW_WIDTH - thickness, 0, thickness, WINDOW_HEIGHT};
    SDL_RenderFillRect(mRenderer, &RightWall);

    SDL_Rect paddle{static_cast<int>(mPaddlePos.x), static_cast<int>(mPaddlePos.y - paddleH / 2), thickness, static_cast<int>(paddleH)};
    SDL_RenderFillRect(mRenderer, &paddle);

    SDL_Rect ball{static_cast<int>(mBallPos.x - thickness / 2), static_cast<int>(mBallPos.y - thickness / 2), thickness, thickness};
    SDL_RenderFillRect(mRenderer, &ball);

    SDL_RenderPresent(mRenderer);
}