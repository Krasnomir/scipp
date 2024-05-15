#include "Camera.hpp"
#include "Game.h"
#include "StateManager.h"


Camera::Camera()
{
	
}

Camera Camera::getDefaultCamera() {
	sf::View tempView = Scipp::globalGame->window->getDefaultView();
	return Camera(tempView.getSize(), tempView.getCenter(), tempView.getViewport());
}

Camera::Camera(const sf::Vector2f& size)
{
	setSize(size);
}

sf::Vector2f Camera::getSize() const{
	return M_viewPort.getSize();
}
sf::Vector2f Camera::getCenter() const {
	return M_viewPort.getCenter();
}

sf::FloatRect Camera::getScreenViewport() const {
	return M_viewPort.getViewport();
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
}


void Camera::apply() {
	Scipp::globalGame->window->setView(M_viewPort);
}

Camera::~Camera()
{
}