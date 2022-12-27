#include "Game.h"

const int thickness = 15;
const int paddleHeight = 100;
const int windowHeight = 768;
const int windowWidth = 1024;


Game::Game(){}

bool Game::Initialize(){
  int sdlResult = SDL_Init(SDL_INIT_VIDEO);
  if (sdlResult != 0){
    SDL_Log("Unable to initialize SDL: %s", SDL_GetError());
    return false;
  }

  mWindow = SDL_CreateWindow(
    "Programming in C++ (Chapter 1)",
    100,
    100,
    windowWidth,
    windowHeight,
    0
  );
  if (!mWindow){
    SDL_Log("Failed to create window: %s", SDL_GetError());
    return false;
  }

  mRenderer = SDL_CreateRenderer(
    mWindow,
    -1,
    SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC
  );
  if (!mRenderer){
    SDL_Log("Failed to create renderer: %s", SDL_GetError());
    return false;
  }

  mPaddlePos.x = thickness/2;
  mPaddlePos.y = windowHeight/2;
  mBallPos.x = windowWidth/2;
  mBallPos.y = windowHeight/2;
  mBallVel.x = -200.0;
  mBallVel.y = 235.0;

  mTicksCount = 0;
  mIsRunning = true;

  return true;
}

void Game::Shutdown(){
  SDL_DestroyRenderer(mRenderer);
  SDL_DestroyWindow(mWindow);
  SDL_Quit();
}

void Game::RunLoop(){
  while (mIsRunning){
    ProcessInput();
    UpdateGame();
    GenerateOutput();
  }
}

void Game::ProcessInput(){
  SDL_Event event;
  while (SDL_PollEvent(&event)){
    switch (event.type){
      case SDL_QUIT:
        mIsRunning = false;
        break;
    }
  }
  const Uint8* state = SDL_GetKeyboardState(NULL);
  if (state[SDL_SCANCODE_ESCAPE]){
    mIsRunning = false;
  }
  mPaddleDir = 0;
  if (state[SDL_SCANCODE_UP]){
    mPaddleDir -= 1;
  }
  if (state[SDL_SCANCODE_DOWN]){
    mPaddleDir += 1;
  }
}

void Game::UpdateGame(){
  while(!SDL_TICKS_PASSED(SDL_GetTicks(), mTicksCount + 16))
  ;
  float deltaTime = (SDL_GetTicks() - mTicksCount)/1000.0;
  if (deltaTime > 0.05){
    deltaTime = 0.05;
  }

  mTicksCount = SDL_GetTicks();

  if (mPaddleDir != 0){
    mPaddlePos.y += mPaddleDir * 300 * deltaTime;
    if (mPaddlePos.y < paddleHeight/2 + thickness){
      mPaddlePos.y = paddleHeight/2 + thickness;
    }
    else if (mPaddlePos.y > windowHeight - paddleHeight/2 - thickness){
      mPaddlePos.y = windowHeight - paddleHeight/2 - thickness;
    }
  }

  mBallPos.x += mBallVel.x * deltaTime;
  mBallPos.y += mBallVel.y * deltaTime;
  if (mBallPos.y <= thickness && mBallVel.y < 0.0){
    mBallVel.y *= -1;
  }
  if (mBallPos.y >= windowHeight - thickness && mBallVel.y > 0.0){
    mBallVel.y *= -1;
  }
  if (mBallPos.x >= windowWidth - thickness && mBallVel.x > 0.0){
    mBallVel.x *= -1;
  }
  float diff = abs(mBallPos.y - mPaddlePos.y);
  // Chcek ball collision with Paddle
  if (diff <=  paddleHeight/2 && mBallPos.x >= 20 && mBallPos.x <= 25 && mBallVel.x < 0.0){
    mBallVel.x *= -1;
  }
}

void Game::GenerateOutput(){
  // BACKGROUND
  SDL_SetRenderDrawColor(
    mRenderer,
    0,   // Red
    0,   // Green
    255, // Blue
    255  // Alpha
  );
  SDL_RenderClear(mRenderer); // Clear the back buffer to the current color

  // WALLS
  SDL_SetRenderDrawColor(mRenderer, 255, 255, 255, 255);

  // This is called "brace initialization" and requires to add -std=c++11 to the compiling command
  SDL_Rect upper_wall{
		0,		       // Top left x
		0,		       // Top left y
		1024,	       // Width
		thickness    // Height
	};
  SDL_RenderFillRect(mRenderer, &upper_wall);

  SDL_Rect lower_wall{
    0,		          // Top left x
    768-thickness,	// Top left y
    1024,		        // Width
    thickness    	  // Height
  };
  SDL_RenderFillRect(mRenderer, &lower_wall);

  SDL_Rect right_wall{
    1024-thickness, // Top left x
    0,			        // Top left y
    thickness,		  // Width
    768    	        // Height
  };
  SDL_RenderFillRect(mRenderer, &right_wall);

  // BALL
  SDL_Rect ball{
    static_cast<int>(mBallPos.x - thickness/2),
    static_cast<int>(mBallPos.y - thickness/2),
    thickness,
    thickness
  };
  SDL_RenderFillRect(mRenderer, &ball);

  // PADDLE
  SDL_Rect paddle{
    static_cast<int>(mPaddlePos.x - thickness/2),
    static_cast<int>(mPaddlePos.y - 100/2),
    thickness,
    100
  };
  SDL_RenderFillRect(mRenderer, &paddle);

  // Update the screen with any rendering performed since the previous call
  SDL_RenderPresent(mRenderer); // Swap fron and back buffers
}
