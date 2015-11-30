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

static const GLenum props[] = {
	GL_TEXTURE_MIN_FILTER,
	GL_TEXTURE_MAG_FILTER,
	GL_TEXTURE_MIN_LOD,
	GL_TEXTURE_MAX_LOD,
	GL_TEXTURE_BASE_LEVEL,
	GL_TEXTURE_MAX_LEVEL,
	GL_TEXTURE_WRAP_S,
	GL_TEXTURE_WRAP_T,
	GL_TEXTURE_WRAP_R,
	GL_TEXTURE_BORDER_COLOR,
	GL_TEXTURE_PRIORITY,
	GL_TEXTURE_COMPARE_MODE,
	GL_TEXTURE_COMPARE_FUNC,
	GL_DEPTH_TEXTURE_MODE,
	GL_GENERATE_MIPMAP
};

TextureMap * Material::defaults = NULL;

/**
 * Constructor for the material class.
 */
Material::Material(GEmaterial t, void * data)
{
	GLfloat * value;

    if (t < 0 || t > GL_FROM_PIXELMAP)
        return;
    type = t;

    /* Setting the right type of texture. */
    switch (type) {
        case GL_SOLID_COLOR:
            texture.data = (unsigned char *) malloc(3 * sizeof(unsigned char));
            if (texture.data != NULL)
                memcpy(texture.data, data, 3 * sizeof(unsigned char));
			texture.width = texture.height = 1;
            break;
        case GL_FROM_FILE:
            readBmpFile((const char *) data);
            break;
        case GL_FROM_PIXELMAP:          
            memcpy(&texture, data, sizeof(struct Pixmap));
            break;
    }

	/* Setting the default texture properties. */
	if (defaults == NULL) {
		defaults = new TextureMap();

		for (unsigned idx = 0; idx < (sizeof( props ) / sizeof(GLenum)); idx++) {
			if (props[idx] == GL_TEXTURE_BORDER_COLOR)
				value = (GLfloat *) malloc(4 * sizeof(float));
			else
				value = (GLfloat *) malloc(sizeof(float));

			if (value != NULL) {
				glGetTexParameterfv(GL_TEXTURE_2D, props[idx], value);
				(*defaults)[props[idx]] = value;
			}
		}
	}

	/* Setting a copy of the default properties for this texture. */
	properties = TextureMap(*defaults);
}

/**
 * Destructor of the class, and if it is the last class, frees the texture buffers.
 */
Material::~Material()
{
	if (texture.data != NULL)
		free( texture.data );
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
	GLubyte	header[54];

	/* Initialize the structure. */
	texture.data = NULL;

	/* Open the bmp file. */
	file = fopen(filename, "rb");
	if (file == NULL)
		return;

	/* Read the file. */
	if (fread(header, 1, 54, file) != 54)
		return;

	/* Check if it is a bmp file. */
	if (header[0] != 'B' || header[1] != 'M')
		return;

	/* Get the parameters of the bmp file. */
	dataPos = *(int *) &(header[0x0A]);
	texture.width = * (int *) &(header[0x12]);
	texture.height = * (int *) &(header[0x16]);
	size = * (int *) &(header[0x22]);

	/* Complete the information. */
	if (dataPos == 0)
		dataPos = 54;
	if (size == 0)
		size = 3 * texture.width * texture.height;

	/* Initialize the data. */
	texture.data = (unsigned char *) malloc(size * sizeof(unsigned char));
	if (texture.data != NULL)
		fread(texture.data, 1, size, file);

	fclose(file);
}

/**
 * Activates the texture for printing.
 */
void
Material::activate()
{
	TextureMap::iterator props;

	if (texture.data == NULL)
		return;

    /* If it is a simple color, print the color. */
    if (type == GL_SOLID_COLOR) {
        glColor3d(texture.data[0] / 0xFF, texture.data[1] / 0xFF, texture.data[2] / 0xFF);
        return;
    }

	glGenTextures(1, &buffer);
	glBindTexture(GL_TEXTURE_2D, buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height,
			0, GL_RGB, GL_UNSIGNED_BYTE, texture.data);

	/* Setting the properties of the object. */
	for (props = properties.begin(); props != properties.end(); props++) {
		glTexParameterfv(GL_TEXTURE_2D, props->first, props->second);
	}
}

/**
 * Deactivates the texture reemplacing it to the default one.
 */
void
Material::deactivate()
{
	TextureMap::iterator props;

	glBindTexture(GL_TEXTURE_2D, 0);
	glDeleteTextures(1, &buffer);

	/* Setting the properties to the default ones. */
	for (props = defaults->begin(); props != defaults->end(); props++) {
		glTexParameterfv(GL_TEXTURE_2D, props->first, props->second);
	}
}

/**
 * Sets a property for the texture class which must be in the range of the
 * texture properties.
 * @param	GLenum	prop	The property to be set.
 * @param	float	* value	The value to be set.
 */
void
Material::setProperty(GLenum prop, float * value)
{
	GLfloat * newVal;
	TextureMap::iterator it;
	int	size = 1;

	/* Search if it is a valid property or return. */
	if ((it = properties.find(prop)) == properties.end())
		return;

	if (prop == GL_TEXTURE_BORDER_COLOR)
		size = 4;

	newVal = (GLfloat *) malloc(sizeof(GLfloat) * size);
	if (newVal == NULL)
		return;

	memcpy(newVal, value, size);
	free( it->second );
	properties[prop] = newVal;
}

