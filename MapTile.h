#pragma once

#include <SFML/Graphics.hpp>

class MapTile {

public:
    static constexpr int SIZE = 50;

    //bool toDraw{};
    bool isPassable;
    bool isExit;
    sf::Vector2f m_pos;
    sf::Texture m_texture;
    sf::Sprite *m_sprite{};

    MapTile(const std::string &texture_name, sf::Vector2f pos, bool is_passable, bool is_exit);

    ~MapTile() {
        delete (m_sprite);
        m_sprite = nullptr;
    }

    void SetSprite(const std::string &texture_name);
};
