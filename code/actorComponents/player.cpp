c_player::c_player(c_actor* father) {
    this -> father = father;
    key = 0;
    god = false;
    constitution = 5;
    agility = 5;
    spirit = 5;
    luck = 3;
    energy = 300;
    level = 1;
    carried = 0;
	equipment[0] = 0;
	equipment[1] = 0;
	equipment[2] = 0;
	equipment[3] = 0;
	equipment[4] = 0;
	equipment[5] = 0;
	equipment[6] = 0;

    // Learns basic abilities
    learnAbility("hit");
    learnAbility("kick");
    learnAbility("consume");
    learnAbility("equip");
    learnAbility("remove");
    learnAbility("get");
    learnAbility("drop");
    learnAbility("wait");
    learnAbility("speak");
    learnAbility("travel");
    learnAbility("walk");
}

bool c_player::channel(const int& key) {
    
    // * Pathfinding

    engine -> setLoading(false);
    if(engine -> interface.getTileDestination()) {
        int destX = engine -> interface.getTileDestination() -> getX();
        int destY = engine -> interface.getTileDestination() -> getY();
        if((destX == father -> getMapX()
        and destY == father -> getMapY()) 
        or engine -> interface.getTileDestination() -> isObstacle()) {
            engine -> interface.setTileDestination(0);

        // If the distance is 1 and it's a location
        // ! This is dirty as fuck and needs refactoring but hey man it works so good job bro
        } if(c_helper::calculateDistance(father -> getMapX(), father -> getMapY(), destX, destY) == 1
        and engine -> interface.getTileDestination() -> isLocation()) {
            structEventData eventData;
            eventData.type = "walk";
            eventData.mapX = destX;
            eventData.mapY = destY;
            return father -> action -> start(eventData); 
        } else {
            bool destWasObstacle = false;
            if(engine -> game -> map -> getTile(destX, destY) -> isObstacle()
            or engine -> game -> map -> getTile(destX, destY) -> isLocation()) {
                engine -> game -> map -> setProperties(destX, destY, true, true);
                destWasObstacle = true; 
            }
            TCODPath* path = engine -> game -> map -> path(father -> getMapX(), father -> getMapY(), destX, destY);
            if(path -> size()) {
                int dx = 0;
                int dy = 0;
                path -> walk(&dx, &dy, true);
                structEventData eventData;
                eventData.type = "walk";
                eventData.mapX = dx;
                eventData.mapY = dy;
                father -> action -> start(eventData); 
                engine -> setLoading(true);
                return true;
            } else {
                engine -> interface.setTileDestination(0);
                
            }
            if(destWasObstacle == true) {
                engine -> game -> map -> setProperties(destX, destY, false, false);
            }
            delete path;
        }
        return false;
    }

    // We first do directional movement since it works in both regular and world maps
	if(key) {

        // Walk (possible in regular and world map)
        switch(key) {

            case key::up: {

                int x = father -> getMapX();
                int y = father -> getMapY();

                // Out limits
                if(y == 0) {
                    engine -> game -> gamelog.message("That's out limits!");
                    return false;
                }

                return engine -> game -> map -> getTile(x, y - 1) -> playerAction(father);
            }

            case key::down: {

                int x = father -> getMapX();
                int y = father -> getMapY();

                // Out limits
                if(y > engine -> game -> map -> getHeight()) {
                    engine -> game -> gamelog.message("That's out limits!");
                    return false;
                }

                return engine -> game -> map -> getTile(x, y + 1) -> playerAction(father);
            }

            case key::right: {

                int x = father -> getMapX();
                int y = father -> getMapY();

                // Out limits
                if(x > engine -> game -> map -> getWidth()) {
                    engine -> game -> gamelog.message("That's out limits!");
                    return false;
                }

                return engine -> game -> map -> getTile(x + 1, y) -> playerAction(father);
            }

            case key::left: {

                int x = father -> getMapX();
                int y = father -> getMapY();

                // Out limits
                if(x == 0) {
                    engine -> game -> gamelog.message("That's out limits!");
                    return false;
                }

                return engine -> game -> map -> getTile(x - 1, y) -> playerAction(father);
            }
        }
    }     

    // World map input
    if(c_helper::isWorldMap()) {
        switch(key) {  
            // Explore current tile in local map
            case key::e: {
                c_helper::changeMap(father -> getMapX(), father -> getMapY(), 0);
                return true;
            }            
        }
    
    // Regular map input
    } else {
        switch(key) {  

            // Speak
            case key::s: {

                std::vector<int> actorList = engine -> game -> map -> countActorsAround(father -> getMapX(), father -> getMapY());
                std::vector<int> creatureList;

                if(actorList.size() > 0) {
                    for(int i = 0; i < actorList.size(); ++i) {
                        if(engine -> game -> actorManager.getActor(actorList[i]) -> life) {
                            creatureList.push_back(actorList[i]);
                        }
                    }
                }

                if(creatureList.size() > 0) {
                    // Only one creature
                    if(creatureList.size() == 1) {
                        structEventData eventData;
                        eventData.type = "speak";
                        eventData.target = creatureList[0];
                        father -> action -> start(eventData);
                        return true;
                    // Several creatures
                    } else {
                        structEventData eventData;
                        eventData.type = "speak";
                        eventData.target = engine -> interface.selectCloseTarget(imode::game, "Speak with", actorType::creature);
                        father -> action -> start(eventData);
                        return true;
                    }
                }

                engine -> game -> gamelog.message("There is nobody there.");
                return false;
            }

            // Kick
            case key::k: {

                std::vector<int> actorList = engine -> game -> map -> countActorsAround(father -> getMapX(), father -> getMapY());

                if(actorList.size() > 0) {
                    // Only one actor
                    if(actorList.size() == 1) {
                        structEventData eventData;
                        eventData.type = "kick";
                        eventData.target = actorList[0];
                        father -> action -> start(eventData);
                        return true;
                    // Several actors
                    } else {
                        structEventData eventData;
                        eventData.type = "kick";
                        eventData.target = engine -> interface.selectCloseTarget(imode::game, "Kick what?");
                        father -> action -> start(eventData);
                        return true;
                    }
                }

                engine -> game -> gamelog.message("You kick the air around you.");
                return false;

                /*int actor = engine -> game -> map -> getActorInRange(father -> getMapX(), father -> getMapY());

                if(actor == 0) {
                    engine -> game -> gamelog.message("There is nothing to kick.");
                    return false;
                } else {
                    structEventData eventData;
                    eventData.type = "kick";
                    eventData.target = actor;
                    father -> action -> start(eventData);
                    return true;
                }*/
            }

            // Travel (go to the world map)
            case key::t: {
                c_helper::changeMap(50, 0, 0);
                return true;
            }  

            // Wait
            case key::period: {
                structEventData eventData;
                eventData.type = "wait";
                father -> action -> start(eventData);
                return true;
            }         

            // Get
            case key::comma: {
                std::vector<int> actorList = engine -> game -> map -> getTile(father -> getMapX(), father -> getMapY()) -> getActorList();
                bool actor = false;
                for(int i = 0; i < actorList.size(); ++i) {
                    if(engine -> game -> actorManager.getActor(actorList[i]) -> body and engine -> game -> actorManager.getActor(actorList[i]) -> body -> getCanPickup() == true) {
                        actor = true;
                    }
                }
                if(actor == true) {
                    structEventData eventData;
                    eventData.type = "get";
                    eventData.target = actorList[0];
                    father -> action -> start(eventData);
                    return true;                
                } else {
                    engine -> game -> gamelog.message("There is nothing to pick up.");
                    return false;
                }
            }
        }
    }
    return false;    
}

