//
// Created by gpelletier on 2022-02.
//

#include "GameWorld.h"
#include "Randomer.h"

#include <iostream>
#include <random>

using namespace std;
using namespace sf;

/**
 * GameWorld Constructor
 * @param gird_size: size of the play gird
 * @param nb_enemies: number of enemies on map
 * @param nb_walls: number of blocking walls on map
 */
GameWorld::GameWorld(const Vector2i gird_size, int nb_enemies, int nb_walls) : m_nbEnemies(nb_enemies),
                                                                               m_nbWalls(nb_walls),
                                                                               m_gridSize(gird_size) {
    SetupInitialState();
}

/**
 * GameWorld Destructor
 */
GameWorld::~GameWorld() {

    for (auto &tile: m_tiles) {
        for (auto mapTile: tile) {
            delete mapTile;
        }
        tile.clear();
    }
}

/**
 * Sets up the initial map elements
 */
void GameWorld::SetupInitialState() {
    SetPlayerPosition();
    SetExitPosition();
    SetEnemyPositions();
    SetWallPositions();
    SetTiles();
}

/**
 * Sets the position of the map exit door
 */
void GameWorld::SetExitPosition() {
    m_exitPos = Vector2i(1, 0); // fixed position top left of the map
    GetAreaAroundPos(m_exitPos, m_doorArea, 2);
}

/**
 * Sets the player position on the map
 */
void GameWorld::SetPlayerPosition() {
    m_playerPos = Vector2i(m_gridSize.x - 2, m_gridSize.y - 2); // fixed position bottom right of the map
    GetAreaAroundPos(m_playerPos, m_playerArea, 2);
}

/**
 * Creates a box area around a central position for a given number of outer iteration(s)
 * @param pos central position
 * @param vec list of positions around central position
 * @param iterations number of iterations around central position
 */
void GameWorld::GetAreaAroundPos(Vector2i pos, vector<Vector2i> &vec, int iterations){
    vec.push_back(pos);

    for(int i = 1; i <= iterations; i++){
        vec.emplace_back(pos.x - i, pos.y);
        vec.emplace_back(pos.x - i, pos.y - i);
        vec.emplace_back(pos.x, pos.y - i);
        vec.emplace_back(pos.x + i, pos.y - i);
        vec.emplace_back(pos.x + i, pos.y);
        vec.emplace_back(pos.x + i, pos.y + i);
        vec.emplace_back(pos.x, pos.y + i);
        vec.emplace_back(pos.x - i, pos.y + i);

    }
}

/**
 * Sets enemy position on map
 */
void GameWorld::SetEnemyPositions() {

    m_enemyPos.clear();

    for (int i = 0; i < m_nbEnemies; i++) {
        m_enemyPos.emplace_back(GetRandomFreePos());
    }
}

/**
 * Sets blocking wall on map
 */
void GameWorld::SetWallPositions() {

    m_wallPos.clear();

    for (int i = 0; i < m_nbWalls; i++) {
        m_wallPos.emplace_back(GetRandomFreePos());
    }
}

/**
 * Finds a random free position on the map
 * @return position
 */
Vector2i GameWorld::GetRandomFreePos() {
    Vector2i pos;
    for (int i = 0; i < m_gridSize.x * m_gridSize.y; i++) {

        pos = Vector2i(Randomer::RandomInt(1, m_gridSize.x - 2),
                       Randomer::RandomInt(1, m_gridSize.y - 2));

        if (ranges::find(m_playerArea, pos) == m_playerArea.end() &&
            ranges::find(m_doorArea, pos) == m_doorArea.end() &&
            ranges::find(m_wallPos, pos) == m_wallPos.end() &&
            ranges::find(m_enemyPos, pos) == m_enemyPos.end()) {
            break;
        }
    }

    return pos;
}

/**
 * Sets up the initial tiles of the map
 */
