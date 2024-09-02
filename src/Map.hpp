#pragma once
#include <SFML/Graphics.hpp>
#include <vector>
#include "constants.hpp"
class map
{
protected:
	sf::RenderWindow& m_renderWindow;
	std::vector<std::vector<unsigned int>> m_vecMap;
public:
	map(sf::RenderWindow& m_renderWindow);

	bool checkMapCase(unsigned int dx, unsigned int dy);

	size_t getWidth();
	size_t getHeight();
};

