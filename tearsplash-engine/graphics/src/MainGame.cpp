/**********************************************************************/
// File:	MainGame.cpp
// Summary:	This is the class that runs the main and most high level
//			portion of the game. Here, things are initialized and run.
//
// Version: Version 1.0 - 2018-08-16
// Author:	Oscar Mårtensson
// -------------------------------------------
// Log:	    2018-08-16 File created
//          2019-03-24 Added timing class and input manager
/**********************************************************************/

// Includes -------------------------
#include <ctime>
#include <string>
#include <random>

#include <Tearsplash/Errors.h>
#include <Tearsplash/ImageLoader.h>
#include <Tearsplash/ResourceManager.h>
#include <Tearsplash/ParticleBatch2D.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/rotate_vector.hpp>

#include <imgui/imgui.h>
#include <imgui/imgui_impl_opengl3.h>
#include <imgui/imgui_impl_sdl.h>

#include "MainGame.h"

// ----------------------------------
// Default constructor
MainGame::MainGame() : 
    mCurrentGameState(GameState::PLAY),
    mFPS(0.0f),
    mMaxFPS(60.0f),
    mWindowWidth(1280), mWindowHeight(720),
    mGravity(0.0f, -9.82f),
    mPlayerPosition(glm::vec2(0.0, 0.0f)),
    mPlayerDirection(glm::vec2(1.0, 0.0f)) {}

// ----------------------------------
// Default destructor
MainGame::~MainGame()
{
    mAudioEngine.destroy();
}

// ----------------------------------
// Runs the game
void MainGame::run()
{
    initSystems();

    gameLoop();
}

// ----------------------------------
// Initializes vital game engine systems such as memory allocation, SDL (for input and creating window),
// etc.
void MainGame::initSystems()
{
    Tearsplash::init();
    mAudioEngine.init();

    mFPSLimiter.init(mMaxFPS);

    mWindow.createWindow("Tearsplash", mWindowWidth, mWindowHeight, Tearsplash::WindowFlags::RESIZABLE);
    mCamera.init(mWindowWidth, mWindowHeight);
    mCamera.setScale(2.0f);

    initShaders();
    mSpritebatch.init();

    mHUDText.init("fonts/28_Days_Later.ttf");

    createPhysicsObjects();

    initParticleSystem();

    initImGui();
}

// ----------------------------------
// Main game loop.
void MainGame::gameLoop()
{
    // Keep looping while player hasn't pressed exit
    while (mCurrentGameState != GameState::EXIT)
    { 
        float timeStep = 1.0f / 60.0f;
        mFPSLimiter.begin();

        float startTicks = static_cast<float>(SDL_GetTicks());
        processInput();

        mCamera.update();

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplSDL2_NewFrame(mWindow.getSDLWindow());
        ImGui::NewFrame();

        ImGui::Begin("Spawn box");
        if (ImGui::Button("Button")) {
          // Create a bunch of falling boxes.
          Tearsplash::Box box;
          box.init(mPhysicsWorld.get(), glm::vec2(0.0f, 100.0f), glm::vec2(15.0f, 15.0f), b2_dynamicBody);
          mPhysicsBoxes.push_back(box);
        }
        ImGui::End();

        // Update all bullets
        for (size_t i = 0; i < mBullets.size();)
        {
            if (mBullets[i].update() == true)
            {
                mBullets[i] = mBullets.back();
                mBullets.pop_back();
            }
            else
            {
                i++;
            }
        }

        mParticleEngine.updateBatches(timeStep);

        updatePhysics(timeStep);

        render();

        mFPS = mFPSLimiter.end();

        printFPS();
    }

    shutdownImGui();

    return;
}

