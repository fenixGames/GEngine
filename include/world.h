/**
 * Description of the classes to manage all the whole regions of the program,
 * not all regions are on the screen at the same time, they are defined as if
 * a real world were.
 *
 * First, we have the universe, with contains several world which can be printed
 * at any time. Each world contains several regions (i.e., continents), these 
 * regions contains several maps (i.e., countries) containing several scenes,
 * which are the compedium of objects that are directly mapped to the display.
 *
 * Usually, we only will have 27 (3x3x3) scenes on memory and the other ones will 
 * be loaded when we move from a scene to another.
 *
 * @author  Roberto Fernandez Cueto
 * @date    03.12.2015
 */

#ifndef _WORLD_H_
#define _WORLD_H_

#include "geometry.h"
#include "camera.h"
#include "light.h"

namespace GEngine {
    class Universe;
    class World;
    class Region;
    class Map;
    class Scene;
    enum position {
        GES_NORTH,
        GES_SOUTH,
        GES_EAST,
        GES_WEST,
        GES_CENTER,
        GES_UP,
        GES_DOWN
    };
};

#define FigureList      std::list<GEngine::Geometry::Figure *>
#define CameraVector    std::vector<GEngine::Camera *>

/**
 * The list of figures and objects to map into the window.
 */
class GEngine::Scene {
    friend class Map;
    private:
        static Material black;
    protected:
        struct {
            long long xmin;
            long long xmax;
            long long ymin;
            long long ymax;
            long long zmin;
            long long zmax;
        } limits;
        FigureList      figures;    /* The list of figures on the scene. */
        Material        * horizon;    /* The material for the horizon -> static. */
        enum position   scenePos;   /* The position of the scene, CENTER means the camera 
                                       is in this scene. */
    public:
        Scene(long long xmin, long long xmax, long long ymin, long long ymax, 
                long long zmin, long long zmax);
        Scene(long long limits[6]);

        /* Add figures. */
        void addFigure(Geometry::Figure * fig);

        /* Sets the horizon's material. */
        void setHorizon(Material  * hor);

        /* Prints the whole scene on the screen. */
        void print();

        /* Gets the whole frame for the camera. */
        void getFrame(long dist2Near, long depth, float angle, double frame[6]);
};

#endif
