#include <Camera.hpp>
#include <Game.hpp>
#include <StateManager.hpp>

#include <iostream>
#include <cmath>
#include <math.h>

Camera::Camera()
{

}

Camera::Camera(const sf::Vector2f& size)
{
	setSize(size);
}

Camera::Camera(const sf::Vector2f& size, const sf::Vector2f& position)
{
	setSize(size);
	setPosition(position);
}

Camera::Camera(const sf::Vector2f& size, const sf::Vector2f& position, const sf::FloatRect& viewport)
{
	setSize(size);
	setPosition(position);
	setScreenViewport(viewport);
}

Camera Camera::getDefaultCamera() {
	sf::View tempView = Scipp::globalGame->window->getDefaultView();
	return Camera(tempView.getSize(), tempView.getCenter(), tempView.getViewport());
}

sf::Vector2f Camera::getSize() const{
	return M_viewPort.getSize();
}

sf::Vector2f Camera::getPosition() const {
	return M_position;
}

float Camera::getRotation() const {
	return M_viewPort.getRotation();
}

sf::Vector2f Camera::getCenter() const {
	return M_viewPort.getCenter();
}

sf::FloatRect Camera::getScreenViewport() const {
	return M_viewPort.getViewport();
}

void Camera::setScreenViewport(const sf::FloatRect& viewport) 
{
	M_viewPort.setViewport(viewport);
}

void Camera::setSize(const sf::Vector2f& size) 
{
	M_viewPort.setSize(size);
}

void Camera::setPosition(const sf::Vector2f& position) 
{
	M_viewPort.setCenter(position);
	M_position = position;
}

void Camera::setRotation(const float rotation)
{
	M_viewPort.setRotation(rotation);
}

void Camera::move(const sf::Vector2f& offset)
{
	M_viewPort.move(offset);
	M_position = sf::Vector2f(M_position.x + offset.x, M_position.y + offset.y);
}

void Camera::apply() {
	Scipp::globalGame->window->setView(M_viewPort);
}

sf::Vector2f Camera::getMousePositionRelativeToCamera() {
	return getMousePositionRelativeToCamera(false);
}

sf::Vector2f Camera::getMousePositionRelativeToCamera(bool addCameraPosition) {
	sf::Vector2i mousePos = sf::Mouse::getPosition(*Scipp::globalGame->window);

	float x = mousePos.x - (getSize().x / 2) + M_position.x; // Mouse x position in the world not relative to camera rotation
	float y = mousePos.y - (getSize().y / 2) + M_position.y; // Mouse y position in the world not relative to camera rotation

	// Now we're calculating coordinates of the second point (mouse position relative to camera rotation) 
	// given camera rotation, camera center point (M_position) and coordinates of the first point (position in the world not relative to camera rotation)

	float radius = sqrt(pow(x - M_position.x, 2) + pow(y - M_position.y, 2)); // Distance between mouse position in the world and the camera center position

	float angle = atan2(y - M_position.y, x - M_position.x); // Angle between mouse position in the world and x-axis casted from the mouse center position
	float cameraRotation = getRotation() * (3.14159 / 180); // Convert camera rotation to radians

	float x2 = radius * cos(angle + cameraRotation);
	float y2 = radius * sin(angle + cameraRotation);

	if(addCameraPosition == true) {
		x2 += M_position.x;
		y2 += M_position.y;
	}

	return sf::Vector2f(x2, y2);
}

Camera::~Camera()
{

}