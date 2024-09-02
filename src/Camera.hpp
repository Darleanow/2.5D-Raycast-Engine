#pragma once

#include <math.h>

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <vector>

#include "Map.hpp"
#include "constants.hpp"

class camera {
 protected:
  map& m_map;
  sf::RenderWindow& m_renderWindow;
  sf::CircleShape m_camera;
  sf::Vector2f m_position, m_intersection;
  std::vector<float> m_vecDistances;
  sf::Clock m_clock;

  float m_angle, m_fov, m_rayLength;
  float m_radius;
  float m_speedMove, m_speedAngle;

  std::vector<sf::VertexArray> m_vecRays;

  std::vector<float> sinTable, cosTable;

 public:
  camera(sf::RenderWindow& renderWindow, map& Map);

  void draw3D();
  void update();
  void projection();
  void checkKeyboardHit(sf::Time dt);
  bool intersect(unsigned int it);

  inline float toRadian(float deg);
  inline float dCos(float deg);
  inline float dSin(float deg);

  sf::Vector2f castRay(const sf::Vector2f& start, const sf::Vector2f& dir,
                       float maxLength);
};