const int& c_player::getMaxEnergy() {
	return (constitution + agility + spirit) * 10;
}

void c_player::consumeEnergy(const int& points) {
    energy -= points;
    if(energy < 0) {
        energy = 0;
    }
}

const bool& c_player::addToInventory(const int& uid, const int& quantity) {

	c_actor* p_item = engine -> game -> actorManager.getActor(uid);

	// Check if another item of the same type is already in the inventory
	for(int i = 0; i < inventory.size(); ++i) {
		if(p_item -> getId() == engine -> game -> actorManager.getActor(inventory[i].uid) -> getId()) {

			// It is, we increment quantity of existing item and remove the one to add
			inventory[i].quantity += quantity;
			if(p_item -> body) {
				carried += p_item -> body -> getMass();
			}			
			engine -> game -> actorManager.deleteActor(uid);
			return true;
		}
	}

	// No other item of the same type is in the inventory, so we add the current one
	s_invItem item;
	item.uid = uid;
	item.quantity = quantity;
	item.equipped = false;
	inventory.push_back(item);
	if(p_item -> body) {
		carried += p_item -> body -> getMass();
	}	
	return false;
}

// Returns true if it was the last item of its type that existed
const bool& c_player::deleteFromInventory(const int& item) {
	c_actor* p_item = engine -> game -> actorManager.getActor(item);
	for(int i = 0; i < inventory.size(); ++i) {
		if(p_item -> getId() == engine -> game -> actorManager.getActor(inventory[i].uid) -> getId()) {
			if(inventory[i].quantity > 1) {
				--inventory[i].quantity;
				return false;
			} else {
				inventory.erase(inventory.begin() + i);
				return true;
			}
		}
	}
	return false;
}

const bool& c_player::equipItem(const int& item) {
	for(int i = 0; i < inventory.size(); ++i) {
		if(inventory[i].uid == item) {
			c_actor* p_item = engine -> game -> actorManager.getActor(item);
			if(p_item -> weapon) {
				equipment[bodySlot::mainHand] = item;
				inventory[i].equipped = true;
				return true;
			}
		}
	}
	return false;
}

const bool& c_player::removeItem(const int& item) {
	for(int i = 0; i < inventory.size(); ++i) {
		if(inventory[i].uid == item) {
			c_actor* p_item = engine -> game -> actorManager.getActor(item);
			if(p_item -> weapon) {
				equipment[bodySlot::mainHand] = 0;
				inventory[i].equipped = false;
				return true;
			}
		}
	}
	return false;
}

const bool& c_player::isInInventory(const int& item) {
	for(int i = 0; i < inventory.size(); ++i) {
		if(inventory[i].uid == item) {
			return true;
		}
	}
	return false;
}

// Adds the ability to the ability memory
const bool& c_player::learnAbility(std::string id, const bool& verbose) {

    // First checks if the ability exists
    s_abilityAsset* ability = engine -> assetManager.getAbilityAsset(id);
    if(ability -> duration == 0) {
        c_helper::message("That word does not exist!");
        return false;
    }

    // Adds the ability to the ability memory
    v_learnedAbilitys.push_back(id);
    if(verbose == true) {
        c_helper::message("You have learned the word '" + id + "'.");
    }
    return true;
}

const bool& c_player::hasAbility(std::string id) {
    for(int i = 0; i < v_learnedAbilitys.size(); ++i) {
        if(v_learnedAbilitys.at(i) == id) {
            return true;
        }
    }
    return false;
}