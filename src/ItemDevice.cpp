#include "ItemDevice.h"

#include "Engine.h"
#include "ActorPlayer.h"
#include "Render.h"
#include "GameTime.h"
#include "Log.h"

//---------------------------------------------------- BASE CLASS
bool Device::toggle(Engine* const engine) {
  if(isActivated_) {
    isActivated_ = false;
    nrTurnsToNextGoodEffect_ = nrTurnsToNextBadEffect_ = -1;
  } else {
    isActivated_ = true;
    nrTurnsToNextGoodEffect_ = getRandomNrTurnsToNextEffect(engine);
    nrTurnsToNextBadEffect_ = getRandomNrTurnsToNextEffect(engine);
  }
  string str = "Device ";
  str += isActivated_ ? "activates" : "deactivates";
  engine->log->addMessage(str);
  specificToggle(engine);
  return false;
}

int Device::getRandomNrTurnsToNextEffect(Engine* const engine) const {
  return engine->dice.getInRange(6, 10);
}

void Device::newTurn(Engine* const engine) {
  if(isActivated_) {
    if(--nrTurnsToNextGoodEffect_ <= 0) {
      runGoodEffect(engine);
      nrTurnsToNextGoodEffect_ = getRandomNrTurnsToNextEffect(engine);
    }
    if(--nrTurnsToNextBadEffect_ <= 0) {
      runBadEffect(engine);
      nrTurnsToNextBadEffect_ = getRandomNrTurnsToNextEffect(engine);
    }
  }
}

void Device::runBadEffect(Engine* const engine) {

}

//---------------------------------------------------- SENTRY DEVICE
void DeviceSentry::runGoodEffect(Engine* const engine) {
  const int DMG = engine->dice(1, 6) + 2;

  engine->player->getSpotedEnemies();
  vector<Actor*>& targetCandidates = engine->player->spotedEnemies;
  const unsigned int NR_TARGET_CANDIDATES = targetCandidates.size();
  if(NR_TARGET_CANDIDATES > 0) {
    Actor* const actor = targetCandidates.at(engine->dice.getInRange(0, NR_TARGET_CANDIDATES - 1));
    const coord& pos = actor->pos;
    engine->log->addMessage(actor->getNameThe() + " is hit by a bolt of lightning!", clrMessageGood, messageInterrupt_force);
    engine->renderer->drawBlastAnimationAtPositionsWithPlayerVision(vector<coord>(1, pos), clrYellow, 1, engine);
    actor->hit(DMG, damageType_electric);
  }
}

//---------------------------------------------------- REPELLER DEVICE
void DeviceRepeller::runGoodEffect(Engine* const engine) {

}

//---------------------------------------------------- REJUVENATOR DEVICE
void DeviceRejuvenator::runGoodEffect(Engine* const engine) {

}

//---------------------------------------------------- TRANSLOCATOR DEVICE
void DeviceTranslocator::runGoodEffect(Engine* const engine) {

}

//---------------------------------------------------- ELECTRIC LANTERN
void DeviceElectricLantern::runGoodEffect(Engine* const engine) {
  (void)engine;
}

void DeviceElectricLantern::specificToggle(Engine* const engine) {
  engine->gameTime->updateLightMap();
  engine->player->FOVupdate();
  engine->renderer->drawMapAndInterface();
}

bool DeviceElectricLantern::isGivingLight() const {
  return isActivated_;
}

void DeviceElectricLantern::runBadEffect(Engine* const engine) {

}

