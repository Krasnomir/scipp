#ifndef __MENU_HPP
#define __MENU_HPP 1

#include <Scriptable/State.hpp>
#include <Scriptable/UI/UI.hpp>
#include <Scriptable/UI/Rect.hpp>
#include <Scriptable/EventObject.hpp>
#include <Game.hpp>

namespace Scriptable::UI{


    class MenuButton : public Rect{
        public:
            MenuButton(sf::Vector2f size) : Rect({0,0, size.x, size.y}){
                m_RenderComponent->setColor(sf::Color::Red);
                setLayer(1);
                setGroup("menu");
                setAttachmentTarget("menu_frame", {AttachmentPoint::TOPLEFT, {0.25, 0.5}});
                setVisible(true);
            }



            virtual ~MenuButton(){

            }
    };

    class MenuFrame : public Rect{
        public:
            MenuFrame() : Rect({0,0,0.4,0.2}){
                setGroup("menu");
                setVisible(true);
                setLayer(0);
                
                auto* currentState = Scipp::globalGame->stateManager.currentState;
                currentState->addUIObject<MenuButton>("menu_playbutton", sf::Vector2f(0.2, 0.2));
                
                {
                    currentState->addUIObject<TextObject>("menu_text", "font");
                    TextObject* text = dynamic_cast<TextObject*>(currentState->getUIObject("menu_text"));
                    text->setString("Radiating Wasteland");
                    text->setFillColor(sf::Color::Black);
                    text->setCharacterSize(25);
                    text->setLayer(2);
                    
                    sf::FloatRect localBound = text->getGlobalBounds();
                    text->setOrigin({localBound.width / 2.f, localBound.height / 2.f});
                    text->setPosition(currentState->translateRatioToPixels({0.5, 0.35}));
                }
                
                {
                    currentState->addUIObject<TextObject>("menu_playtext", "font");
                    TextObject* text = dynamic_cast<TextObject*>(currentState->getUIObject("menu_playtext"));
                    text->setString(">");
                    text->setFillColor(sf::Color::Black);
                    text->setCharacterSize(80);
                    
                    
                    sf::FloatRect localBound = text->getLocalBounds();
                    printf("%f %f %f %f\n", localBound.left, localBound.top, localBound.width, localBound.height);
                    text->setOrigin({(localBound.width + localBound.left) / 2.f, (localBound.height + localBound.top) / 2.f});
                    text->setPosition(currentState->translateRatioToPixels({0.4, 0.46}));  
                    text->setLayer(2);
                }
                
            }

            

            ~MenuFrame(){

            }
    };

};

#endif