c_game::~c_game() {
    saveMap();
    actorManager.clear();
    if(map) {
        delete map;
    }
}

const bool& c_game::newGame() {
    // Creates map
    map = new c_map();
    map -> init();
    // Creates world map 
    world = new s_worldTile*[MAPSIZE];
	for(int i = 0; i < MAPSIZE; ++i)
		world[i] = new s_worldTile[MAPSIZE];
    for(int i1 = 0; i1 < MAPSIZE; ++i1) {
        for(int i2 = 0; i2 < MAPSIZE; ++i2) {
            world[i1][i2].biome = biome::grassland;
            world[i1][i2].danger = 0;
        }
    }
    // Loads initial map
    loadMap(50, 0, 0);
    storeMap();
    updateWorld();
    actorManager.createActor("avatar", 2, 2);
    c_helper::teleportActor(actorManager.getPlayer() -> getUid(), 2, 2, true);
    engine -> sound.playAmbience(engine -> game -> map -> getAmbience());
    return true;
}

const bool& c_game::saveGame() {
    return true;
}

const bool& c_game::loadGame() {
    return true;
}

void c_game::endGame() {
    engine -> interface.gameOver();
	exit(EXIT_SUCCESS);
    return;
}

const bool& c_game::changeMap(const int& x, const int& y, const int& z) {
    if(!map) {
        return false;
    }
    c_actor* p_player = actorManager.getPlayer();
    // Calculate starting position
    int startX = p_player -> getMapX();
    int startY = p_player -> getMapY();
    if(x == 50 and y == 0 and z == 0) {
        startX = map -> getX();
        startY = map -> getY();
    } else if(isWorldMap() == true) {
        startX = map -> getWidth() / 2;
        startY = map -> getHeight() / 2;
    } else {
        if(z == map -> getZ()) {
            if(x < map -> getX()) {
                startX = map -> getWidth() - 2;
            } else if(x > map -> getX()) {
                startX = 1;
            } else if(y < map -> getY()) {
                startY = map -> getHeight() - 2;
            } else if(y > map -> getY()) {
                startY = 1;
            }
        }       
    }
    // Screen update
    engine -> setLoading(true);
    engine -> screen.clear();
    engine -> interface.draw();
    engine -> screen.display();
    engine -> interface.setTileDestination(0);
    // Map operations
    saveMap();
    actorManager.clear();
    loadMap(x, y, z);
    // Teleport player to new starting tile
    map -> teleportActor(p_player -> getUid(), startX, startY, true);
	//engine -> sound.playAmbience(engine -> game -> map -> getAmbience());
	engine -> setLoading(false);
    return true;
}

void c_game::loadMap(const int& x, const int& y, const int& z) {
	if(!map) {
		return;
	}
	TCODZip zip;
    map -> wipe(x, y, z);
	// World map
	if(x == 50 and y == 0 and z == 0) {
        // Load static
        std::string path = "data/map/" + std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z) + ".map";
        if(zip.loadFromFile(path.c_str())) {
            map -> load(&zip);
            path = "data/map/" + std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z) + ".act";
            actorManager.loadActorsFromText(path);
            return;
        }
        // Parse from txt if no static was found
        map -> parse("data/world.txt"); 
	} else {
        // Load saved
        std::string path = "data/save/" + std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z) + ".sav";
        if(zip.loadFromFile(path.c_str())) {
            map -> load(&zip);
            actorManager.loadActorsFromBinary(&zip);
            return;
        }
        // Load static
        path = "data/map/" + std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z) + ".map";
        if(zip.loadFromFile(path.c_str())) {
            map -> load(&zip);
            path = "data/map/" + std::to_string(x) + "." + std::to_string(y) + "." + std::to_string(z) + ".act";
            actorManager.loadActorsFromText(path);
            return;
        }
        // Genrate wilderness map
        map -> setX(x);
	    map -> setY(y);
	    map -> setZ(z);
		if(world[x][y].biome == biome::grassland) {
			engine -> runScript("gen/grassland.lua");
		} else if(world[x][y].biome == biome::temperateForest) {
			engine -> runScript("gen/temperateForest.lua");
		} else if(world[x][y].biome == biome::taiga) {
			engine -> runScript("gen/taiga.lua");
		} else if(world[x][y].biome == biome::jungle) {
			engine -> runScript("gen/jungle.lua");
		} else if(world[x][y].biome == biome::desert) {
			engine -> runScript("gen/desert.lua");
		} else if(world[x][y].biome == biome::savanna) {
			engine -> runScript("gen/savanna.lua");
		} else if(world[x][y].biome == biome::marsh) {
			engine -> runScript("gen/marsh.lua");
		} else if(world[x][y].biome == biome::tundra) {
			engine -> runScript("gen/tundra.lua");
		} else if(world[x][y].biome == biome::river) {
			engine -> runScript("gen/river.lua");
		}
	}
    map -> setX(x);
	map -> setY(y);
	map -> setZ(z);
}

