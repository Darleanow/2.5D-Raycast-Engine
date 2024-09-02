#include <SFML/Graphics.hpp>
#include "src/Camera.hpp"
#include "src/Map.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(SCREENWIDTH, SCREENHEIGHT), "RayCast");
    window.setFramerateLimit(144);

    map _map(window);
    camera camera_(window, _map);
    window.setMouseCursorVisible(false);

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event)) {
            switch (event.type)
            {
                case sf::Event::Closed:
                    window.close();
                    break;
                case sf::Event::KeyReleased:
                    if (event.key.code == sf::Keyboard::Escape)
                        window.close();
                    break;
            }
        }
        camera_.update();
        window.clear();
        
        camera_.draw3D();

        window.display();
    }

}
