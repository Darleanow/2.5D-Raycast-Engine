/**
 * @file Camera.cpp
 * @brief Implementation of the camera class for a 2.5D raycasting engine.
 */

#include "Camera.hpp"

#include <cmath>
#include <iostream>


/**
 * @brief Constructs a camera object.
 *
 * @param renderWindow Reference to the SFML window where the scene is rendered.
 * @param Map Reference to the map object used by the camera.
 */
camera::camera(sf::RenderWindow& renderWindow, map& Map)
    : m_renderWindow(renderWindow), m_map(Map) {
  m_radius = 5;
  m_camera.setRadius(m_radius);
  m_camera.setFillColor(sf::Color(0, 255, 0));
  m_camera.setOrigin(sf::Vector2f(m_radius, m_radius));

  m_speedMove = 150.0f;
  m_speedAngle = 100.0f;

  m_position = {128, 128};

  m_fov = 60;
  m_rayLength = 800;

  m_angle = 0;
  m_pitch = 0;

  for (unsigned int i = 0; i < SCREENWIDTH; i++) {
    sf::VertexArray ray(sf::Lines, 2);
    ray[0].color = sf::Color::Red;
    ray[1].color = sf::Color::Red;
    m_vecRays.push_back(ray);
    m_vecDistances.push_back(0.0f);
  }

  sinTable.resize(SCREENWIDTH);
  cosTable.resize(SCREENWIDTH);

  for (unsigned int i = 0; i < SCREENWIDTH; i++) {
    float rayAngle = m_angle + m_fov / 2.0f - (i * m_fov) / (float)SCREENWIDTH;
    sinTable[i] = dSin(rayAngle);
    cosTable[i] = dCos(rayAngle);
  }
}

/**
 * @brief Renders the 3D view of the scene based on the camera's perspective.
 */
void camera::draw3D() {
  float sliceWidth = static_cast<float>(SCREENWIDTH) / m_vecDistances.size();
  float wallHeightFactor = 100.0f;

  for (unsigned int i = 0; i < m_vecDistances.size(); i++) {
    float correctedDistance = m_vecDistances[i] * cosTable[i];
    float sliceHeight = (wallHeightFactor / correctedDistance) * SCREENHEIGHT;
    float halfSliceHeight = sliceHeight / 2.0f;

    // Apply pitch offset to simulate looking up/down
    float pitchOffset = m_pitch * (SCREENHEIGHT / 2.0f) / 89.0f;

    sf::RectangleShape slice(sf::Vector2f(sliceWidth, sliceHeight));
    float yPosition = (SCREENHEIGHT / 2.0f) - halfSliceHeight + pitchOffset;

    // Ensure yPosition doesn't push the slice completely out of view
    if (yPosition > SCREENHEIGHT) {
      yPosition = SCREENHEIGHT - sliceHeight;
    } else if (yPosition + sliceHeight < 0) {
      yPosition = 0;
    }

    slice.setPosition(i * sliceWidth, yPosition);

    float maxDist = m_rayLength;
    float minColor = 50.0f;
    float maxColor = 255.0f;

    float colorIntensity =
        maxColor - ((correctedDistance / maxDist) * (maxColor - minColor));
    float red =
        std::max(minColor / 2, colorIntensity * 0.8f);  // Purple tone red
    float green =
        std::max(minColor / 2, colorIntensity * 0.2f);  // Less green for purple
    float blue = colorIntensity;                        // Full blue intensity

    slice.setFillColor(sf::Color(red, green, blue));

    m_renderWindow.draw(slice);
  }
}

/**
 * @brief Updates the camera's position and orientation based on user input.
 */
void camera::update() {
  sf::Time dt = m_clock.restart();
  checkKeyboardHit(dt);
  projection();
}

/**
 * @brief Casts rays from the camera's position to determine distances to
 * obstacles.
 */
