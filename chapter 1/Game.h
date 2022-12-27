#include <SDL2/SDL.h>

struct Vector2{
  float x;
  float y;
};

class Game{
  public:
    Game();
    bool Initialize();
    void RunLoop();
    void Shutdown();
  private:
    void ProcessInput();
    void UpdateGame();
    void GenerateOutput();

    bool mIsRunning;
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;

    Vector2 mBallPos;
    Vector2 mBallVel;
    Vector2 mPaddlePos;
    int mPaddleDir;

    Uint32 mTicksCount;
};
