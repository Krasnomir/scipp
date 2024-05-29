#include <Camera.hpp>
#include <Game.h>
#include <StateManager.h>

#include <iostream>

Camera::Camera()
{
	M_viewPort.setCenter(0.f, 0.f);
}

Camera::Camera(const sf::Vector2f& size)
{
	setSize(size);
	M_viewPort.setCenter(0.f, 0.f);
}

Camera::Camera(const sf::Vector2f& size, const sf::Vector2f& position)
{
	setSize(size);
	setPosition(position);
	M_viewPort.setCenter(0.f, 0.f);
}

Camera::Camera(const sf::Vector2f& size, const sf::Vector2f& position, const sf::FloatRect& viewport)
{
	setSize(size);
	setPosition(position);
	setScreenViewport(viewport);
	M_viewPort.setCenter(0.f, 0.f);
}

Camera Camera::getDefaultCamera() {
	sf::View tempView = Scipp::globalGame->window->getDefaultView();
	return Camera(tempView.getSize(), tempView.getCenter(), tempView.getViewport());
}

sf::Vector2f Camera::getSize() const{
	return M_viewPort.getSize();
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
}

void Camera::setRotation(const float rotation)
{
	M_viewPort.setRotation(rotation);
}

void Camera::apply() {
	Scipp::globalGame->window->setView(M_viewPort);
}

Camera::~Camera()
{
}