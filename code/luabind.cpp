state["runEvent"] = &c_game::runEvent;
state["random"] = &c_helper::random;
state["test"] = &c_helper::test;
state["message"] = &c_helper::gameMessage;
state["fullscreen"] = &c_helper::toggleFullScreen;

// Map
state["saveMap"] = &c_helper::saveMap;
state["changeMap"] = &c_helper::changeMap;
state["worldMap"] = &c_helper::worldMap;
state["resetMap"] = c_helper::resetMap;
state["addScript"] = &c_helper::createMapScript;
state["getMapSeed"] = &c_helper::getMapSeed;
state["getMapWidth"] = &c_helper::getMapWidth;
state["getMapHeight"] = &c_helper::getMapHeight;
state["getMapX"] = &c_helper::getMapX;
state["getMapY"] = &c_helper::getMapY;
state["clear"] = &c_helper::genClear;
state["digRoom"] = &c_helper::genDigRoom;
state["genCastle"] = &c_helper::genCastle;
state["setFloor1"] = &c_helper::setGenFloor1;
state["setFloor2"] = &c_helper::setGenFloor2;
state["setFloor3"] = &c_helper::setGenFloor3;
state["setWall1"] = &c_helper::setGenWall1;
state["setWall2"] = &c_helper::setGenWall2;
state["setWall3"] = &c_helper::setGenWall3;
state["build"] = &c_helper::build;

state["isObstacle"] = &c_helper::isObstacle;
state["createActor"] = &c_helper::createActor;
state["actorTypeInTile"] = &c_helper::actorTypeInTile;
state["getFirstActorInTile"] = &c_helper::getFirstActorInTile;
state["teleportActor"] = &c_helper::teleportActor;
state["startAction"] = &c_helper::startAction;
state["findEnemy"] = &c_helper::findEnemy;
state["isEnemy"] = &c_helper::isEnemy;
state["showActorPosition"] = &c_helper::showActorPosition;
state["showPlayerPosition"] = &c_helper::showPlayerPosition;
state["getDirectionToActor"] = &c_helper::getDirectionToActor;
state["getActorId"] = &c_helper::getActorId;
state["getName"] = &c_helper::getName;
state["getActorMapX"] = &c_helper::getActorMapX;
state["getActorMapY"] = &c_helper::getActorMapY;
state["getPlayerName"] = &c_helper::getPlayerName;
state["getHealth"] = &c_helper::getHealth;
state["getViewRange"] = &c_helper::getViewRange;
state["calculateDistance"] = &c_helper::calculateDistance;
state["getPlayer"] = &c_helper::getPlayer;
state["isPlayer"] = &c_helper::isPlayer;
state["restoreHealth"] = &c_helper::restoreHealth;
state["damage"] = &c_helper::damage;
state["kill"] = &c_helper::kill;
state["openCloseDoor"] = &c_helper::openCloseDoor;
state["getItemFromFloor"] = &c_helper::getItemFromFloor;
state["consume"] = &c_helper::consume;
state["findStaircase"] = &c_helper::findStaircase;
state["equipItem"] = &c_helper::equipItem;
state["removeItem"] = &c_helper::removeItem;   
state["give"] = &c_helper::give;   
state["getConstitution"] = &c_helper::getConstitution;
state["getMeleeDamage"] = &c_helper::getMeleeDamage;
state["god"] = &c_helper::toggleGodMode;
state["isPlayerGod"] = &c_helper::isPlayerGod;