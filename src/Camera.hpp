#pragma once

#include <math.h>

#define _USE_MATH_DEFINES

#include <SFML/Graphics.hpp>
#include <vector>

#include "Map.hpp"
#include "constants.hpp"

/**
 * @class camera
 * @brief Represents the camera in a 2.5D raycasting engine.
 */
class camera {
 protected:
  map& m_map;                        ///< Reference to the map object.
  sf::RenderWindow& m_renderWindow;  ///< Reference to the SFML render window.
  sf::CircleShape m_camera;  ///< Representation of the camera as a circle.
  sf::Vector2f m_position;   ///< Camera's current position in the world.
  sf::Vector2f
      m_intersection;  ///< Intersection point of the ray with an obstacle.
  std::vector<float>
      m_vecDistances;  ///< Distances from the camera to obstacles.
  sf::Clock m_clock;   ///< Clock to manage time-based events.

  float m_angle;       ///< Camera's current rotation angle.
  float m_fov;         ///< Camera's field of view.
  float m_rayLength;   ///< Maximum length of the rays cast by the camera.
  float m_radius;      ///< Radius of the camera's circular representation.
  float m_speedMove;   ///< Speed of camera movement.
  float m_speedAngle;  ///< Speed of camera rotation.
  int m_pitch;         ///< Camera's pitch (Y-axis rotation).

  std::vector<sf::VertexArray> m_vecRays;  ///< Rays cast from the camera.

  std::vector<float> sinTable;  ///< Precomputed sine values for ray angles.
  std::vector<float> cosTable;  ///< Precomputed cosine values for ray angles.

 public:
  /**
   * @brief Constructs a camera object.
   *
   * @param renderWindow Reference to the SFML window where the scene is
   * rendered.
   * @param Map Reference to the map object used by the camera.
   */
  camera(sf::RenderWindow& renderWindow, map& Map);

  /**
   * @brief Renders the 3D view of the scene based on the camera's perspective.
   */
  void draw3D();

  /**
   * @brief Updates the camera's position and orientation based on user input.
   */
  void update();

  /**
   * @brief Casts rays from the camera's position to determine distances to
   * obstacles.
   */
  void projection();

  /**
   * @brief Handles keyboard input to control the camera's movement and
   * rotation.
   *
   * @param dt Delta time to ensure consistent movement speed regardless of
   * frame rate.
   */
  void checkKeyboardHit(sf::Time dt);

  /**
   * @brief Determines whether a ray intersects with an obstacle in the map.
   *
   * @param it The index of the ray.
   * @return true if the ray intersects with an obstacle, false otherwise.
   */
  bool intersect(unsigned int it);

  /**
   * @brief Converts degrees to radians.
   *
   * @param deg Degrees to be converted.
   * @return Converted value in radians.
   */
  inline float toRadian(float deg);

  /**
   * @brief Computes the cosine of an angle in degrees.
   *
   * @param deg Angle in degrees.
   * @return Cosine of the angle.
   */
  inline float dCos(float deg);

  /**
   * @brief Computes the sine of an angle in degrees.
   *
   * @param deg Angle in degrees.
   * @return Sine of the angle.
   */
  inline float dSin(float deg);

};
