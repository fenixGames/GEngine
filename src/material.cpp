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

static const GLenum matProps[] = {
    GL_AMBIENT,
    GL_DIFFUSE,
    GL_SPECULAR,
    GL_EMISSION,
    GL_SHININESS
};

TextureMap * Material::texDefs = NULL;
TextureMap * Material::matDefs = NULL;

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
        case GL_FROM_LIGHT_PARAMS:
            break;
    }

	/* Setting the default texture properties. */
	if (texDefs == NULL) {
		texDefs = new TextureMap();

		for (unsigned idx = 0; idx < (sizeof( props ) / sizeof(GLenum)); idx++) {
			if (props[idx] == GL_TEXTURE_BORDER_COLOR)
				value = (GLfloat *) malloc(4 * sizeof(float));
			else
				value = (GLfloat *) malloc(sizeof(float));

			if (value != NULL) {
				glGetTexParameterfv(GL_TEXTURE_2D, props[idx], value);
				(*texDefs)[props[idx]] = value;
			}
		}
	}

    /* Setting the default material properties. */
    if (matDefs == NULL) {
        matDefs = new MaterialMap();

        for (unsigned idx = 0; idx < (sizeof( matProps ) / sizeof(GLenum)); idx++) {
            if (matProps[idx] == GL_SHININESS)
                value = new float[1];
            else
                value = new float[4];

            if (value != NULL) {
                glGetMaterialfv(GL_FRONT, matProps[idx], value);
                (*matDefs)[matProps[idx]] = value;
            }
        }
    }
    
	/* Setting a copy of the default properties for this texture. */
	properties = TextureMap(*texDefs);
    material = MaterialMap( *matDefs);

    /* Generating the texture pointer. */
	glGenTextures(1, &buffer);
}

/**
 * Destructor of the class, and if it is the last class, frees the texture buffers.
 */
Material::~Material()
{
	if (texture.data != NULL)
		free( texture.data );
    glDeleteTextures(1, &buffer);
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
    MaterialMap::iterator mats;

	if (texture.data == NULL)
		return;

    /* If it is a simple color, print the color. */
    if (type == GL_SOLID_COLOR) {
        glColor3d(texture.data[0] / 0xFF, texture.data[1] / 0xFF, texture.data[2] / 0xFF);
        return;
    } else if (type == GL_FROM_LIGHT_PARAMS) {
        for (mats = material.begin(); mats != material.end(); mats++) {
            glMaterialfv(GL_FRONT_AND_BACK, mats->first, mats->second);
        }
        return;
    }

	glBindTexture(GL_TEXTURE_2D, buffer);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture.width, texture.height,
			0, GL_BGR, GL_UNSIGNED_BYTE, texture.data);

#ifdef DEBUG
    printf("texture = ");
    for (int idx = 0; idx < 3 * texture.width * texture.height; idx++)
        printf("%2.2hhx ", texture.data[idx]);
    printf("\n");
#endif

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
    GLfloat defs[4] = {0.0, 0.0, 0.0, 1.0};

    if (type == GL_FROM_LIGHT_PARAMS) {
        glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, defs);
        glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, defs);
        glMaterialfv(GL_FRONT_AND_BACK, GL_COLOR_INDEXES, defs);
        return;
    }

	/* Setting the properties to the default ones. */
	for (props = texDefs->begin(); props != texDefs->end(); props++) {
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
Material::setTexProperty(GLenum prop, float * value)
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

/**
 * Sets the properties of the material derivated from light reflexion and so on.
 * @param   GLenum  prop    The property to set.
 * @param   float   * value The values to set.
 */
void
Material::setMatProperty(GLenum prop, float * value)
{
    GLfloat * mat;
    MaterialMap::iterator it;
    int size = 4;

    /* Search the property. */
    if ((it = material.find(prop)) == material.end())
        return;

    if (prop == GL_SHININESS)
        size = 1;

    mat = new GLfloat[size];
    if (mat == NULL)
        return;

    memcpy(mat, value, size * sizeof(GLfloat));

    delete material[prop];
    material[prop] = mat;
}

