//
// Created by gpelletier on 2022-02.
//

#include <SFML/Graphics.hpp>
#include "GameWorld.h"

using namespace sf;
using namespace std;

int main() {

    Texture bgTexture;
    Sprite bgSprite;
    Clock clock;
    Time elapsed;

    RenderWindow window(VideoMode(800, 600), "RogueLike");
    GameWorld gameWorld = GameWorld(Vector2i(16, 12), 3, 20);

    if (!bgTexture.loadFromFile("images/Grass_Sample.png")) {
        return EXIT_FAILURE;
    }

    bgTexture.setRepeated(true);
    bgSprite.setTexture(bgTexture);
    bgSprite.setTextureRect(IntRect(0, 0, 800, 600));

    window.setKeyRepeatEnabled(false); // stops button spamming

    while (window.isOpen()) {
        Event event{};

        while (window.pollEvent(event)) {
            if (event.type == Event::Closed) {
                window.close();
            } else if (event.type == Event::KeyPressed) {
                if (Keyboard::isKeyPressed(Keyboard::Up)) {
                    gameWorld.MovePlayer(Vector2i(0, -1));
                }
                if (Keyboard::isKeyPressed(Keyboard::Down)) {
                    gameWorld.MovePlayer(Vector2i(0, 1));
                }
                if (Keyboard::isKeyPressed(Keyboard::Left)) {
                    gameWorld.MovePlayer(Vector2i(-1, 0));
                }
                if (Keyboard::isKeyPressed(Keyboard::Right)) {
                    gameWorld.MovePlayer(Vector2i(1, 0));
                }
            }
        }

        window.clear();

        window.draw(bgSprite);

        elapsed = clock.getElapsedTime();
        if (elapsed.asSeconds() >= 0.5) {
            clock.restart();
            gameWorld.MoveEnemies();
        }

        for (auto &mapTiles: gameWorld.m_tiles) {
            for (auto &mapTile: mapTiles) {
                if (mapTile->m_sprite != nullptr) // don't draw null sprites
                    window.draw(*mapTile->m_sprite);
            }
        }

        window.display();
    }

    return 0;
}
