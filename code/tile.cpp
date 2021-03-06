c_tile::c_tile()
:   x(0),
    y(0),
    id("void"),
    name("Void"),
    desc("Void"),
    tileX(0),
    tileY(0),
    type(tileType::floor),
    script(0),
    interior(false),
    explored(false),
    visible(false),
    check(false),
    tickCounter(0) {
    engine -> game -> map -> setProperties(x, y, false, false);
}

c_tile::~c_tile() {
}

void c_tile::draw(const int& x, const int& y, const bool& playerIsInside, const bool& fog) {

    if(fog == false) {
        engine -> screen.drawTile(11, 13, x, y, bgcolor);
        //drawOverlay(x, y, type, olcolor);
        if(v_actor.size() > 0) {
            c_actor* p_actor = engine -> game -> actorManager.getActor(v_actor[0]);
            //if(p_actor != engine -> game -> actorManager.getPlayer()) {
                drawShadow(x, y);
                engine -> screen.drawTile(p_actor -> getTileX(), p_actor -> getTileY(), x, y, p_actor -> getColor());
            //}
        } else {
            if(type == tileType::world) { 
                engine -> screen.drawTile(tileX, tileY, x + 1, y + 1, sf::Color::Black);
            }
            engine -> screen.drawTile(tileX, tileY, x, y, color);
            drawShadow(x, y);
        }
        if(type == tileType::wall) {
            engine -> screen.drawTexture("wallmark", x, y);
        }

        // Transition mark
        //drawTransitionMark(x, y);
        //engine -> screen.drawTexture("fog-night", x, y);
        return;
    }
    
    // Out of view
    if(visible == false) {
        if(explored == true) {

            if(interior == true
            and type != tileType::wall
            and (interior == false and playerIsInside == true) or (interior == true and playerIsInside == false)) {

                //engine -> screen.drawTexture("background_main", x, y, finalLight);
                //engine -> screen.drawTexture("fog-night", x, y);
                return;
            }
            
            // Print tile with fog
            engine -> screen.drawTile(11, 13, x, y, bgcolor);
            //drawOverlay(x, y, type, olcolor);
            drawShadow(x, y);

            // Draw non-living actor
            if(v_actor.size() > 0 and !engine -> game -> actorManager.getActor(v_actor[0]) -> life) {
                engine -> game -> actorManager.getActor(v_actor[0]) -> draw(x, y);
            } else {
                engine -> screen.drawTile(tileX, tileY, x, y, color);
            }

            engine -> screen.drawTexture("fog", x, y);

            // Wall mark
            if(type == tileType::wall) {
                engine -> screen.drawTexture("wallmark", x, y);
            }

            // Transition mark
            //drawTransitionMark(x, y);
        }
        //engine -> screen.drawTexture("fog-night", x, y);
        return;                
    }

    // Draw actor
    if(v_actor.size() > 0) {
        drawShadow(x, y);
        if(v_actor.size() == 1) {
            engine -> screen.drawTile(11, 13, x, y, bgcolor);
            //drawOverlay(x, y, type, olcolor);
            drawShadow(x, y);
            engine -> game -> actorManager.getActor(v_actor[0]) -> draw(x, y);
            //engine -> screen.drawTexture("fog-night", x, y);
            return;
        }
        for(int i = 0; i < v_actor.size(); ++i) {
            c_actor* p_actor = engine -> game -> actorManager.getActor(v_actor[i]);
            if(p_actor -> life) {
                engine -> screen.drawTile(11, 13, x, y, bgcolor);
                drawShadow(x, y);
                p_actor -> draw(x, y);
                return;
            }
        }
        // We cycle across all actors, changing frame every X ticks
        const int ticks = 50;
        ++tickCounter;
        if(tickCounter > ticks) {
            tickCounter = 0;
            ++frameIndex;
            if(frameIndex == v_actor.size()) {
                frameIndex = 0;
            }
        }
        c_actor* p_actor = engine -> game -> actorManager.getActor(v_actor[frameIndex]);
        engine -> screen.drawTile(11, 13, x, y, bgcolor);
        drawShadow(x, y);
        p_actor -> draw(x, y);
        return;
    }

    // Otherwise draw tile terrain
    engine -> screen.drawTile(11, 13, x, y, bgcolor);
    //drawOverlay(x, y, type, olcolor);
    //if(type == tileType::world) { 
    //    engine -> screen.drawTile(tileX, tileY, x + 1, y + 1, sf::Color::Black);
    //}
    engine -> screen.drawTile(tileX, tileY, x, y, color);
    drawShadow(x, y);

    // Wall mark
    if(type == tileType::wall) {
        engine -> screen.drawTexture("wallmark", x, y);
    }

    // Transition mark
    //drawTransitionMark(x, y);

    //engine -> screen.drawTexture("fog-night", x, y);
}

