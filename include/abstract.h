/**
 * An abstraction of the objects and components which will compose the scenes for 
 * the world of the game.
 *
 * @author  Roberto Fernandez Cueto
 * @date    16.12.2015
 *
 * $Id $
 */
#include <list>
#include <string>

#define ComponentList   std::list<GEngine::Component *>

namespace GEngine {
    class GameObject;
    class Component;
}

/**
 * An abstract class for the objects that can be on scenes.
 *
 * TODO: Scripts.
 */
class GEngine::GameObject {
    protected:
        ComponentList   attributes;
        std::string     name;
    public:
        void attachComponent(Component * comp);
        void removeComponent(std::string name, unsigned int type);
};

/**
 * An abstract class for the components of the GameObjects.
 */
class GEngine::Component {
    protected:
        std::string     name;
        unsigned int    type;
    public:
        Component(const std::string   name);
        Component(const char * name);
};

