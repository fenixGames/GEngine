/**
 * This file sets everything about the generation of materials for the figures.
 *
 * @author  Roberto Fernandez Cueto
 * @date 27.11.2015
 *
 * $Id$
 */

#include <GL/gl.h>
#include "material.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

using namespace GEngine;

unsigned Material::nmaterials = 0;

/**
 * Constructor for the material class.
 */
Material::Material(GEmaterial t, void * data)
{
    if (t < 0 || t > GL_FROM_PIXELMAP)
        return;

    type = t;

    /* Setting the right type of texture. */
    switch (type) {
        case GL_SOLID_COLOR:
            texture.data = (unsigned char *) malloc(3 * sizeof(unsigned char));
            if (texture.data != NULL)
                memcpy(texture.data, data, 3 * sizeof(unsigned char));
            break;
        case GL_FROM_FILE:
            readBmpFile((const char *) data);
            break;
        case GL_FROM_PIXELMAP:          
            memcpy(&texture, data, sizeof(struct Pixmap));
            break;
    }
}

/**
 * Reads a BMP file and parses the content into this material.
 * @param   char    *filename   The name of the file to load.
 */
void
Material::readBmpFile(const char * filename)
{
    FILE    * file;
    int     dataPos, size;


}

