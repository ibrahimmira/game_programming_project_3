#include "Entity.h"

Entity::Entity(Vector2 position, Vector2 scale, 
    std::vector<const char*> textureFilepaths, TextureType textureType,
    Vector2 spriteSheetDimensions, std::map<RocketState, std::vector<int>> animationAtlas, EntityType entityType) : mPosition {position}, 
    mMovement { 0.0f, 0.0f }, mScale {scale}, mColliderDimensions {scale}, 
    mTextureType {ATLAS}, 
    mSpriteSheetDimensions {spriteSheetDimensions}, 
    mAnimationAtlas {animationAtlas},
    mAnimationIndices {animationAtlas.at(IDLE)}, 
    mFrameSpeed {DEFAULT_FRAME_SPEED}, mAngle { 0.0f }, 
    mSpeed { DEFAULT_SPEED }, mRocketStatus { IDLE }, 
    mEntityType { entityType }
{
    for (int i = 0; i < textureFilepaths.size(); i++)
        mTextures[(RocketState) i] = LoadTexture(textureFilepaths[i]);

    mCurrentTexture = mTextures[mRocketStatus];
}

Entity::Entity(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType)
    : mPosition{position},
      mMovement{0.0f, 0.0f},
      mVelocity{0.0f, 0.0f},
      mAcceleration{0.0f, 0.0f},
      mScale{scale},
      mColliderDimensions{scale},
      mTextureType{SINGLE},
      mSpriteSheetDimensions{},
      mAnimationAtlas{},
      mAnimationIndices{},
      mFrameSpeed{0},
      mAngle{0.0f},
      mSpeed{DEFAULT_SPEED - 150},
      mRocketStatus{IDLE}, 
      mEntityType { entityType },
      mStartingXPosition{position.x}
      
{
    mTextures[IDLE]   = LoadTexture(textureFilepath);
    mCurrentTexture   = mTextures[IDLE];
}

Entity::~Entity() 
{
    for (int i = 0; i < mTextures.size(); i++)
        UnloadTexture(mTextures[(RocketState) i]); 
};

/**
 * Iterates through a list of collidable entities, checks for collisions with
 * the player entity, and resolves any vertical overlap by adjusting the 
 * player's position and velocity accordingly.
 * 
 * @param collidableEntities An array of pointers to `Entity` objects that 
 * represent the entities that the current `Entity` instance can potentially
 * collide with. The `collisionCheckCount` parameter specifies the number of
 * entities in the `collidableEntities` array that need to be checked for
 * collision.
 * @param collisionCheckCount The number of entities that the current entity
 * (`Entity`) should check for collisions with. This parameter specifies how
 * many entities are in the `collidableEntities` array that need to be checked
 * for collisions with the current entity.
 */
void Entity::checkCollisionY(Entity *collidableEntities, int collisionCheckCount)
{

    for (int i = 0; i < collisionCheckCount; i++)
    {
        // STEP 1: For every entity that our player can collide with...
        Entity *collidableEntity = &collidableEntities[i];
        
        if (isColliding(collidableEntity))
        {
            // STEP 2: Calculate the distance between its centre and our centre
            //         and use that to calculate the amount of overlap between
            //         both bodies.
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) - (collidableEntity->mColliderDimensions.y / 2.0f));
            
            // STEP 3: "Unclip" ourselves from the other entity, and zero our
            //         vertical velocity.
            if (mVelocity.y > 0) 
            {
                mPosition.y -= yOverlap;
                mVelocity.y  = 0;
                mIsCollidingBottom = true;
            } else if (mVelocity.y < 0) 
            {
                mPosition.y += yOverlap;
                mVelocity.y  = 0;
                mIsCollidingTop = true;
            }
        }
    }
}

void Entity::checkCollisionX(Entity *collidableEntities, int collisionCheckCount)
{
    for (int i = 0; i < collisionCheckCount; i++)
    {
        Entity *collidableEntity = &collidableEntities[i];
        
        if (isColliding(collidableEntity))
        {            
            // When standing on a platform, we're always slightly overlapping
            // it vertically due to gravity, which causes false horizontal
            // collision detections. So the solution I dound is only resolve X
            // collisions if there's significant Y overlap, preventing the 
            // platform we're standing on from acting like a wall.
            float yDistance = fabs(mPosition.y - collidableEntity->mPosition.y);
            float yOverlap  = fabs(yDistance - (mColliderDimensions.y / 2.0f) - (collidableEntity->mColliderDimensions.y / 2.0f));

            // Skip if barely touching vertically (standing on platform)
            if (yOverlap < Y_COLLISION_THRESHOLD) continue;

            float xDistance = fabs(mPosition.x - collidableEntity->mPosition.x);
            float xOverlap  = fabs(xDistance - (mColliderDimensions.x / 2.0f) - (collidableEntity->mColliderDimensions.x / 2.0f));

            if (mVelocity.x > 0) {
                mPosition.x     -= xOverlap;
                mVelocity.x      = 0;

                // Collision!
                mIsCollidingRight = true;
            } else if (mVelocity.x < 0) {
                mPosition.x    += xOverlap;
                mVelocity.x     = 0;
 
                // Collision!
                mIsCollidingLeft = true;
            }
        }
    }
}

