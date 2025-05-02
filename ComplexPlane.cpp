#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	//Assigns m_pixel_size to parameter variables.
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;

	//Calculates and assigns aspect ratio of monitor.
	m_aspectRatio = (float)pixelHeight / (float)pixelWidth;
	//Sets plane center to coord 0,0.
	m_plane_center = { -2,0 };
	//Assign plane size
	m_plane_size = { BASE_WIDTH,BASE_HEIGHT * m_aspectRatio };
	//Assigns zoom count to zero.
	m_zoomCount = 0;
	//Assigns state
	m_State = State::CALCULATING;
	//Initalizing vertex array
	m_vArray=sf::VertexArray(sf::PrimitiveType::Points, pixelWidth * pixelHeight);

}
//Draw function
void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const
{
	target.draw(m_vArray);
}
//updateRender function
void ComplexPlane::updateRender()
{
	if (m_State == State::CALCULATING)
	{
		for (int i = 0; i < m_pixel_size.y; i++)
		{
			for (int j = 0; j < m_pixel_size.x; j++)
			{
				m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };
				sf::Vector2f coord = mapPixelToCoords({ j,i });
				size_t iterator = countIterations(coord);

				sf::Uint8 r, g, b;
				iterationsToRGB(iterator, r, g, b);
				m_vArray[j + i * m_pixel_size.x].color = sf::Color(r, g, b);
			}
		}
	}
	m_State = State::DISPLAYING;
}
//zoomIn function
void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	float x_size = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	float y_size = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
	m_plane_size.x = x_size;
	m_plane_size.y = y_size;
	m_State = State::CALCULATING;
}
//zoomOut function
void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	float x_size = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	float y_size = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
	m_plane_size.x = x_size;
	m_plane_size.y = y_size;
	m_State = State::CALCULATING;
}
//Set Center function.
void ComplexPlane::setCenter(sf::Vector2i mousePixel)
{
	m_plane_center = mapPixelToCoords(mousePixel);
	m_State = State::CALCULATING;
}
//Set Mouse Location
void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel)
{
	m_mouseLocation = mapPixelToCoords(mousePixel);
}
//Load text function.
void ComplexPlane::loadText(sf::Text& text)
{
	std::stringstream ss;
	ss << "Mandelbrot Set\n";
	ss << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")\n";
	ss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")\n";
	ss << "Left click to zoom in.\n";
	ss << "Right click to zoom out.\n";
	text.setString(ss.str());
}
//Count Iterations function.
size_t ComplexPlane::countIterations(sf::Vector2f coord)
{
	std::complex <float> z(0, 0);
	std::complex<float> c(coord.x, coord.y);
	size_t iterator = 0;
	while (abs(z) <= 2.0f && iterator < MAX_ITER)
	{
		z = z * z + c;
		++iterator;
	}
	return iterator;
}
//Iterations to RGB function
void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b)
{
	if (count == MAX_ITER)
	{
		r = g = b = 0;
	}
	else
	{
		float t = static_cast<float>(count) / MAX_ITER;
		r = static_cast<sf::Uint8>(9 * (1 - t) * t * t * t * 255);
		g = static_cast<sf::Uint8>(15 * (1 - t) * (1-t) * t * t * 255);
		b = static_cast<sf::Uint8>(8.5 * (1 - t) * (1-t) * t * t * 255);
	}
}
//Map Pixel to Coords function
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel)
{
	float xCoord = ((float)mousePixel.x / m_pixel_size.x) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0f);
	float yCoord = ((float)mousePixel.y / m_pixel_size.y) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0f);

	return { xCoord,yCoord };
}
