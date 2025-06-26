#include <iostream>
#include <SFML/Graphics.hpp>
using namespace std;

enum directions
{
	UP,
	DOWN,
	LEFT,
	RIGHT
};

int main()
{
	// Set the width and height of the window
	unsigned int width = 1080;
	unsigned int height = 720;

	// Create a window with the specified width and height
	sf::RenderWindow* window = new sf::RenderWindow(sf::VideoMode({ width, height }), "Morskoy Boy");
	window->setFramerateLimit(60);

	sf::Texture FourCellShipT;

	if (!FourCellShipT.loadFromFile("Sprites/4.png"))
	{
		cerr << "Error loading texture for FourCellShipT" << endl;
		return -1;
	}

	sf::Sprite FourCellShipS(FourCellShipT);

	sf::IntRect dir[4];

	for (int i = 0; i < 4; i++)
	{
		dir[i] = sf::IntRect({ 32 * i, 0 }, { 32, 32 });
	}
	FourCellShipS.setTextureRect(dir[DOWN]);
	FourCellShipS.setOrigin({ 100, 100 });
	FourCellShipS.setPosition({ static_cast<float>(width) / 2, static_cast<float>(height) / 2 });


	// Main loop to keep the window open and handle events
	while (window->isOpen())
	{
		// Poll for events in the window
		while(const optional event = window->pollEvent())
		{
			// Check if the event is a window close event or a key press event
			if (event->is<sf::Event::Closed>())
			{
				window->close();
			}
			else if (const auto*keyPressed = event->getIf<sf::Event::KeyPressed>())
			{
				if (keyPressed->scancode == sf::Keyboard::Scancode::Escape)
				{
					window->close();
				}
			}
		}
		
		//Render the window
		window->clear();

		//Drawing code
		window->draw(FourCellShipS);

		// Display the contents of the window
		window->display();
	}

	// Clean up and close the window
	delete window;

	return 0;
}