/*
  LCDGameInstance.h
  Abstraction of a simple LCD Game mechanic.
  Objects will travel down distinct lanes one after another.
  The player may position themselves at the bottom of any lane with the goal of either catching or avoiding the objects.
  
  e.g. with 4 lanes, and length of 5 units for each object to travel down...
  | x | o | o | o |
  | o | o | o | o |
  | o | x | o | o |
  | o | o | x | o |
  | o | o | o | o |
  |   |   | ^ |   | // Player positioned in lane 3 currently.
*/
class LCDGameInstance
{
public:
  LCDGameInstance(const int laneLength, const int laneCount);

  void update(unsigned int timeElapsed, int (*randomFunc)(const int upper));
  int getCurrentLaneIdx() const;
  int getCurrentLaneSectionIdx() const;
  int getScore() const;
  int getPlayerLaneIdx() const;
  void setPlayerLaneIdx(const int playerLaneIdx);
  
private:
  const int mLaneLength;      // How long is a particular lane?
  const int mLaneCount;       // How many lanes?
  unsigned int mTimer;        // Time since last event
  
  int mCurrentLaneIdx;        // Which lane is currently selected?
  int mCurrentLaneSectionIdx; // Which section of the lane is currently active?
  int mPlayerLaneIdx;         // Which lane does the player currently reside in?
    
  int mScore;                 // What is the player's current score?
  const bool mAvoidObject;    // Player should avoid object, or try to catch it?
};

