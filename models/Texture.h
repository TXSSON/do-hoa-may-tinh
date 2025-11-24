#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>
#include <string>

/**
 * 👤 NGƯỜI 2: Texture Loader
 * Load và bind textures từ file ảnh
 */
class Texture
{
public:
    GLuint ID;
    std::string type; // diffuse, specular, normal...
    std::string path;

    /**
     * Load texture từ file
     * @param filepath Đường dẫn tới file ảnh (jpg, png...)
     * @param flipVertically Có lật ảnh theo chiều dọc không (thường dùng cho OpenGL)
     */
    Texture(const char *filepath, bool flipVertically = true);
    Texture(unsigned int width, unsigned int height, unsigned char* data, GLenum format = GL_RGBA);
    ~Texture();

    // Bind texture to a texture unit
    void bind(unsigned int unit = 0);
    void unbind();
    void setFiltering(GLenum minFilter, GLenum magFilter);

private:
    void loadFromFile(const char *path, bool flip);
};

#endif
