#ifndef UNIFORM_H
#define UNIFORM_H

/*#include "../core/Ref.h"
#include "../math/Vector2.h"
#include "../math/Vector3.h"
#include "../math/Vector4.h"
#include "../math/Matrix.h"*/
#include "../graphics/Texture.h"

namespace gameplay
{



//------------------------------------------------------------------------------
// Uniforms
//------------------------------------------------------------------------------

enum UniformType
{
    UT_SAMPLER,             /// Int, used for samplers only.
    UT_VECTOR4,             /// 4 floats vector.
    UT_MATRIX3,             /// 3x3 matrix.
    UT_MATRIX4,             /// 4x4 matrix.
};

struct UniformInfo
{
    std::string name;       /// Uniform name.
    UniformType type;       /// Uniform type.
    unsigned short num;     /// Number of elements in array.
};








class Vector2;
class Vector3;
class Vector4;
class Matrix;
class Effect;

/**
 * Represents a uniform variable within an effect.
 */
class Uniform
{


    friend class Effect;

public:

    /**
     * Returns the name of this uniform.
     *
     * @return The name of the uniform.
     */
    const char* getName() const;

    /**
     * Returns the OpenGL uniform type.
     *
     * @return The OpenGL uniform type.
     */
    const GLenum getType() const;

    /**
     * Returns the effect for this uniform.
     *
     * @return The uniform's effect.
     */
    Effect* getEffect() const;



    const unsigned int getIndex() const;



    /**
     * Sets a float uniform value.
     *
     * @param uniform The uniform to set.
     * @param value The float value to set.
     */
    virtual void setValue(float value);

    /**
     * Sets a float array uniform value.
     *
     * @param uniform The uniform to set.
     * @param values The array to set.
     * @param count The number of elements in the array.
     */
    virtual void setValue(const float* values, unsigned int count = 1);

    /**
     * Sets an integer uniform value.
     *
     * @param uniform The uniform to set.
     * @param value The value to set.
     */
    virtual void setValue(int value);

    /**
     * Sets an integer array uniform value.
     *
     * @param uniform The uniform to set.
     * @param values The array to set.
     * @param count The number of elements in the array.
     */
    virtual void setValue(const int* values, unsigned int count = 1);

    /**
     * Sets a matrix uniform value.
     *
     * @param uniform The uniform to set.
     * @param value The value to set.
     */
    virtual void setValue(const Matrix& value);

    /**
     * Sets a matrix array uniform value.
     *
     * @param uniform The uniform to set.
     * @param values The array to set.
     * @param count The number of elements in the array.
     */
    virtual void setValue(const Matrix* values, unsigned int count = 1);

    /**
     * Sets a vector uniform value.
     *
     * @param uniform The uniform to set.
     * @param value The value to set.
     */
    virtual void setValue(const Vector2& value);

    /**
     * Sets a vector array uniform value.
     *
     * @param uniform The uniform to set.
     * @param values The array to set.
     * @param count The number of elements in the array.
     */
    virtual void setValue(const Vector2* values, unsigned int count = 1);

    /**
     * Sets a vector uniform value.
     *
     * @param uniform The uniform to set.
     * @param value The value to set.
     */
    virtual void setValue(const Vector3& value);

    /**
     * Sets a vector array uniform value.
     *
     * @param uniform The uniform to set.
     * @param values The array to set.
     * @param count The number of elements in the array.
     */
    virtual void setValue(const Vector3* values, unsigned int count = 1);

    /**
     * Sets a vector uniform value.
     *
     * @param uniform The uniform to set.
     * @param value The value to set.
     */
    virtual void setValue(const Vector4& value);

    /**
     * Sets a vector array uniform value.
     *
     * @param uniform The uniform to set.
     * @param values The array to set.
     * @param count The number of elements in the array.
     */
    virtual void setValue(const Vector4* values, unsigned int count = 1);

    /**
     * Sets a sampler uniform value.
     *
     * @param uniform The uniform to set.
     * @param sampler The sampler to set.
     */
    virtual void setValue(const Texture::Sampler* sampler);

    /**
     * Sets a sampler array uniform value.
     *
     * @param uniform The uniform to set.
     * @param values The sampler array to set.
     * @param count The number of elements in the array.
     *
     * @script{ignore}
     */
    virtual void setValue(const Texture::Sampler** values, unsigned int count);











    /**
     * Constructor.
     */
    Uniform();

    /**
     * Copy constructor.
     */
    Uniform(const Uniform& copy);

    /**
     * Destructor.
     */
    ~Uniform();

    /**
     * Hidden copy assignment operator.
     */
    Uniform& operator=(const Uniform&);

private:

    std::string _name;
    GLint _location;
    GLenum _type;
    unsigned int _index;
    Effect* _effect;
};

}

#endif // UNIFORM_H
