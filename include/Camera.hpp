#ifndef _CAMERA_HPP
#define _CAMERA_HPP 1

#include <SFML/Graphics.hpp>

class Camera {
public:
	Camera();
	Camera(const sf::Vector2f& size);
	Camera(const sf::Vector2f& size, const sf::Vector2f& position);
	Camera(const sf::Vector2f& size, const sf::Vector2f& position, const sf::FloatRect& viewport);
	~Camera();

	sf::Vector2f getSize() const;
	sf::Vector2f getPosition() const;
	float getRotation() const;
	sf::Vector2f getCenter() const;
	sf::FloatRect getScreenViewport() const;
		
	void setScreenViewport(const sf::FloatRect& viewport);
	
	void setSize(const sf::Vector2f& size);
	void setPosition(const sf::Vector2f& position);
	void setRotation(const float rotation);

	void move(const sf::Vector2f& offset);

	void apply();

	sf::Vector2f getMousePositionRelativeToCamera(bool addCameraPosition);
	sf::Vector2f getMousePositionRelativeToCamera();

	static Camera getDefaultCamera();

private:
	sf::Vector2f M_position;

	sf::View M_viewPort;

	friend class InitState;
};

#endif