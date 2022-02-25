#pragma once

#include <SFML/Graphics.hpp>
#include "MapTile.h"
#include <vector>

class GameWorld {

private:
    int m_nbEnemies;
    int m_nbWalls;
    sf::Vector2i m_exitPos;
    sf::Vector2i m_playerPos;
    std::vector<sf::Vector2i> m_wallPos;
    std::vector<sf::Vector2i> m_enemyPos;
    std::vector<sf::Vector2i> m_doorArea;
    std::vector<sf::Vector2i> m_playerArea;

    void SetupInitialState();

    void SetEnemyPositions();

    void SetWallPositions();

    sf::Vector2i GetRandomFreePos();

    void SetTiles();

    bool CheckIfPosIsFree(sf::Vector2i pos);

    std::vector<sf::Vector2i> GetFreePos(sf::Vector2i pos);

    void CheckCollisions();

    void RedrawEnemySprites();

    void SetPlayerPosition();

    void SetExitPosition();

    static void GetAreaAroundPos(sf::Vector2i pos, std::vector<sf::Vector2i> &vec, int iterations);

public:
    std::vector<std::vector<MapTile *>> m_tiles;
    sf::Vector2i m_gridSize;

    GameWorld(sf::Vector2i gird_size, int nb_enemies, int nb_walls);

    ~GameWorld();

    void MovePlayer(sf::Vector2i pos);

    void MoveEnemies();
};
