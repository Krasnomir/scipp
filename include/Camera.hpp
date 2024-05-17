#ifndef _CAMERA_HPP
#define _CAMERA_HPP 1

#include "SFML/Graphics.hpp"

class Camera {
public:
	Camera();
	Camera(const sf::Vector2f& size);
	Camera(const sf::Vector2f& size, const sf::Vector2f& position);
	Camera(const sf::Vector2f& size, const sf::Vector2f& position, const sf::FloatRect& viewport);
	~Camera();

	sf::Vector2f getSize() const;
	sf::Vector2f getCenter() const;
	sf::FloatRect getScreenViewport() const;
		
	void setScreenViewport(const sf::FloatRect& viewport);
	
	void setSize(const sf::Vector2f& size);
	void setPosition(const sf::Vector2f& position);

	void apply();

	static Camera getDefaultCamera();

private:
	sf::View M_viewPort;

	friend class InitState;


};


#endif