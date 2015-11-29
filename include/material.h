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

#include <GL/gl.h>
#include <map>

#ifndef _GENGINE_H
namespace GEngine {
    class Material;
};
#endif

struct Pixmap {
    unsigned char * data;
    int width, height;
};

#define TextureMap	std::map<GLenum, GLfloat *>
/**
 * A material used to print some figure.
 * The figure should give the mapping between world
 * coordinates and texture coordinates.
 */
class GEngine::Material {
    private:
		void readBmpFile(const char * filename);
		static TextureMap * defaults;
    public:
        enum GEmaterial {
            GL_SOLID_COLOR = 0,
            GL_FROM_FILE,
            GL_FROM_PIXELMAP
        };
        Material(GEmaterial type, void * data);
		~Material();

		/* Activates/Deactivates this texture for 
		 * printing. */
		void activate();
		void deactivate();

		/* Sets a property for the texture. */
		void setProperty(GLenum prop, float * value);
    protected:
        GEmaterial type;
        struct Pixmap texture;
		unsigned buffer;
		TextureMap properties;
};

#endif