/**
 * Checks if two entities are colliding based on their positions and collider 
 * dimensions.
 * 
 * @param other represents another Entity with which you want to check for 
 * collision. It is a pointer to the Entity class.
 * 
 * @return returns `true` if the two entities are colliding based on their
 * positions and collider dimensions, and `false` otherwise.
 */
bool Entity::isColliding(Entity *other) const 
{
    float xDistance = fabs(mPosition.x - other->getPosition().x) - 
        ((mColliderDimensions.x + other->getColliderDimensions().x) / 2.0f);
    float yDistance = fabs(mPosition.y - other->getPosition().y) - 
        ((mColliderDimensions.y + other->getColliderDimensions().y) / 2.0f);

    if (xDistance < 0.0f && yDistance < 0.0f) return true;

    return false;
}

/**
 * Updates the current frame index of an entity's animation based on the 
 * elapsed time and frame speed.
 * 
 * @param deltaTime represents the time elapsed since the last frame update.
 */
void Entity::animate(float deltaTime)
{
    // mAnimationIndices = mAnimationAtlas.at(mDirection);
    mAnimationIndices = mAnimationAtlas.at(mRocketStatus);

    mAnimationTime += deltaTime;
    float framesPerSecond = 1.0f / mFrameSpeed;

    if (mAnimationTime >= framesPerSecond)
    {
        mAnimationTime = 0.0f;

        mCurrentFrameIndex++;
        mCurrentFrameIndex %= mAnimationIndices.size();
    }
}

void Entity::displayCollider() 
{
    // draw the collision box
    Rectangle colliderBox = {
        mPosition.x - mColliderDimensions.x / 2.0f,  
        mPosition.y - mColliderDimensions.y / 2.0f,  
        mColliderDimensions.x,                        
        mColliderDimensions.y                        
    };

    DrawRectangleLines(
        colliderBox.x,      // Top-left X
        colliderBox.y,      // Top-left Y
        colliderBox.width,  // Width
        colliderBox.height, // Height
        GREEN               // Color
    );
}

void Entity::update(float deltaTime, Entity *collidableEntities[], int collisionCheckCount)
{
    if  (mEntityStatus == INACTIVE) return;
    if  (mEntityType != ROCKET)     return;
    displayStats();
    if (mIsGameOver) {
        gameOver();
    }

    if (!mIsGameOver) {
        for (int i = 0; i < collisionCheckCount; i++) {
            checkCollisionY(collidableEntities[i], collisionCheckCount);
            checkCollisionX(collidableEntities[i], collisionCheckCount);
        }

        if (mIsCollidingLeft || mIsCollidingRight || mIsCollidingTop) {
            mGameOverReason = CRASHED;
            mIsGameOver = true;
            for (int i = 0; i < collisionCheckCount; i++) collidableEntities[i]->setGameOver();
        }

        else if (mIsCollidingBottom && fabs(mVelocity.x) <= 5.0f) {

            mGameOverReason = LANDED_SUCCESSFULLY;
            mIsGameOver = true;
            for (int i = 0; i < collisionCheckCount; i++) collidableEntities[i]->setGameOver();
            
        }

        resetColliderFlags();
        resetAcceleration();

        if (mAcceleratingUp)    mAcceleration.y -= THRUSTING_ACCELERATION;
        if (mAcceleratingLeft)  mAcceleration.x -= HORIZONTAL_ACCELERATION;
        if (mAcceleratingRight) mAcceleration.x += HORIZONTAL_ACCELERATION;


        if (!mAcceleratingLeft && !mAcceleratingRight) 
        {
            applyDrag(deltaTime);
        }

        setRocketState((mAcceleratingUp || mAcceleratingLeft || mAcceleratingRight) ? THRUSTING : IDLE);

        mAcceleratingRight = mAcceleratingLeft = mAcceleratingUp = false;

        mVelocity.x += mAcceleration.x * deltaTime;
        mVelocity.y += mAcceleration.y * deltaTime;

        mPosition.x += mVelocity.x * deltaTime;
        mPosition.y += mVelocity.y * deltaTime;
        

        if (getPosition().y > 850.0f || getPosition().y < -50.0f || 
            getPosition().x < -50.0f || getPosition().x > 1550.0f) 
        {
            mGameOverReason = OUT_OF_BOUNDS;
            mIsGameOver = true;
            for (int i = 0; i < collisionCheckCount; i++) collidableEntities[i]->setGameOver();
        }

        if (mFuelTank <= 0.0f){

            mFuelTank = 0.0f;
            mGameOverReason = OUT_OF_FUEL;
            mIsGameOver = true;
            for (int i = 0; i < collisionCheckCount; i++) collidableEntities[i]->setGameOver();
        } 
    
    if (mTextureType == ATLAS) 
    
        animate(deltaTime);
    }
}

