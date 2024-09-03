#pragma once
#include <SFML/Graphics.hpp>
#include <vector>

#include "constants.hpp"

/**
 * @class map
 * @brief Represents the game map, including its dimensions and obstacles.
 *
 * The map class handles the creation of a game environment, including
 * generating obstacles and providing methods to interact with the map data.
 */
class map {
 protected:
  sf::RenderWindow& m_renderWindow;  ///< Reference to the SFML render window.
  std::vector<std::vector<unsigned int>>
      m_vecMap;  ///< 2D vector representing the map grid.

  int width;   ///< Width of the map in blocks.
  int height;  ///< Height of the map in blocks.

  /**
   * @brief Adds random obstacles to the map.
   *
   * This function randomly places a specified number of obstacles within the
   * map.
   *
   * @param count The number of obstacles to add.
   */
  void addObstacles(int count);

 public:
  /**
   * @brief Constructs a map object.
   *
   * Initializes the map, sets its dimensions, and populates it with obstacles.
   *
   * @param m_renderWindow Reference to the SFML render window.
   */
  map(sf::RenderWindow& m_renderWindow);

  /**
   * @brief Checks if a specific map cell is a wall or obstacle.
   *
   * @param dx The x-coordinate of the cell.
   * @param dy The y-coordinate of the cell.
   * @return true if the cell is a wall or obstacle, false otherwise.
   */
  bool checkMapCase(unsigned int dx, unsigned int dy);

  /**
   * @brief Gets the width of the map.
   *
   * @return The width of the map in blocks.
   */
  size_t getWidth();

  /**
   * @brief Gets the height of the map.
   *
   * @return The height of the map in blocks.
   */
  size_t getHeight();
};
