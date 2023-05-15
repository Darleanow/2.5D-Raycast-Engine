#include "Camera.hpp"
#include <iostream>

camera::camera(sf::RenderWindow& renderWindow, map& Map) :
	m_renderWindow(renderWindow), m_map(Map)
{
	m_radius = 5;
	m_camera.setRadius(m_radius);
	m_camera.setFillColor(sf::Color(0, 255, 0));
	m_camera.setOrigin(sf::Vector2f(m_radius, m_radius));

	m_speedMove = 150.0f;
	m_speedAngle = 100.0f;

	m_position = { 128, 128 };

	m_fov = 60;
	m_rayLength = 800;

	for (unsigned int i = 0; i < SCREENWIDTH; i++)
	{
		sf::VertexArray ray(sf::Lines, 2);
		ray[0].color = sf::Color::Red;
		ray[1].color = sf::Color::Red;
		m_vecRays.push_back(ray);
		m_vecDistances.push_back(0.0f);
	}

	sinTable.resize(SCREENWIDTH);
	cosTable.resize(SCREENWIDTH);

	for (unsigned int i = 0; i < SCREENWIDTH; i++)
	{
		float rayAngle = m_angle + m_fov / 2 - i * (m_fov / SCREENWIDTH);
		sinTable[i] = dSin(rayAngle - m_angle);
		cosTable[i] = dCos(rayAngle - m_angle);
	}

}

void camera::draw3D()
{
	float sliceWidth = SCREENWIDTH / m_vecDistances.size();
	float wallHeightFactor = 100.0f;
	for (unsigned int i = 0; i < m_vecDistances.size(); i++)
	{
		float correctedDistance = m_vecDistances[i] / cosTable[i];

		float sliceHeight = (wallHeightFactor / correctedDistance) * SCREENHEIGHT;
		float halfSliceHeight = sliceHeight / 2.0f;
		sf::RectangleShape slice(sf::Vector2f(sliceWidth, sliceHeight));
		slice.setPosition(i * sliceWidth, (SCREENHEIGHT / 2) - halfSliceHeight);

		float maxDist = m_rayLength;
		float minBlue = 50.0f;
		float maxBlue = 255.0f;
		float blue = maxBlue - ((correctedDistance / maxDist) * (maxBlue - minBlue));
		slice.setFillColor(sf::Color(0, 0, std::max(minBlue, blue)));

		m_renderWindow.draw(slice);
	}
}


void camera::update()
{
	sf::Time dt = m_clock.restart();
	checkKeyboardHit(dt);
	projection();
}

void camera::projection()
{
	for (unsigned int i = 0; i < m_vecRays.size(); i++)
	{
		m_vecRays[i][0].position = m_position;

		float fAngle = m_angle + m_fov / 2 - i * (m_fov / SCREENWIDTH);
		fAngle -= (m_fov / 2.0) * ((float)i / (float)m_vecRays.size());

		sf::Vector2f direction = { dCos(fAngle), dSin(fAngle) };

		float x = m_position.x;
		float y = m_position.y;

		float deltaX = direction.x * m_rayLength;
		float deltaY = direction.y * m_rayLength;

		int steps = (abs(deltaX) > abs(deltaY)) ? abs(deltaX) : abs(deltaY);

		float xIncrement = deltaX / (float)steps;
		float yIncrement = deltaY / (float)steps;

		for (int j = 0; j < steps; j++)
		{
			x += xIncrement;
			y += yIncrement;

			int mapX = x / blocSize;
			int mapY = y / blocSize;

			if (mapX >= 0 && mapY >= 0 && mapX < m_map.getWidth() && mapY < m_map.getHeight() && m_map.checkMapCase(mapX, mapY))
			{
				m_intersection.x = x;
				m_intersection.y = y;
				m_vecDistances[i] = sqrt(pow((m_position.x - x), 2) + pow((m_position.y - y), 2));
				m_vecRays[i][1].position = m_intersection;
				break;
			}
			else
			{
				m_vecDistances[i] = m_rayLength;
			}
		}
	}
}

void camera::checkKeyboardHit(sf::Time dt)
{
	sf::Vector2f direction(dCos(m_angle), dSin(m_angle));
	sf::Vector2f right(-direction.y, direction.x);

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Z))
	{
		m_position += direction * m_speedMove * dt.asSeconds();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
	{
		m_position -= direction * m_speedMove * dt.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q))
	{
		m_position += right * m_speedMove * dt.asSeconds();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
	{
		m_position -= right * m_speedMove * dt.asSeconds();
	}

	if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
	{
		m_angle -= m_speedAngle * dt.asSeconds();
	}
	else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
	{
		m_angle += m_speedAngle * dt.asSeconds();
	}

	if (m_angle > 360)
		m_angle = 0;
	else if (m_angle < 0)
		m_angle = 360;

	m_camera.setPosition(m_position);
	m_camera.setRotation(m_angle);
}

bool camera::intersect(unsigned int it)
{
	float fAngle = (m_angle + m_fov / 2 - it * (m_fov / SCREENWIDTH));

	sf::Vector2f direction = { dCos(fAngle), dSin(fAngle) };

	for (unsigned int length = 0; length < m_rayLength; length++)
	{
		int dx = static_cast<int>(m_position.x + length * direction.x);
		int dy = static_cast<int>(m_position.y + length * direction.y);

		if (dx / blocSize < m_map.getWidth() && dy / blocSize < m_map.getHeight() && m_map.checkMapCase(dx / blocSize, dy / blocSize))
		{
			m_intersection.x = dx;
			m_intersection.y = dy;
			return true;
		}
	}
	return false;
}


inline float camera::toRadian(float deg)
{
	return (3.1415926535897932384626433832795028841971693993751058209749445923078164062862089986280348253421170679 / 180) * deg;
}

inline float camera::dCos(float deg)
{
	return cos(toRadian(deg));
}

inline float camera::dSin(float deg)
{
	return sin(toRadian(deg));
}