/**
 * This file sets everything about the generation of materials for the figures.
 *
 * @author  Roberto Fernandez Cueto
 * @date 27.11.2015
 *
 * $Id$
 */

#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include <GL/gl.h>
#include <map>

namespace GEngine {
    class Material;
    class Texture;
    struct Pixmap;
};

struct GEngine::Pixmap {
    unsigned char * data;
    int width, height;
};

#define TextureMap	std::map<GLenum, GLfloat *>
#define MaterialMap TextureMap
#define PixmapMap   std::map<GLenum, struct GEngine::Pixmap *>

/**
 * A material used to print some figure.
 */
class GEngine::Material {
    private:
        static MaterialMap * matDefs;
    protected:
        MaterialMap material;
    public:
        Material();

		/* Activates/Deactivates this texture for 
		 * printing. */
		void activate();
		void deactivate();

		/* Sets a property for the texture/material. */
        void setMatProperty(GLenum prop, float * values);
};

/**
 * A texture used to make a figure have a different aspect.
 */
class GEngine::Texture {
    private:
        void readBmpFile(const char *filename);
        static TextureMap * texDefs;
    protected:
        PixmapMap pixmaps;
        unsigned buffer;
        TextureMap properties;
    public:
        Texture();
        ~Texture();

        /* Activates/Deactivates the texture. */
        void activate();
        void deactivate();

        /* Sets a property for the texture/Material.*/
        void setTexProperty(GLenum prop, float * values);
};
#endif