void Entity::update(float deltaTime) {

    if  (mEntityStatus == INACTIVE) return;
    if  (mEntityType != MOVING_LANDING_PAD) return;
    if  (mIsGameOver) return;

    if (mEntityType == MOVING_LANDING_PAD) {
        mPosition.x += mSpeed * deltaTime;

        if (mPosition.x > mStartingXPosition + 500.0f || mPosition.x < mStartingXPosition - 500.0f) {
            mSpeed = -mSpeed;
        }
    }
}

void Entity::render()
{
    if(mEntityStatus == INACTIVE) return;

    Rectangle textureArea;

    switch (mTextureType)
    {
        case SINGLE:
            // Whole texture (UV coordinates)
            textureArea = {
                // top-left corner
                0.0f, 0.0f,

                // bottom-right corner (of texture)
                static_cast<float>(mCurrentTexture.width),
                static_cast<float>(mCurrentTexture.height)
            };
            break;
        case ATLAS:
            textureArea = getUVRectangle(
                &mCurrentTexture, 
                mAnimationIndices[mCurrentFrameIndex], 
                mSpriteSheetDimensions.x, 
                mSpriteSheetDimensions.y
            );
        
        default: break;
    }

    // Destination rectangle – centred on gPosition
    Rectangle destinationArea = {
        mPosition.x,
        mPosition.y,
        static_cast<float>(mScale.x),
        static_cast<float>(mScale.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 originOffset = {
        static_cast<float>(mScale.x) / 2.0f,
        static_cast<float>(mScale.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        mCurrentTexture, 
        textureArea, destinationArea, originOffset,
        mAngle, WHITE
    );

    displayCollider();
}

void Entity::setRocketState(RocketState newState)
{
    if (mRocketStatus == newState) return;

        mRocketStatus = newState;
        mCurrentTexture = mTextures[mRocketStatus];
        mAnimationIndices = mAnimationAtlas.at(mRocketStatus);
        mCurrentFrameIndex = 0;
        mAnimationTime = 0.0f;
}

void Entity::displayStats() 
{
    float ticks = (float) GetTime();
    int countdown = 60 - (int)ticks;


    DrawText(TextFormat("Fuel: %04.2f%%", mFuelTank), 20, 20, 20, WHITE);
    DrawText(TextFormat("Altitude: %08.2f", 800 - getPosition().y), 1500 - 300, 20, 20, WHITE);
    DrawText(TextFormat("Horizontal Speed: %08.2f", getVelocity().x), 1500 - 300, 50, 20, WHITE);
    DrawText(TextFormat("Vertical Speed: %08.2f", getVelocity().y), 1500 - 300, 80, 20, WHITE);
}

void Entity::gameOver() 
{
    if (mGameOverReason == OUT_OF_BOUNDS) {
        DrawText("MISSION FAILED: OUT OF BOUNDS", 1500 / 2 - 450, 800 / 2, 50, RED);
        return;
    }

    if (mGameOverReason == OUT_OF_FUEL) {
        DrawText("MISSION FAILED: OUT OF FUEL", 1500 / 2 - 350, 800 / 2, 50, RED);
        return;
    }

    if (mGameOverReason == CRASHED) {
        DrawText("MISSION FAILED: CRASHED", 1500 / 2 - 350, 800 / 2, 50, RED);
        return;
    }

    if (mGameOverReason == LANDED_SUCCESSFULLY) {
        DrawText("MISSION ACCOMPLISHED: LANDED SUCCESSFULLY", 1500 / 2 - 650, 800 / 2, 50, GREEN);
        return;
    }
}