void camera::projection() {
  for (unsigned int i = 0; i < m_vecRays.size(); i++) {
    m_vecRays[i][0].position = m_position;

    float fAngle =
        m_angle + m_fov / 2.0f - (i * m_fov) / static_cast<float>(SCREENWIDTH);
    sf::Vector2f direction = {dCos(fAngle), dSin(fAngle)};

    float x = m_position.x;
    float y = m_position.y;

    float deltaX = direction.x * m_rayLength;
    float deltaY = direction.y * m_rayLength;

    int steps = std::max(abs(deltaX), abs(deltaY));

    float xIncrement = deltaX / static_cast<float>(steps);
    float yIncrement = deltaY / static_cast<float>(steps);

    int mapXPrev = -1, mapYPrev = -1;

    for (int j = 0; j < steps; j++) {
      x += xIncrement;
      y += yIncrement;

      int mapX = x / blocSize;
      int mapY = y / blocSize;

      if (mapX == mapXPrev && mapY == mapYPrev) continue;

      if (mapX >= 0 && mapY >= 0 && mapX < m_map.getWidth() &&
          mapY < m_map.getHeight() && m_map.checkMapCase(mapX, mapY)) {
        m_intersection.x = x;
        m_intersection.y = y;
        float dx = m_position.x - x;
        float dy = m_position.y - y;
        m_vecDistances[i] = sqrt(dx * dx + dy * dy);
        m_vecRays[i][1].position = m_intersection;

        break;
      } else {
        m_vecDistances[i] = m_rayLength;
      }

      mapXPrev = mapX;
      mapYPrev = mapY;
    }
  }
}

/**
 * @brief Handles keyboard input to control the camera's movement and rotation.
 *
 * @param dt Delta time to ensure consistent movement speed regardless of frame
 * rate.
 */
void camera::checkKeyboardHit(sf::Time dt) {
  sf::Vector2f direction(dCos(m_angle), dSin(m_angle));
  sf::Vector2f right(direction.y, -direction.x);

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z)) {
    m_position += direction * m_speedMove * dt.asSeconds();
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S)) {
    m_position -= direction * m_speedMove * dt.asSeconds();
  }

  if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
    m_position -= right * m_speedMove * dt.asSeconds();
  } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D)) {
    m_position += right * m_speedMove * dt.asSeconds();
  }

  sf::Vector2i mousePos = sf::Mouse::getPosition(m_renderWindow);
  float diffX = mousePos.x - m_renderWindow.getSize().x / 2.f;
  float diffY = mousePos.y - m_renderWindow.getSize().y / 2.f;

  m_angle += -diffX * (m_speedAngle * 0.5f) * dt.asSeconds();
  m_pitch -= diffY * (m_speedAngle * 0.5f) * dt.asSeconds();

  // Clamp the pitch to avoid flipping
  if (m_pitch > 89.0f) m_pitch = 89.0f;
  if (m_pitch < -89.0f) m_pitch = -89.0f;

  sf::Mouse::setPosition(sf::Vector2i(m_renderWindow.getSize().x / 2,
                                      m_renderWindow.getSize().y / 2),
                         m_renderWindow);

  m_camera.setPosition(m_position);
  m_camera.setRotation(m_angle);
}

/**
 * @brief Determines whether a ray intersects with an obstacle in the map.
 *
 * @param it The index of the ray.
 * @return true if the ray intersects with an obstacle, false otherwise.
 */
bool camera::intersect(unsigned int it) {
  float fAngle =
      m_angle + m_fov / 2.0f - (it * m_fov) / static_cast<float>(SCREENWIDTH);
  sf::Vector2f direction = {dCos(fAngle), dSin(fAngle)};

  for (unsigned int length = 0; length < m_rayLength; length++) {
    int dx = static_cast<int>(m_position.x + length * direction.x);
    int dy = static_cast<int>(m_position.y + length * direction.y);

    if (dx / blocSize < m_map.getWidth() && dy / blocSize < m_map.getHeight() &&
        m_map.checkMapCase(dx / blocSize, dy / blocSize)) {
      m_intersection.x = dx;
      m_intersection.y = dy;
      return true;
    }
  }
  return false;
}

/**
 * @brief Converts degrees to radians.
 *
 * @param deg Degrees to be converted.
 * @return Converted value in radians.
 */
inline float camera::toRadian(float deg) { return (M_PI / 180.0f) * deg; }

/**
 * @brief Computes the cosine of an angle in degrees.
 *
 * @param deg Angle in degrees.
 * @return Cosine of the angle.
 */
inline float camera::dCos(float deg) { return cos(toRadian(deg)); }

/**
 * @brief Computes the sine of an angle in degrees.
 *
 * @param deg Angle in degrees.
 * @return Sine of the angle.
 */
inline float camera::dSin(float deg) { return sin(toRadian(deg)); }