void c_game::saveMap() {
	if(!map) {
		return;
	}
    TCODZip zip;
    map -> save(&zip);
    actorManager.saveMapActors(&zip);
    std::string filename;
    filename = "data/save/" + std::to_string(map -> getX()) + "." + std::to_string(map -> getY()) + "." + std::to_string(map -> getZ()) + ".sav";
    zip.saveToFile(filename.c_str());
	return;
}

void c_game::storeMap() {
	if(!map) {
		return;
	}
    // Save tile map in a binary file
    TCODZip zip;
    map -> save(&zip);
    actorManager.saveMapActors(&zip); // Not needed but we leave it not to break updateWorld()
    std::string filename;
    filename = "data/map/" + std::to_string(map -> getX()) + "." + std::to_string(map -> getY()) + "." + std::to_string(map -> getZ()) + ".map";
    zip.saveToFile(filename.c_str());
    // Save actors in a human-readable file
    std::string path = "data/map/" + std::to_string(map -> getX()) + "." + std::to_string(map -> getY()) + "." + std::to_string(map -> getZ()) + ".act";
    actorManager.storeMapActors(path);
	return;
}

// Fuck I had to be really stoned when I wrote this
void c_game::updateWorld() {
    TCODZip zip;
    std::string defaultFilename = "data/map/50.0.0.map";
    // First we need to load a temporal map with the info from 0.0.0
    if(zip.loadFromFile(defaultFilename.c_str())) {
        typedef struct s_temp {
            std::string tile;
            std::vector<std::string> v_actor;
        };
        const int width = zip.getInt();;
        const int height = zip.getInt();
        s_temp tempMap[MAPSIZE][MAPSIZE];
        for(int i1 = 0; i1 < width; ++i1) {
            for(int i2 = 0; i2 < height; ++i2) {
                // We assign the tile from the data file
                tempMap[i1][i2].tile = zip.getString();
                // We need to do this to respect the order of the stored data
                zip.getInt(); 
            }
        }
        // We need to do this to respect the order of the stored data
        zip.getInt();
        zip.getInt();
        zip.getInt();
        zip.getString();
        zip.getString();
        // We load the actors
        int size = zip.getInt();
        if(size > 0) {
            for(int i = 0; i < size; ++i) {
                std::string id = zip.getString();
                int x = zip.getInt();
                int y = zip.getInt();
                int actor = actorManager.createActor(id, x, y); // We need to create the actor so that it loads and respect the data file...
                actorManager.getActor(actor) -> load(&zip);
                actorManager.deleteActor(actor); // We delete the actor and make sure nobody saw us
                // We add the actor to its tile in the temp map
                tempMap[x][y].v_actor.push_back(id);
            }
        }
        // All right, so we have a temporal map with tile ids and actor vectors
        // Now here goes the meat, it's what everyone was waiting for, you, your mother, everyone
        // We choose biome depending on tile type, actor, etc. 
        // Well actually we will only use the actors to check for locations
        for(int i = 0; i < width; ++i) {
            for(int j = 0; j < height; ++j) {
                // GRASSLAND (default)
                world[i][j].biome = biome::grassland;
                world[i][j].danger = 0;
                // Look for locations
                if(tempMap[i][j].v_actor.size() != 0) {
                    for(int k = 0; k < tempMap[i][j].v_actor.size(); ++k) {
                        if(engine -> assetManager.getActorAsset(tempMap[i][j].v_actor[k]) -> type == actorType::location) {
                            world[i][j].biome = biome::location;
                        } 
                    }
                }
                // Select biome by tile
                // MOUNTAIN
                if(tempMap[i][j].tile == "world_mountain") {
                    world[i][j].biome = biome::mountain;
                }
                // TEMPERATE FOREST
                if(tempMap[i][j].tile == "world_temperateForest") {
                    world[i][j].biome = biome::temperateForest;
                }
                // TAIGA
                if(tempMap[i][j].tile == "world_taiga") {
                    world[i][j].biome = biome::taiga;
                }
                // JUNGLE  
                if(tempMap[i][j].tile == "world_jungle") {
                    world[i][j].biome = biome::jungle;
                }
                // DESERT   
                if(tempMap[i][j].tile == "world_desert") {
                    world[i][j].biome = biome::desert;
                }
                // SAVANNA
                if(tempMap[i][j].tile == "world_savanna") {
                    world[i][j].biome = biome::savanna;
                }
                // MARSH
                if(tempMap[i][j].tile == "world_marsh") {
                    world[i][j].biome = biome::marsh;
                }              
                // TUNDRA
                if(tempMap[i][j].tile == "world_tundra") {
                    world[i][j].biome = biome::tundra;
                }
                // OCEAN
                if(tempMap[i][j].tile == "world_ocean") {
                    world[i][j].biome = biome::ocean;
                }
                // RIVER
                if(tempMap[i][j].tile == "world_river") {
                    world[i][j].biome = biome::river;
                }
                // LOCATION
                if(tempMap[i][j].tile == "world_location") {
                    world[i][j].biome = biome::location;
                }
            }
        }
        engine -> message("World updated!");
    } else {
        engine -> message("World update failed because map 50.0.0 was not found.");
    }
}

