#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
{
	//Assigns m_pixel_size to parameter variables.
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;

	//Calculates and assigns aspect ratio of monitor.
	m_aspectRatio = (float)pixelHeight / (float)pixelWidth;
	//Sets plane center to coord 0,0.
	m_plane_center = { 0,0 };
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
			}
		}
	}
}
//zoomIn function
void ComplexPlane::zoomIn()
{
	m_zoomCount++;
	int x_size = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	int y_size = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
	m_plane_size.x = x_size;
	m_plane_size.y = y_size;
	m_State = State::CALCULATING;
}
//zoomOut function
void ComplexPlane::zoomOut()
{
	m_zoomCount--;
	int x_size = BASE_WIDTH * (pow(BASE_ZOOM, m_zoomCount));
	int y_size = BASE_HEIGHT * m_aspectRatio * (pow(BASE_ZOOM, m_zoomCount));
	m_plane_size.x = x_size;
	m_plane_size.y = y_size;
	m_State = State::CALCULATING;
}
//Set Center function.
void ComplexPlane::setCenter(sf::Vector2i mousePixel)
{

}
//Load text function.
void ComplexPlane::loadText(sf::Text& text)
{

}
//Count Iterations function.
size_t ComplexPlane::countIterations(sf::Vector2f coord)
{

}
//Iterations to RGB function
void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b)
{

}
//Map Pixel to Coords function
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel)
{

}
