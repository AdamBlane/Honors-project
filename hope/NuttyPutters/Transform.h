#pragma once

#include "include\glm-0.9.8.5\glm\glm\glm.hpp"
#include "include\glm-0.9.8.5\glm\glm\gtx\transform.hpp"

using namespace glm;

class Transform
{
public:
	Transform(const vec3& pos = vec3(), const vec3& rot = vec3(), const vec3& scale = vec3(1.0f, 1.0f, 1.0f)) :
		m_pos(pos),
		m_rot(rot),
		m_scale(scale) {}

	inline mat4 getModel() const
	{
		//Set 4x4 matrix for transformations
		mat4 posMatrix = translate(m_pos);
		mat4 rotXMatrix = rotate(m_rot.x, vec3(1, 0, 0));
		mat4 rotYMatrix = rotate(m_rot.y, vec3(0, 1, 0));
		mat4 rotZMatrix = rotate(m_rot.z, vec3(0, 0, 1));
		mat4 scaleMatrix = scale(m_scale);

		mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;

		return posMatrix * rotMatrix * scaleMatrix;
	}

	inline vec3& getPos() { return m_pos; }
	inline vec3& getRot() { return m_rot; }
	inline vec3& getScale() { return m_scale; }

	inline void setPos(vec3& pos) { m_pos = pos; }
	inline void setRot(vec3& rot) { m_rot = rot; }
	inline void setScale(vec3& scale) { m_scale = scale; }
protected:


private:
	vec3 m_pos;
	vec3 m_rot;
	vec3 m_scale;

};