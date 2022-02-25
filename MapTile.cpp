//
// Created by gpelletier on 2022-02.
//

#include "MapTile.h"

using namespace sf;
using namespace std;

/**
 * MapTile constructor
 * @param texture_name name of the texture
 * @param pos position on map
 * @param is_passable is tile passable
 * @param is_exit is tile an exit
 */
MapTile::MapTile(const string &texture_name, Vector2f pos, bool is_passable, bool is_exit) :
        isPassable(is_passable),
        isExit(is_exit),
        m_pos(pos) {

    SetSprite(texture_name);
}

/**
 * Sets the texture, rect and position of a sprite
 * @param texture_name
 * @return
 */
void MapTile::SetSprite(const string &texture_name) {
    if (texture_name.empty() || !m_texture.loadFromFile(texture_name)) {
        m_sprite = nullptr;
    } else {
        m_texture.setSmooth(true);

        m_sprite = new Sprite();

        m_sprite->setTexture(m_texture);
        m_sprite->setTextureRect(IntRect(0, 0, SIZE, SIZE));
        m_sprite->setPosition(m_pos);
    }
}