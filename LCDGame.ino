#include "LCDGameInstance.h"

/*
  LCDGame
  Provides the Arduino specific implementation for visualisation of the LCD Game logic.
  This uses a potentiometer for input and digital writes to update LEDs representing the objects.
*/
const int leftLanePins[] = {13, 12, 11};
const int rightLanePins[] = {10, 9, 8};
const int playerPins[] = {6, 7};
const int* lanes[] = {leftLanePins, rightLanePins};
const int lanesLength = sizeof(leftLanePins) / sizeof(int);
const int lanesCount = sizeof(lanes) / sizeof(int*);

LCDGameInstance gameInstance(lanesLength, lanesCount);

// Pot range is 0-1023, giving a slight bias to avoid noise on the upper bound we can divide the pot's output into 
// the input segments for the player.
const int potSegmentSize = 1024 / lanesCount;
const int potPin = A0;
int potVal;

// Frame rate
const unsigned int timeBetweenTicks = 15;

void setup()
{
  // We need to ensure that all digiral pins are correctly initialised in order to light the LEDs properly.
  Serial.begin(9600);
  
  for(int i = 0; i < lanesLength; ++i)
  {
    pinMode(leftLanePins[i], OUTPUT);
    digitalWrite(leftLanePins[i], LOW);
  }
  for(int i = 0; i < lanesLength; ++i)
  {
    pinMode(rightLanePins[i], OUTPUT);
    digitalWrite(rightLanePins[i], LOW);
  }
  for(int i = 0; i < lanesCount; ++i)
  {
    pinMode(playerPins[i], OUTPUT);
    
    // To avoid extra branches later, assume that for now player is at lane 0
    digitalWrite(playerPins[i], (i == 0) ? HIGH : LOW);
  }
  
  // No connections on pin 0, this should seed somewhat randomly.
  randomSeed(analogRead(0));
}

// To present to LCDGameInstance as our Arduino-flavoured random func.
int nextRandom(const int upper)
{
  return random(upper);
}

void loop()
{
  const int previousLaneIdx = gameInstance.getCurrentLaneIdx();
  const int previousLaneSectionIdx = gameInstance.getCurrentLaneSectionIdx();
  
  // Tick main game logic
  gameInstance.update(timeBetweenTicks, &nextRandom);
  
  const int currentLaneIdx = gameInstance.getCurrentLaneIdx();
  const int currentLaneSectionIdx = gameInstance.getCurrentLaneSectionIdx();
  const int score = gameInstance.getScore();
  
  // Update visual, and serial logic for the objects and lane.
  if(previousLaneSectionIdx != currentLaneSectionIdx)
  {
    digitalWrite(lanes[previousLaneIdx][previousLaneSectionIdx], LOW);
    Serial.print("Lane is: ");
    Serial.print(currentLaneIdx);
    Serial.print(", Wave is: ");
    Serial.print(currentLaneSectionIdx);
    Serial.print(" Score is: ");
    Serial.print(score);
    Serial.print(", ");
  }
  digitalWrite(lanes[currentLaneIdx][currentLaneSectionIdx], HIGH);
    
  // Update player position, driven from pot for arduino implementation.
  potVal = analogRead(potPin);
  
  int previousPlayerLaneIdx = gameInstance.getPlayerLaneIdx();
  int playerLaneIdx = potVal / potSegmentSize;
  
  // Feed our player idx into simulation
  gameInstance.setPlayerLaneIdx(playerLaneIdx);
  
  if(playerLaneIdx != previousPlayerLaneIdx)
  {
    digitalWrite(playerPins[previousPlayerLaneIdx], LOW);
    digitalWrite(playerPins[playerLaneIdx], HIGH);
  }
  
  Serial.print("PotVal is: ");
  Serial.print(potVal);
  Serial.print(", PlayerLaneIdx is: ");
  Serial.println(playerLaneIdx);
  
  // We delay by the amount we provide to the update, thus the Arduino implementation can dictate its own framerate.
  delay(timeBetweenTicks);
}
  
