/**
 * This file sets everything about the generation of materials for the figures.
 *
 * @author  Roberto Fernandez Cueto
 * @date 27.11.2015
 *
 * $Id$
 */

#ifndef _MATERIAL_H
#define _MATERIAL_H

#ifndef _GENGINE_H
namespace GEngine {
    class Material;
};
#endif

struct Pixmap {
    unsigned char * data;
    int width, height;
};

/**
 * A material used to print some figure.
 */
class GEngine::Material {
    private:
        static unsigned nmaterials;
        void readBmpFile(const char * filename);
    public:
        enum GEmaterial {
            GL_SOLID_COLOR = 0,
            GL_FROM_FILE,
            GL_FROM_PIXELMAP
        };
        Material(GEmaterial type, void * data);
    protected:
        GEmaterial type;
        struct Pixmap texture;
};

#endif