// ----------------------------------
// Processes user input.
void MainGame::processInput()
{
    const float SCALE_SPEED = 0.1f;
    const float CAMERA_SPEED = 5.0f;
    const float PLAYER_SPEED = 5.0f;

    mInputManager.update();
    SDL_Event userInput;

    while (SDL_PollEvent(&userInput) == 1)
    {
        ImGui_ImplSDL2_ProcessEvent(&userInput);
        // There is a user input present
        switch (userInput.type)
        {
            case SDL_QUIT:
                mCurrentGameState = GameState::EXIT;
                break;

            case SDL_MOUSEMOTION:
                mInputManager.setMouseCoords(static_cast<float>(userInput.motion.x), static_cast<float>(userInput.motion.y));
                glm::vec2 mouseCoords = mInputManager.getMouseCoords();
                mouseCoords = mCamera.convertScreen2World(mouseCoords);
                mPlayerDirection = glm::normalize(mouseCoords - mPlayerPosition);
                break;

            case SDL_KEYDOWN:
                mInputManager.pressKey(userInput.key.keysym.sym);
                break;

            case SDL_KEYUP:
                mInputManager.releaseKey(userInput.key.keysym.sym);
                break;

            case SDL_MOUSEBUTTONDOWN:
                mInputManager.pressKey(userInput.button.button);
                break;

            case SDL_MOUSEBUTTONUP:
                mInputManager.releaseKey(userInput.button.button);
                break;

            default:
                // Do nothing
                break;
        }
    }

    // Check for key pressed in input manager and add action

    if (mInputManager.isKeyPressed(SDLK_w))
    {
        // NOTE! Camera is moving down, scene moving up
        //mCamera.setPosition(mCamera.getPosition() + glm::vec2(0.0f, -CAMERA_SPEED));
        mPlayerPosition = mPlayerPosition + glm::vec2(0.0f, PLAYER_SPEED);
    }

    if (mInputManager.isKeyPressed(SDLK_s))
    {
        // NOTE! Camera is moving up, scene moving down
        //mCamera.setPosition(mCamera.getPosition() + glm::vec2(0.0f, CAMERA_SPEED));

        mPlayerPosition = mPlayerPosition + glm::vec2(0.0f, -PLAYER_SPEED);
    }

    if (mInputManager.isKeyPressed(SDLK_a))
    {
        // NOTE! Camera is moving right, scene moving left
        //mCamera.setPosition(mCamera.getPosition() + glm::vec2(CAMERA_SPEED, 0.0f));

        mPlayerPosition = mPlayerPosition + glm::vec2(-PLAYER_SPEED, 0.0f);
    }

    if (mInputManager.isKeyPressed(SDLK_d))
    {
        // NOTE! Camera is moving left, scene moving right
        //mCamera.setPosition(mCamera.getPosition() + glm::vec2(-CAMERA_SPEED, 0.0f));

        mPlayerPosition = mPlayerPosition + glm::vec2(PLAYER_SPEED, 0.0f);
    }

    if (mInputManager.isKeyPressed(SDLK_q))
    {
        mCamera.setScale(mCamera.getScale() - SCALE_SPEED);
    }

    if (mInputManager.isKeyPressed(SDLK_e))
    {
        mCamera.setScale(mCamera.getScale() + SCALE_SPEED);
    }

    if (mInputManager.isKeyPressed(SDLK_f))
    {
        glm::vec2 mouseCoords = mInputManager.getMouseCoords();
        // This is really ugly, should not try to load the sound effect every time a bullet is fired.
        mBullets.emplace_back(mPlayerPosition, mPlayerDirection, 10.0f, 1000, mAudioEngine.loadSoundEffect("sound/shots/pistol.wav"));
        mBullets.back().playSoundFX();
    }
}

// ----------------------------------
// Rendering main function
void MainGame::render()
{
    // Prepare for rendering
    glClearDepth(1.0f); // Clear depth to 1
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	// Clear color buffer and depth buffer between draws

    // Use shader program and set first texture (0)
    mColorShaders.use();
    glActiveTexture(GL_TEXTURE0);

  
    // Set uniforms
    GLint textureLocation = mColorShaders.getUniformLocation("texSampler");
    glUniform1i(textureLocation, 0);


    GLint pLocation = mColorShaders.getUniformLocation("P");
    glm::mat4 cameraMatrix = mCamera.getCameraMatrix();
    glUniformMatrix4fv(pLocation, 1, GL_FALSE, &(cameraMatrix[0][0]));

    // Start filling sprite batches
    mSpritebatch.begin(Tearsplash::GlyphSortType::TEXTURE);

    glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
    static Tearsplash::GLTexture playerTexture = Tearsplash::ResourceManager::getTexture("textures/jimmyJump_pack/PNG/CharacterRight_Standing.png");
    Tearsplash::ColorRGBA8 color;
    color.r = 255;
    color.g = 255;
    color.b = 255;
    color.a = 255;

    // Draw the player sprite.
    mSpritebatch.draw(glm::vec4(mPlayerPosition, 50.0, 50.0), uv, playerTexture.id, 0, color, mPlayerDirection);

    for (size_t i = 0; i < mBullets.size(); i++)
    {
        // Render the bullets to the sprite batch.
        if (mCamera.isInView(mBullets[i].getPosition(), mBullets[i].getAABB())) {
            mBullets[i].draw(mSpritebatch);
        }
    }

    // Draw the physics boxes.
    static Tearsplash::GLTexture brickTexture = Tearsplash::ResourceManager::getTexture("textures/01bricks1.png");
    for (auto& box : mPhysicsBoxes) {
        glm::vec4 destRect;
        // Subtract half of the dimensions since the physics box origin is in the box center,
        // but our draw center is in the corner.
        destRect.x = box.getBody()->GetPosition().x - box.getDimensions().x * 0.5f;
        destRect.y = box.getBody()->GetPosition().y - box.getDimensions().y * 0.5f;
        destRect.z = box.getDimensions().x;
        destRect.w = box.getDimensions().y;
        mSpritebatch.draw(destRect, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f), brickTexture.id, 0.0f, Tearsplash::ColorRGBA8(255, 255, 255, 255));
    }

    // Stop filling sprite batches
    mSpritebatch.end();

    // Render sprite batches
    mSpritebatch.renderBatch();

    // Draw the particles.
    mParticleEngine.drawBatches();

    // Stop using shader program
    mColorShaders.dontuse();

    // Render Text
    mHUDText.drawText("hejsan sa", glm::vec4(100.0f, 0.0f, 0.0f, 0.0f), cameraMatrix, glm::vec3(1.0f, 1.0f, 1.0f), 1.0f);
    mHUDText.render();

    ImGui::Render();
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

    mWindow.swapBuffer();
}

