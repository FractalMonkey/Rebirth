state["runEvent"] = &c_game::runEvent;
state["random"] = &c_helper::random;
state["test"] = &c_helper::test;
state["message"] = &c_helper::gameMessage;
state["fullscreen"] = &c_helper::toggleFullScreen; // Changes between window and fullscreen mode
state["talk"] = &c_helper::talk; // Opens a talk window in the interface with several dialogue options

// Sound
state["playAmbience"] = &c_helper::playAmbience;

// Map
state["info"] = &c_helper::showMapInfo; 
state["saveMap"] = &c_helper::saveMap;
state["changeMap"] = &c_helper::changeMap;
state["worldMap"] = &c_helper::worldMap;
state["isWorldMap"] = &c_helper::isWorldMap;
state["resetMap"] = c_helper::resetMap;
state["addScript"] = &c_helper::createMapScript;
state["getMapSeed"] = &c_helper::getMapSeed;
state["getMapWidth"] = &c_helper::getMapWidth;
state["getMapHeight"] = &c_helper::getMapHeight;
state["getMapX"] = &c_helper::getMapX;
state["getMapY"] = &c_helper::getMapY;
state["getMapZ"] = &c_helper::getMapZ;
state["clear"] = &c_helper::genClear;
state["digRoom"] = &c_helper::genDigRoom;
state["genDungeon"] = &c_helper::genDungeon;
state["genCave"] = &c_helper::genCave;
state["genWorld"] = &c_helper::genWorld;
state["genAddCellularPatch"] = &c_helper::genAddCellularPatch;
state["genPlantTrees"] = &c_helper::genPlantTrees;
state["genPlaceActorSomewhere"] = &c_helper::genPlaceActorSomewhere;
state["setFloor1"] = &c_helper::setGenFloor1;
state["setFloor2"] = &c_helper::setGenFloor2;
state["setFloor3"] = &c_helper::setGenFloor3;
state["setWall1"] = &c_helper::setGenWall1;
state["setWall2"] = &c_helper::setGenWall2;
state["setWall3"] = &c_helper::setGenWall3;
state["setAmbience"] = &c_helper::setAmbience;
state["build"] = &c_helper::build;
state["forgetMap"] = &c_helper::forgetMap;
state["updateWorld"] = &c_helper::updateWorld; // Updates world map from map 0.0.0 stored locally

state["isObstacle"] = &c_helper::isObstacle;
state["createActor"] = &c_helper::createActor;
state["actorTypeInTile"] = &c_helper::actorTypeInTile;
state["getFirstActorInTile"] = &c_helper::getFirstActorInTile;
state["teleportActor"] = &c_helper::teleportActor;
state["startAction"] = &c_helper::startAction;
state["findEnemy"] = &c_helper::findEnemy;
state["isEnemy"] = &c_helper::isEnemy;
state["showActorPosition"] = &c_helper::showActorPosition; // Shows actor's coordinates in the game console
state["showPlayerPosition"] = &c_helper::showPlayerPosition; // Shows player's coordinates in the game console
state["getDirectionToActor"] = &c_helper::getDirectionToActor;
state["getActorId"] = &c_helper::getActorId; // Returns the actor's id
state["getName"] = &c_helper::getName; // Returns the actor's name
state["getPlayerName"] = &c_helper::getPlayerName; // Returns the actor's name
state["getPlayer"] = &c_helper::getPlayer; // Returns the player actor uid
state["getActorMapX"] = &c_helper::getActorMapX;
state["getActorMapY"] = &c_helper::getActorMapY;
state["getHealth"] = &c_helper::getHealth;
state["getMaxHealth"] = &c_helper::getMaxHealth;
state["getMinDamage"] = &c_helper::getMinDamage;
state["getMaxDamage"] = &c_helper::getMaxDamage;
state["getAccuracy"] = &c_helper::getAccuracy;
state["getDodge"] = &c_helper::getDodge;
state["getViewRange"] = &c_helper::getViewRange;
state["calculateDistance"] = &c_helper::calculateDistance;
state["isPlayer"] = &c_helper::isPlayer;
state["restoreHealth"] = &c_helper::restoreHealth;
state["damage"] = &c_helper::damage;
state["kill"] = &c_helper::kill;
state["openCloseDoor"] = &c_helper::openCloseDoor;
state["getItemFromFloor"] = &c_helper::getItemFromFloor;
state["dropItemFromInventory"] = &c_helper::dropItemFromInventory;
state["consume"] = &c_helper::consume;
state["findStaircase"] = &c_helper::findStaircase;
state["equipItem"] = &c_helper::equipItem;
state["removeItem"] = &c_helper::removeItem;   
state["give"] = &c_helper::give;   
state["getConstitution"] = &c_helper::getConstitution;
state["setAttributes"] = &c_helper::setAttributes;
state["god"] = &c_helper::toggleGodMode;
state["isPlayerGod"] = &c_helper::isPlayerGod;
state["isLocation"] = &c_helper::isLocation;
state["learn"] = &c_helper::learn; // Teaches the player a skill
state["findActorByName"] = &c_helper::findActorByName;
state["findTileByName"] = &c_helper::findTileByName; // Returns true if the tile in location has the same name as parameter
state["getBiome"] = &c_helper::getBiome; // Returns the biome of the selected location of the world map