#include "Map.hpp"

#include <cstdlib>
#include <ctime>
#include <random>
#include <vector>

map::map(sf::RenderWindow& m_renderWindow) : m_renderWindow(m_renderWindow) {
  std::srand(std::time(nullptr));

  width = 101;
  height = 61;

  m_vecMap.resize(height, std::vector<unsigned int>(width, 0));

  for (int x = 0; x < width; ++x) {
    m_vecMap[0][x] = 1;
    m_vecMap[height - 1][x] = 1;
  }
  for (int y = 0; y < height; ++y) {
    m_vecMap[y][0] = 1;
    m_vecMap[y][width - 1] = 1;
  }

  addObstacles(300);
}

void map::addObstacles(int count) {
  std::default_random_engine rng(std::random_device{}());
  std::uniform_int_distribution<int> distX(1, width - 2);
  std::uniform_int_distribution<int> distY(1, height - 2);

  for (int i = 0; i < count; i++) {
    int x = distX(rng);
    int y = distY(rng);
    if (m_vecMap[y][x] == 0) {
      m_vecMap[y][x] = 1;
    }
  }
}

bool map::checkMapCase(unsigned int dx, unsigned int dy) {
  if (dx >= m_vecMap[0].size() || dy >= m_vecMap.size()) {
    return false;
  }

  return m_vecMap[dy][dx] != 0;
}

size_t map::getHeight() { return m_vecMap.size(); }

size_t map::getWidth() { return m_vecMap[0].size(); }