void GameWorld::SetTiles() {
    m_tiles.clear();

    for (int i = 0; i < m_gridSize.x; i++) {
        vector<MapTile *> row;
        for (int j = 0; j < m_gridSize.y; j++) {
            const int tileI = i * MapTile::SIZE;
            const int tileJ = j * MapTile::SIZE;

            if (auto curPos = Vector2i(i, j); curPos == m_exitPos) {
                row.push_back(new MapTile("images/door.png", Vector2f(tileI, tileJ), true, true));
            } else if (curPos == m_playerPos) {
                row.push_back(new MapTile("images/player_transparent.png", Vector2f(tileI, tileJ), true, false));
            } else if (curPos.x == 0 || curPos.x == m_gridSize.x - 1 || curPos.y == 0 || curPos.y == m_gridSize.y - 1) {
                row.push_back(new MapTile("images/wall.png", Vector2f(tileI, tileJ), false, false));
            } else if (ranges::find(m_enemyPos, curPos) != m_enemyPos.end()) {
                row.push_back(new MapTile("images/enemy_transparent.png", Vector2f(tileI, tileJ), true, false));
            } else if (ranges::find(m_wallPos, curPos) != m_wallPos.end()) {
                row.push_back(new MapTile("images/wall.png", Vector2f(tileI, tileJ), false, false));
            } else {
                row.push_back(new MapTile("", Vector2f(tileI, tileJ), true, false));
            }
        }

        m_tiles.push_back(row);
    }
}

/**
 * Verifies if the position in parameter is free
 * @param pos: position to verify
 * @return true if position is free
 */
bool GameWorld::CheckIfPosIsFree(const Vector2i pos) {
    if (pos.x < 0 ||
        pos.y < 0 ||
        pos.x > m_gridSize.x - 1 ||
        pos.y > m_gridSize.y - 1 ||
        !m_tiles[pos.x][pos.y]->isPassable ||
        m_tiles[pos.x][pos.y]->isExit ||
        ranges::find(m_enemyPos, pos) != m_enemyPos.end()) {
        return false;
    }

    return true;
}

/**
 * Finds free position around the current position passed in parameter
 * @param pos: current position
 * @return list of free position
 */
vector<Vector2i> GameWorld::GetFreePos(Vector2i pos) {
    vector<Vector2i> freePositions;
    vector<Vector2i> possibleMoves;

    GetAreaAroundPos(pos, possibleMoves, 1);

    for (auto &pos: possibleMoves) {
        if (CheckIfPosIsFree(pos)) {
            freePositions.push_back(pos);
        }
    }

    return freePositions;
}

/**
 * Moves all enemies to random free positions, redraws their sprites and checks for collisions
 */
void GameWorld::MoveEnemies() {
    for (auto &pos: m_enemyPos) {
        m_tiles[pos.x][pos.y]->SetSprite("");

        vector<Vector2i> freePositions = GetFreePos(pos);
        Vector2i newPos = freePositions[Randomer::RandomInt(0, (int) freePositions.size() - 1)];
        pos = newPos;
    }

    RedrawEnemySprites();
    CheckCollisions();
}

/**
 * Checks collision between player and exit and player and enemies, if collision reset game
 */
void GameWorld::CheckCollisions() {
    if (m_playerPos == m_exitPos ||
        ranges::find(m_enemyPos, m_playerPos) != m_enemyPos.end()) {
        SetupInitialState();
    }
}

/**
 * Moves the player by increment passed in parameter and sets player sprite to current position
 * @param pos: increment to the player's position
 */
void GameWorld::MovePlayer(Vector2i pos) {
    if (pos.y == -1) {
        if (m_playerPos.y == 0 || !m_tiles[m_playerPos.x][m_playerPos.y - 1]->isPassable)
            return;
    }

    if (pos.y == 1) {
        if (m_playerPos.y == m_gridSize.x - 1 || !m_tiles[m_playerPos.x][m_playerPos.y + 1]->isPassable)
            return;
    }

    if (pos.x == -1) {
        if (m_playerPos.x == 0 || !m_tiles[m_playerPos.x - 1][m_playerPos.y]->isPassable)
            return;
    }

    if (pos.x == 1) {
        if (m_playerPos.x == m_gridSize.x - 1 || !m_tiles[m_playerPos.x + 1][m_playerPos.y]->isPassable)
            return;
    }

    m_tiles[m_playerPos.x][m_playerPos.y]->SetSprite("");

    m_playerPos += pos;
    m_tiles[m_playerPos.x][m_playerPos.y]->SetSprite("images/player_transparent.png");

    CheckCollisions();
}

/**
 * Sets enemy sprites to there current position
 */
void GameWorld::RedrawEnemySprites() {
    for (const auto &pos: m_enemyPos) {
        m_tiles[pos.x][pos.y]->SetSprite("images/enemy_transparent.png");
    }
}