/*void c_tile::drawActors(const int& x, const int& y) {
    if(v_actor.size() > 0) {
        engine -> game -> actorManager.getActor(v_actor[0]) -> draw(x, y);
    }
}*/

bool c_tile::playerAction(c_actor* p_player) {

    if(p_player -> action -> isRunning()) {
        return false;
    }

    // Tile blocks movement
    if(p_player -> isGod() == false and type == tileType::wall) {
        engine -> game -> gamelog.message("A " + name + " blocks your way.");
        return false;
    }
    
    // Actor
    bool playerAction = false;
    if(v_actor.size() > 0) {
        for(int i = 0; i < v_actor.size(); ++i) {
            playerAction = engine -> game -> actorManager.getActor(v_actor[i]) -> playerAction(true, p_player);
        }                    
    }
    if(playerAction == true) {
        return true;
    }

    structEventData eventData;
    eventData.type = "walk";
    eventData.mapX = x;
    eventData.mapY = y;
    p_player -> action -> start(eventData);
    return true;
}

void c_tile::drawShadow(const int& x, const int& y) {
    
    if(type != tileType::wall and this -> x > 0 and this -> y > 0 and this -> x < engine -> game -> map -> getWidth() and this -> y < engine -> game -> map -> getHeight()) {
        /* if(type == tileType::water) {
            if(engine -> game -> map -> getTile(this -> x, this -> y - 1) -> getType() != tileType::water) {
                if(engine -> game -> map -> getTile(this -> x - 1, this -> y) -> getType() != tileType::water) {
                    engine -> screen.drawTexture("tileShadowFull", x, y);
                } else {
                    engine -> screen.drawTexture("tileShadowBottom", x, y);
                }
            } else {
                if(engine -> game -> map -> getTile(this -> x - 1, this -> y) -> getType() != tileType::water) {
                    engine -> screen.drawTexture("tileShadowRight", x, y);
                } else {
                    if(engine -> game -> map -> getTile(this -> x - 1, this -> y - 1) -> getType() != tileType::water) {
                        engine -> screen.drawTexture("tileShadowBottomRight", x, y);
                    }
                }
            }
        } else*/ if(engine -> game -> map -> getTile(this -> x, this -> y - 1) -> getType() == tileType::wall) {
            if(engine -> game -> map -> getTile(this -> x - 1, this -> y) -> getType() == tileType::wall) {
                engine -> screen.drawTexture("tileShadowFull", x, y);
            } else {
                engine -> screen.drawTexture("tileShadowBottom", x, y);
            }
        } else {
            if(engine -> game -> map -> getTile(this -> x - 1, this -> y) -> getType() == tileType::wall) {
                engine -> screen.drawTexture("tileShadowRight", x, y);
            } else {
                if(engine -> game -> map -> getTile(this -> x - 1, this -> y - 1) -> getType() == tileType::wall) {
                    engine -> screen.drawTexture("tileShadowBottomRight", x, y);
                }
            }
        }
    }    
}

bool c_tile::isObstacle() {
    if(type == tileType::wall or type == tileType::obstacle) {
        return true;
    }
    if(v_actor.size() > 0) {
        for(int i = 0; i < v_actor.size(); ++i) {
            c_actor* p_actor = engine -> game -> actorManager.getActor(v_actor[i]);
            if(p_actor -> body and p_actor -> body -> getCanMove() == false) {
                return true;
            }
        }
    }
    return false;
}

bool c_tile::isLocation() {
    if(v_actor.size() > 0) {
        for(int i = 0; i < v_actor.size(); ++i) {
            c_actor* p_actor = engine -> game -> actorManager.getActor(v_actor[i]);
            if(p_actor -> getType() == actorType::location) {
                return true;
            }
        }
    }
    return false;
}

void c_tile::updateObstacle() {
    if(isObstacle() or isLocation()) {
        engine -> game -> map -> setProperties(x, y, false, false);
    } else {
        engine -> game -> map -> setProperties(x, y, true, true);
    }    
}

void c_tile::explore() {
    explored = true;
    updateObstacle();
}

const bool& c_tile::blocksView() {
    if(v_actor.size() > 0) {
        for(int i = 0; i < v_actor.size(); ++i) {
            if(engine -> game -> actorManager.getActor(v_actor[i]) -> body
            and !engine -> game -> actorManager.getActor(v_actor[i]) -> body -> getCanView()) {
                return true;
            }
        }
    }
    if(type == tileType::wall or type == tileType::obstacle or id == "world_mountain") {
        return true;
    }
    return false;
}