void c_game::populate(std::string herd) {
    s_herdAsset* asset = engine -> assetManager.getHerdAsset(herd);
    const int herdSize = 16;
    // Count max number of actors in herd
    int actorQuantity = 0;
    for(int i = 0; i < herdSize; ++i) {
        if(asset -> actor[i] != "") {
            ++actorQuantity;
        }
    }
    // Collect a list of all possible tiles
    std::vector<s_coordinates> tiles;
    for(int i = 0; i < MAPSIZE; ++i) {
        for(int j = 0; j < MAPSIZE; ++j) {
            c_tile* tile = map -> getTile(i, j);
            if(tile -> getType() == tileType::floor and tile -> hasAnyActor() == false) {
                s_coordinates coords;
                coords.x = tile -> getX();
                coords.y = tile -> getY();
                tiles.push_back(coords);
            }
        }
    }
    // Chooses a tile
    if(tiles.size() != 0) {
        std::random_shuffle(tiles.begin(), tiles.end());
        // Limits number of possible tiles
        int maxTiles = tiles.size();
        if(maxTiles > 64) {
            maxTiles = 64;
        }
        for(int i = 0; i < maxTiles; ++i) {
            connectedTiles.clear();
            for(int i1 = 0; i1 < MAPSIZE; ++i1) {
                for(int i2 = 0; i2 < MAPSIZE; ++i2) {
                    map -> getTile(i1, i2) -> setCheck(false);
                }
            }
            flood(tiles[i].x, tiles[i].y);
            // Discard connected tiles that are too far away from origin
            std::vector<s_coordinates> finalTiles;
            for(int j = 0; j < connectedTiles.size(); ++j) {
                TCODPath* path = map -> path(tiles[i].x, tiles[i].y, connectedTiles[j].x, connectedTiles[j].y);
                if(path -> size() <= global::maxHerdDistanceFromOrigin) {
                    finalTiles.push_back(connectedTiles[j]);
                }
                delete path;
                if(finalTiles.size() >= (actorQuantity * 2)) {
                        break;
                }
            }
            // If origin tile has enough space around for all possible actors, use it to populate
            if(finalTiles.size() >= actorQuantity) {
                std::random_shuffle(finalTiles.begin(), finalTiles.end());
                int tileCounter = 0;
                for(int j = 0; j < actorQuantity; ++j) {
                    if(c_helper::d100(asset -> chance[j]) == true) {
                        actorManager.createActor(asset -> actor[j], finalTiles[tileCounter].x, finalTiles[tileCounter].x);
                        ++tileCounter;
                    }
                }
                break;
            }
        }
    }
}

// This flood is ONLY used in populate
void c_game::flood(const int& x, const int& y) {
    if(x > 0 and x < MAPSIZE - 1 and y > 0 and y < MAPSIZE - 1
    and map -> getTile(x, y) -> getType() == tileType::floor
    and map -> getTile(x, y) -> hasAnyActor() == false
    and map -> getTile(x, y) -> getCheck() == false) {
        map -> getTile(x, y) -> setCheck(true);
        s_coordinates coords;
        coords.x = x;
        coords.y = y;
        connectedTiles.push_back(coords);
    }
    else {
        return;
    }
    flood(x + 1, y);
    flood(x - 1, y);
    flood(x, y + 1);
    flood(x, y - 1);
}

int c_game::getBiome(const int& x, const int& y) {
    if(x > 0 and y > 0 and x < MAPSIZE and y < MAPSIZE) {
        return world[x][y].biome;
    }
}

void c_game::update(const int& key) {
    if(!actorManager.getPlayer() or !actorManager.getPlayer() -> action or !actorManager.getPlayer() -> player) {
        return;
    }

    gamelog.clear();

    // If the player is doing something, update time till he finishes
    if(actorManager.getPlayer() -> action -> isRunning()) {
        turn();

    // If he's not, check if he can channel the human player
    } else {
        actorManager.getPlayer() -> player -> channel(key);
    }

    gamelog.update();
}

void c_game::turn() {
    if(!actorManager.getPlayer() -> action -> isRunning()) {
        return;
    }
    do {
        // Update player
        actorManager.getPlayer() -> timeUpdate();

        // Update active actors
        actorManager.timeUpdate();
    } while(actorManager.getPlayer() -> action -> isRunning());
}

void c_game::message(std::string text) {
    gamelog.message(text);
}

bool c_game::runScript(structEventData& data) {  
    std::string path = data.type + ".lua";
    c_engine::runScript(path, data);
    return false;
}

bool c_game::runEvent(structEventData& data) {  
    if(!engine -> game -> map) {
        return false;
    }
    std::string path = "event/" + data.type + ".lua";
    c_engine::runScript(path, data);
    return false;
}

bool c_game::runEffect(structEventData& data) {  
    if(!engine -> game -> map) {
        return false;
    }
    std::string path = "effect/" + data.type + ".lua";
    c_engine::runScript(path, data);
    return false;
}

const bool& c_game::isWorldMap() { 
    if(map and map -> getX() == 50 and map -> getY() == 0) {
        return true; 
    }
    return false; 
}