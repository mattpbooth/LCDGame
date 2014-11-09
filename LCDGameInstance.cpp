#include "LCDGameInstance.h"

const unsigned int kTimeUntilNextMovement = 500;

LCDGameInstance::LCDGameInstance(const int laneLength, const int laneCount)
  : mLaneLength(laneLength)
  , mLaneCount(laneCount)
  , mTimer(0)
  , mCurrentLaneIdx(0)
  , mPlayerLaneIdx(0)
  , mScore(0)
  , mAvoidObject(false)
{
}

void LCDGameInstance::update(const unsigned int timeElapsed, int (*randomFunc)(const int upper))
{
  mTimer += timeElapsed;
  if(mTimer >= kTimeUntilNextMovement)
  {
    mTimer = 0;
    ++mCurrentLaneSectionIdx;
    
    // If we're at the end of the lane
    if(mCurrentLaneSectionIdx >= mLaneLength)
    {
      // Adjust score, based on whether player should be allowed or prevented from intersection with object
      if((mCurrentLaneIdx == mPlayerLaneIdx && !mAvoidObject) ||
         (mCurrentLaneIdx != mPlayerLaneIdx && mAvoidObject))
      {
        ++mScore;
      }
      
      mCurrentLaneIdx = randomFunc(mLaneCount);
      mCurrentLaneSectionIdx = 0;
    }
  }
}

int LCDGameInstance::getCurrentLaneIdx() const
{
  return mCurrentLaneIdx;
}

int LCDGameInstance::getCurrentLaneSectionIdx() const
{
  return mCurrentLaneSectionIdx;
}

int LCDGameInstance::getScore() const
{
  return mScore;
}

int LCDGameInstance::getPlayerLaneIdx() const
{
  return mPlayerLaneIdx;
}

void LCDGameInstance::setPlayerLaneIdx(const int playerLaneIdx)
{
  mPlayerLaneIdx = playerLaneIdx;
}




