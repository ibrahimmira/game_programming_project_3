#ifndef ENTITY_H
#define ENTITY_H

#include "cs3113.h"

enum EntityStatus       { ACTIVE, INACTIVE        };
enum RocketState        { IDLE, THRUSTING         };
enum EntityType         { ROCKET, FIXED_LANDING_PAD, MOVING_LANDING_PAD };
enum GameOverReason     { OUT_OF_BOUNDS, OUT_OF_FUEL, LANDED_SUCCESSFULLY, CRASHED };
constexpr float         GRAVITATIONAL_ACCELERATION = 10.0f,
                        THRUSTING_ACCELERATION   = 18.0f,
                        HORIZONTAL_ACCELERATION = 12.0f,
                        DRAG_CONSTANT = 0.5f;


class Entity
{
private:
    Vector2 mPosition;
    Vector2 mMovement;
    Vector2 mVelocity;
    Vector2 mAcceleration;

    float mStartingXPosition;

    Vector2 mScale;
    Vector2 mColliderDimensions;
    
    std::map<RocketState, Texture2D> mTextures;
    Texture2D mCurrentTexture;
    TextureType mTextureType;
    Vector2 mSpriteSheetDimensions;
    
    std::map<RocketState, std::vector<int>> mAnimationAtlas;
    std::vector<int> mAnimationIndices;

    RocketState mRocketStatus;
    EntityType mEntityType;
    int mFrameSpeed;

    int mCurrentFrameIndex = 0;
    float mAnimationTime = 0.0f;

    bool mIsJumping = false;
    float mJumpingPower = 0.0f;

    int mSpeed;
    float mAngle;

    float mFuelTank = 100.0f;

    bool mIsCollidingTop    = false;
    bool mIsCollidingBottom = false;
    bool mIsCollidingRight  = false;
    bool mIsCollidingLeft   = false;

    bool mAcceleratingUp    = false;
    bool mAcceleratingDown  = false;
    bool mAcceleratingLeft  = false;
    bool mAcceleratingRight = false;

    bool mIsGameOver        = false;
    GameOverReason mGameOverReason;

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

    Entity();

    Entity(Vector2 position, Vector2 scale, const char *textureFilepath, EntityType entityType);

    Entity(Vector2 position, Vector2 scale, std::vector<const char*> textureFilepaths, 
        TextureType textureType, Vector2 spriteSheetDimensions, 
        std::map<RocketState, std::vector<int>> animationAtlas, EntityType entityType);

    ~Entity();

    void update(float deltaTime, Entity *collidableEntities[], int collisionCheckCount);
    void update(float deltaTime);
    void render();
    void normaliseMovement() { Normalise(&mMovement); }

    void jump()       { mIsJumping = true;  }
    void activate()   { mEntityStatus  = ACTIVE;   }
    void deactivate() { mEntityStatus  = INACTIVE; }
    void displayCollider();

    void displayStats();

    bool isActive() { return mEntityStatus == ACTIVE ? true : false; }

    void accelerateUp()    {
        
        if (mFuelTank > 0.0f && !mIsGameOver) {
            mFuelTank -= 0.001f;
            mAcceleratingUp = true;
        } 
       
     }
    void accelerateLeft()  {
        
        if (mFuelTank > 0.0f && !mIsGameOver) {
            mFuelTank -= 0.001f;
            mAcceleratingLeft = true;
        } 

    }
    void accelerateRight() {

        if (mFuelTank > 0.0f && !mIsGameOver) {
            mFuelTank -= 0.001f;
            mAcceleratingRight = true;
        } 

    }

    void applyDrag(float deltaTime) {

        float currDrag = -mVelocity.x * DRAG_CONSTANT;     
        float stoppingAccel = -mVelocity.x / deltaTime;                         
        if (fabsf(currDrag) > fabsf(stoppingAccel)) currDrag = stoppingAccel;   
        mAcceleration.x += currDrag;

    }

    void resetMovement() { mMovement = { 0.0f, 0.0f }; }

    void resetAcceleration() { setAcceleration({ 0.0f, GRAVITATIONAL_ACCELERATION }); }

    Vector2     getPosition()              const { return mPosition;              }
    Vector2     getMovement()              const { return mMovement;              }
    Vector2     getVelocity()              const { return mVelocity;              }
    Vector2     getAcceleration()          const { return mAcceleration;          }
    Vector2     getScale()                 const { return mScale;                 }
    Vector2     getColliderDimensions()    const { return mScale;                 }
    Vector2     getSpriteSheetDimensions() const { return mSpriteSheetDimensions; }
    std::map<RocketState, Texture2D> getTextures()        const { return mTextures;         }
    TextureType getTextureType()           const { return mTextureType;           }

    int         getFrameSpeed()            const { return mFrameSpeed;            }
    float       getJumpingPower()          const { return mJumpingPower;          }
    bool        isJumping()                const { return mIsJumping;             }
    int         getSpeed()                 const { return mSpeed;                 }
    float       getAngle()                 const { return mAngle;                 }

    EntityType  getEntityType()           const { return mEntityType;            }
    
    bool isCollidingTop()    const { return mIsCollidingTop;    }
    bool isCollidingBottom() const { return mIsCollidingBottom; }

    std::map<RocketState, std::vector<int>> getAnimationAtlas() const { return mAnimationAtlas; }

    void setPosition(Vector2 newPosition)
        { mPosition = newPosition;                 }
    void setMovement(Vector2 newMovement)
        { mMovement = newMovement;                 }
    void setAcceleration(Vector2 newAcceleration)
        { mAcceleration = newAcceleration;         }
    void setScale(Vector2 newScale)
        { mScale = newScale;                       }
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
    void setRocketState(RocketState newState);
    void setGameOver() { mIsGameOver = true; };
    
    void gameOver();

};

#endif // ENTITY_CPP