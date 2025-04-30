// ComplexPlane.cpp
#include "ComplexPlane.h"

// Constructor: initialize screen size, complex plane, and vertex array
ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight)
    : m_pixelWidth(pixelWidth), m_pixelHeight(pixelHeight) {
    m_aspectRatio = static_cast<float>(pixelHeight) / static_cast<float>(pixelWidth);
    m_plane_center = { -2.0f, 0.f }; // Start view shifted left
    m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
    m_zoomCount = 0;
    m_State = State::CALCULATING;

    m_vArray.setPrimitiveType(sf::Points); // each pixel is a point
    m_vArray.resize(pixelWidth * pixelHeight);
}

// Draw function for the render window (required by SFML)
void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
    target.draw(m_vArray);
}

// Updates the Mandelbrot image only if the state is CALCULATING
void ComplexPlane::updateRender() {
    if (m_State != State::CALCULATING) return;

    // Loop through each pixel on the screen
    for (int i = 0; i < m_pixelHeight; ++i) {
        for (int j = 0; j < m_pixelWidth; ++j) {
            size_t index = j + i * m_pixelWidth;
            m_vArray[index].position = sf::Vector2f(static_cast<float>(j), static_cast<float>(i));

            sf::Vector2f coord = mapPixelToCoords({ j, i });
            size_t iter = countIterations(coord);

            sf::Uint8 r, g, b;
            iterationsToRGB(iter, r, g, b);
            m_vArray[index].color = sf::Color(r, g, b);
        }
    }

    m_State = State::DISPLAYING;
}

// Zoom in by reducing the view size
void ComplexPlane::zoomIn() {
    m_zoomCount++;
    float scale = std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { BASE_WIDTH * scale, BASE_HEIGHT * m_aspectRatio * scale };
    m_State = State::CALCULATING;
}

// Zoom out by increasing the view size
void ComplexPlane::zoomOut() {
    m_zoomCount--;
    float scale = std::pow(BASE_ZOOM, m_zoomCount);
    m_plane_size = { BASE_WIDTH * scale, BASE_HEIGHT * m_aspectRatio * scale };
    m_State = State::CALCULATING;
}

// Set a new center point in the complex plane based on mouse click
void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
    m_plane_center = mapPixelToCoords(mousePixel);
    m_State = State::CALCULATING;
}

// Update the stored mouse location (used for showing cursor coordinates)
void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) 
{
    m_mouseLocation = mapPixelToCoords(mousePixel);
}

// Load formatted text with center, cursor, and zoom instructions
void ComplexPlane::loadText(sf::Text& text) {
    std::stringstream ss;
    ss << "Mandelbrot Set\n";
    ss << "Center: (" << m_plane_center.x << "," << m_plane_center.y << ")\n";
    ss << "Cursor: (" << m_mouseLocation.x << "," << m_mouseLocation.y << ")\n";
    ss << "Left-click to Zoom in\n";
    ss << "Right-click to Zoom out";
    text.setString(ss.str());
}

// Compute how many iterations it takes for the point to escape the Mandelbrot set
size_t ComplexPlane::countIterations(sf::Vector2f coord) const {
    std::complex<float> z(0, 0);
    std::complex<float> c(coord.x, coord.y);
    size_t iter = 0;

    while (std::abs(z) <= 2.0f && iter < MAX_ITER) {
        z = z * z + c;
        ++iter;
    }

    return iter;
}

// Convert iteration count to an RGB color (smooth gradient)
void ComplexPlane::iterationsToRGB(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) const {
    if (count == MAX_ITER) {
        r = g = b = 0;
    }
    else {
        float t = static_cast<float>(count) / MAX_ITER;
        r = static_cast<sf::Uint8>(9 * (1 - t) * t * t * t * 255);
        g = static_cast<sf::Uint8>(15 * (1 - t) * (1 - t) * t * t * 255);
        b = static_cast<sf::Uint8>(8.5 * (1 - t) * (1 - t) * (1 - t) * t * 255);
    }
}

// Convert a screen pixel position to a coordinate in the complex plane
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i pixel) const {
    float xCoord = (static_cast<float>(pixel.x) / m_pixelWidth) * m_plane_size.x
        + (m_plane_center.x - m_plane_size.x / 2.0f);
    float yCoord = (1.0f - static_cast<float>(pixel.y) / m_pixelHeight) * m_plane_size.y
        + (m_plane_center.y - m_plane_size.y / 2.0f);
    return { xCoord, yCoord };
}
