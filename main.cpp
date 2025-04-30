#include <iostream>
#include "ComplexPlane.h"

using namespace std;
using namespace sf;


int main()
{
	sf::VideoMode::getDesktopMode().width;
	sf::VideoMode::getDesktopMode().height;

	sf::RenderWindow window(sf::VideoMode::getDesktopMode(), "MandelBrot!",sf::Style::Default);

	ComplexPlane Myplane(sf::VideoMode::getDesktopMode().width, sf::VideoMode::getDesktopMode().height);
	

	sf::Font font;
	if (!font.loadFromFile("Font.ttf"))
	{
		cout << "Unable to open file" << endl;
	}
	
	sf::Text text;
	text.setFont(font);
	Myplane.loadText(text);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed) { window.close(); }

			if (event.type == Event::MouseButtonPressed)
			{
				if (event.mouseButton.button == Mouse::Left)
				{
					Myplane.zoomIn();
					Myplane.setCenter(Mouse::getPosition(window));
				}

				else if (event.mouseButton.button == Mouse::Right)
				{
					Myplane.zoomOut();
					Myplane.setCenter(Mouse::getPosition(window));
				}
			}

			if (event.MouseMoved)
			{
				Myplane.setMouseLocation(Mouse::getPosition(window));
			}
		}










	}








}