void c_tile::stainBlood() {
    color = m_color["darker red"];
}

/*void c_tile::drawTransitionMark(const int& x, const int& y) {
    if(this -> y == 0) {
        engine -> screen.drawTexture("travelNorth", x, y);
        return;
    } else if(this -> y == engine -> game -> map -> getHeight() - 1) {
        engine -> screen.drawTexture("travelSouth", x, y);
        return;
    } else if(this -> x == engine -> game -> map -> getWidth() - 1) {
        engine -> screen.drawTexture("travelEast", x, y);
        return;
    } else if(this -> x == 0) {
        engine -> screen.drawTexture("travelWest", x, y);
        return;
    }    
}*/

void c_tile::addActor(const int& actor) {
    v_actor.push_back(actor);
    c_actor* p_actor = engine -> game -> actorManager.getActor(actor);
    if((p_actor -> body and p_actor -> body -> getCanMove() == false)
    or (p_actor -> getType() == actorType::location)) {
        engine -> game -> map -> setProperties(x, y, false, false);
    }
}

const bool& c_tile::removeActor(const int& actor) {
    for(int i = 0; i < static_cast<int>(v_actor.size()); ++i) {
        if(v_actor[i] == actor) {
            v_actor.erase(v_actor.begin() + i);
            if(isObstacle() or isLocation()) { //  or explored == false (removed after having actors using fov)
                engine -> game -> map -> setProperties(x, y, false, false);
            } else {
                engine -> game -> map -> setProperties(x, y, true, true);
            }
            return true;
        }
    }
    return false;
}

void c_tile::removeActors(const bool& savePlayer) {
    std::vector<int> actors = v_actor;
    for(int i = 0; i < actors.size(); ++i) {
        if(engine -> game -> actorManager.getActor(actors[i]) == engine -> game -> actorManager.getPlayer()) {
            if(savePlayer == false) {
                engine -> game -> actorManager.deleteActor(actors[i]);
            }
        } else {
            engine -> game -> actorManager.deleteActor(actors[i]);
        }
    }
}

const bool& c_tile::hasActor(const int& actor) {
    for(int i = 0; i < v_actor.size(); ++i) {
        if(v_actor[i] == actor) {
            return true;
        }
    }
    return false;
}

const bool& c_tile::hasActorType(std::string actType) {
    for(int i = 0; i < v_actor.size(); ++i) {
        if(engine -> game -> actorManager.getActor(v_actor[i]) -> getId() == actType) {
            return true;
        }
    }
    return false;
}

// Returns vector of actors that can be picked up
std::vector<int> c_tile::getItems() {
    std::vector<int> v_item;
    for(int i = 0; i < v_actor.size(); ++i) {
        c_actor* p_actor = engine -> game -> actorManager.getActor(v_actor[i]);
        if(p_actor -> body and p_actor -> body -> getCanPickup()) {
            v_item.push_back(v_actor[i]);
        }
    }
    return v_item;
} 

void c_tile::setAsset(const s_tileAsset* asset) {
    id = asset -> id;
    name = asset -> name;
    desc = asset -> desc;
    tileX = asset -> tx;
    tileY = asset -> ty;
    color = asset -> color;
    bgcolor = asset -> bgcolor;
    olcolor = asset -> olcolor;
    type = asset -> type;
}

void c_tile::wipe(const s_tileAsset* asset) {
    id = asset -> id;
    name = asset -> name;
    desc = asset -> desc;
    tileX = asset -> tx;
    tileY = asset -> ty;
    color = asset -> color;
    bgcolor = asset -> bgcolor;
    olcolor = asset -> olcolor;
    type = asset -> type;
    script = 0;
    interior = 0;
    explored = 0;
    visible = 0;
    v_actor.clear();
    check = 0;
}

const int& c_tile::findActorByName(std::string name) {
    for(int i = 0; i < v_actor.size(); ++i) {
        if(engine -> game -> actorManager.getActor(v_actor[i]) -> getId() == name) {
            return engine -> game -> actorManager.getActor(v_actor[i]) -> getUid();
        }
    }
    return 0;
}

const int& c_tile::findStaircase() {
    for(int i = 0; i < v_actor.size(); ++i) {
        if(engine -> game -> actorManager.getActor(v_actor[i]) -> staircase) {
            return engine -> game -> actorManager.getActor(v_actor[i]) -> staircase -> getDirection();
        }
    }
    return 0;
}