// ----------------------------------
// Initializes shaders
void MainGame::initShaders()
{
    mColorShaders.compileShaders("shaders/colorShading.vert", "shaders/colorShading.frag");

    // Setup attribute pointers
    mColorShaders.addAttribute("vertexPosition");
    mColorShaders.addAttribute("vertexColor");
    mColorShaders.addAttribute("vertexUV");

    mColorShaders.linkShaders();
}

// ----------------------------------
// Prints fps each 10 frames
void MainGame::printFPS()
{
    static int frameCounter = 0;

    frameCounter++;

    // Print each 10th frame
    if (frameCounter == 100)
    {
        std::cout << "fps: " << mFPS << std::endl;
        frameCounter = 0;
    }
}

void MainGame::createPhysicsObjects()
{
    // Create the physics world.
    mPhysicsWorld = std::make_unique<b2World>(mGravity);

    // Create a static ground box.
    Tearsplash::Box ground;
    ground.init(mPhysicsWorld.get(), glm::vec2(0.0f, -10.0f), glm::vec2(100.0f, 15.0f));
    // Don't add this to the mPhysicsBoxes since we don't want it rendered.
    //mPhysicsBoxes.push_back(ground);

    // Create a bunch of falling boxes.
    Tearsplash::Box box;
    box.init(mPhysicsWorld.get(), glm::vec2(0.0f, 100.0f), glm::vec2(15.0f, 15.0f), b2_dynamicBody);
    mPhysicsBoxes.push_back(box);
}
 
void MainGame::updatePhysics(const float timeStep)
{
    // Since we're locking the fps to 60, we'll use that as a timestep.
    // If the FPS starts lagging behind this won't work very well.
    mPhysicsWorld->Step(timeStep, 6, 2);
}

void MainGame::initParticleSystem() {
    // Init the spritebatch used for the particles.
    mSpritebatchParticles.init();

    const int maxParticles = 1000;
    //mParticleTexture = Tearsplash::ResourceManager::getTexture("textures/smoke_07.png");
    mParticleTexture = Tearsplash::ResourceManager::getTexture("textures/whitePuff02.png");
    mParticleBatch2D.init(maxParticles, 1.0f, mParticleTexture);
    // Setup random generator for random angle around 360 deg circle.
    std::mt19937 mt_rand(std::time(nullptr));
    std::uniform_real_distribution<float> randAngleCircle(0.0f, 360);

    // Specify velocity and rotate it.
    mParticleVelocity = glm::vec2(17.0f, 0.0f);

    // Color the particles.
    mParticleColor.r = 255;
    mParticleColor.g = 0;
    mParticleColor.b = 0;
    mParticleColor.a = 255;
    for (int i = 0; i < maxParticles; i++) {
        mParticleBatch2D.addParticle(glm::vec2(-100.0f, 0.0f), glm::rotate(mParticleVelocity, randAngleCircle(mt_rand)), mParticleColor, 5.0f, 1.0f);
    }

    mParticleEngine.addParticleBatch(mParticleBatch2D, mSpritebatchParticles);
}

void MainGame::initImGui() {
  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();

  // Setup Platform/Renderer bindings
  ImGui_ImplSDL2_InitForOpenGL(mWindow.getSDLWindow(), mWindow.getGLContext());
  ImGui_ImplOpenGL3_Init(mWindow.getGLSLVersion());
}

void MainGame::shutdownImGui() {
  ImGui_ImplOpenGL3_Shutdown();
  ImGui_ImplSDL2_Shutdown();
  ImGui::DestroyContext();
}