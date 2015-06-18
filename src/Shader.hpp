/**
    Shader.hpp

    @version    0.1
    @author     Miika 'LehdaRi' Lehtimäki
    @date       2015-06-18

**/


#ifndef ROBOKASI_SHADER_HPP
#define ROBOKASI_SHADER_HPP


//#include "LinearAlgebra.hpp"

#include <string>
#include <GL/glew.h>


class Shader {
public:
    Shader(const std::string& vsFileName, const std::string& fsFileName);
    ~Shader(void);

    GLuint getId(void) const;
    /*void useShader(const Matrix4Glf& mvp,
                   const Vector3Glf& color = Vector3Glf(1.0f, 1.0f, 1.0f)) const;*/
    void use(void);

private:
    GLuint programId_;
    GLuint uniformPosition_MVP_;
    GLuint uniformPosition_Color_;
};


#endif // ROBOKASI_SHADER_HPP
