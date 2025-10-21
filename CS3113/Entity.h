#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum Direction    { LEFT, UP, RIGHT, DOWN }; // For walking
enum EntityStatus { ACTIVE, INACTIVE      };
enum RocketState  { IDLE, THRUSTING     };
constexpr float GRAVITATIONAL_ACCELERATION = 10.0f,
                THRUSTING_ACCELERATION   = 18.0f,
                HORIZONTAL_ACCELERATION = 12.0f,
                DRAG = 0.5f;


class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    Vector2 mScale;
    Vector2 mColliderDimensions;
    
    std::map<RocketState, Texture2D> mTextures;
    Texture2D mCurrentTexture;
    TextureType mTextureType;
    Vector2 mSpriteSheetDimensions;
    
    // std::map<Direction, std::vector<int>> mAnimationAtlas;
    std::map<RocketState, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;
    Direction mDirection;
    RocketState mRocketStatus;
    int mFrameSpeed;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    bool mIsJumping = false;
    float mJumpingPower = 0.0f;

    int mSpeed;
    float mAngle;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    bool mAcceleratingUp    = false;
    bool mAcceleratingDown  = false;
    bool mAcceleratingLeft  = false;
    bool mAcceleratingRight = false;

    EntityStatus mEntityStatus = ACTIVE;

    bool isColliding(Entity *other) const;
    void checkCollisionY(Entity *collidableEntities, int collisionCheckCount);
    void checkCollisionX(Entity *collidableEntities, int collisionCheckCount);
    void resetColliderFlags() 
    {
        mIsCollidingTop    = false;
        mIsCollidingBottom = false;
        mIsCollidingRight  = false;
        mIsCollidingLeft   = false;
    }

    void animate(float deltaTime);

public:
    static constexpr int   DEFAULT_SIZE          = 250;
    static constexpr int   DEFAULT_SPEED         = 200;
    static constexpr int   DEFAULT_FRAME_SPEED   = 12;
    static constexpr float Y_COLLISION_THRESHOLD = 0.5f;

    // check later
    static constexpr float ROTATION_SPEED = 120.0f;

    Entity();
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath);
    Entity(Vector2 position, Vector2 scale, const char *textureFilepath, 
        TextureType textureType, Vector2 spriteSheetDimensions, 
        std::map<Direction, std::vector<int>> animationAtlas);

    Entity(Vector2 position, Vector2 scale, std::vector<const char*> textureFilepaths, 
        TextureType textureType, Vector2 spriteSheetDimensions, 
        std::map<RocketState, std::vector<int>> animationAtlas);

    ~Entity();

    void update(float deltaTime, Entity *collidableEntities, int collisionCheckCount);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void jump()       { mIsJumping = true;  }
    void activate()   { mEntityStatus  = ACTIVE;   }
    void deactivate() { mEntityStatus  = INACTIVE; }
    void displayCollider();

    // check later
    void displayText();

    bool isActive() { return mEntityStatus == ACTIVE ? true : false; }

    // void moveUp()    { mMovement.y = -1; mDirection = UP;    }
    // void moveDown()  { mMovement.y =  1; mDirection = DOWN;  }
    // void moveLeft()  { mMovement.x = -1; mDirection = LEFT;  }
    // void moveRight() { mMovement.x =  1; mDirection = RIGHT; }

    

    void accelerateUp()    {
        // setAcceleration({ 0.0f, GRAVITATIONAL_ACCELERATION });
        // mAcceleration.y -= THRUSTING_ACCELERATION;
        // // setRocketState(THRUSTING);
        mAcceleratingUp = true;
        // mAcceleration.y = -10.0f;
        // setRocketState(THRUSTING);
     }
    void accelerateLeft()  {
        // setAcceleration({ 0.0f, GRAVITATIONAL_ACCELERATION });
        // mAcceleration.x -= HORIZONTAL_ACCELERATION;
        // mAcceleration.x = -5.0f;
        // setRocketState(THRUSTING); 
        mAcceleratingLeft = true;
    }
    void accelerateRight() {
        // setAcceleration({ 0.0f, GRAVITATIONAL_ACCELERATION });
        // mAcceleration.x += HORIZONTAL_ACCELERATION;
        // // setRocketState(THRUSTING); 
        mAcceleratingRight = true;
        // mAcceleration.x = 5.0f;
        // setRocketState(THRUSTING);

    }
    void accelerateDown()  {
        // if (mVelocity.x < 0.0f) DRAG *= -1.0f;
        // else DRAG = fabs(DRAG);
        // setAcceleration({0.0f, GRAVITATIONAL_ACCELERATION });
        // setRocketState(IDLE); 
        // mAcceleratingDown = true;
        // mAcceleration.y = 10.0f;
        // setRocketState(IDLE);
    }

    // check later
    // void rotate(float deltaAngle, float direction);

    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mScale;                 }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    std::map<RocketState, Texture2D> getTextures()        const { return mTextures;         }
    TextureType getTextureType()           const { return mTextureType;           }
    Direction   getDirection()             const { return mDirection;             }
    int         getFrameSpeed()            const { return mFrameSpeed;            }
    float       getJumpingPower()          const { return mJumpingPower;          }
    bool        isJumping()                const { return mIsJumping;             }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getAngle()                 const { return mAngle;                 }
    
    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }

    // std::map<Direction, std::vector<int>> getAnimationAtlas() const { return mAnimationAtlas; }
    std::map<RocketState, std::vector<int>> getAnimationAtlas() const { return mAnimationAtlas; }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setScale(Vector2 newScale)
        { mScale = newScale;                       }
    // void setTexture(const char *textureFilepath)
    //     { mTexture = LoadTexture(textureFilepath); }
    void setColliderDimensions(Vector2 newDimensions) 
        { mColliderDimensions = newDimensions;     }
    void setSpriteSheetDimensions(Vector2 newDimensions) 
        { mSpriteSheetDimensions = newDimensions;  }
    void setSpeed(int newSpeed)
        { mSpeed  = newSpeed;                      }
    void setFrameSpeed(int newSpeed)
        { mFrameSpeed = newSpeed;                  }
    void setJumpingPower(float newJumpingPower)
        { mJumpingPower = newJumpingPower;         }
    void setAngle(float newAngle) 
        { mAngle = newAngle;                       }

    // check later
    void setRocketState(RocketState newState);

};

#endif // ENTITY_CPP