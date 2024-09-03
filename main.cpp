/**
 * @file main.cpp
 * @brief Entry point for the 2.5D raycasting engine.
 *
 * This file contains the main function, which initializes the SFML window,
 * creates the map and camera objects, and runs the main loop of the
 * application.
 */

#include <SFML/Graphics.hpp>

#include "src/Camera.hpp"
#include "src/Map.hpp"

/**
 * @brief The main function for the 2.5D raycasting engine.
 *
 * Initializes the game window, sets up the map and camera, and enters the main
 * loop where events are handled, and the scene is updated and rendered.
 *
 * @return int Returns 0 upon successful execution.
 */
int main() {
  // Create the SFML render window
  sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "RayCast");
  window.setFramerateLimit(144);
  window.setVerticalSyncEnabled(true);

  // Create a map with a randomly generated maze
  map _map(window);
  camera camera_(window, _map);
  window.setMouseCursorVisible(false);

  // Main loop
  while (window.isOpen()) {
    sf::Event event;
    while (window.pollEvent(event)) {
      switch (event.type) {
        case sf::Event::Closed:
          window.close();
          break;
        case sf::Event::KeyReleased:
          if (event.key.code == sf::Keyboard::Escape) window.close();
          break;
      }
    }

    // Update and draw in the same loop
    camera_.update();
    window.clear();
    camera_.draw3D();
    window.display();
  }

  return 0;
}
