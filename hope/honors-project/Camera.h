#pragma once
#include "include\glm-0.9.8.5\glm\glm\gtc\quaternion.hpp"
#include "include\glm-0.9.8.5\glm\glm\gtx\quaternion.hpp"
#include "include\glm-0.9.8.5\glm\glm\gtx\transform.hpp"
#include "include\glm-0.9.8.5\glm\glm\gtx\euler_angles.hpp"
#include "include\glm-0.9.8.5\glm\glm\glm.hpp"
#include "include\glm-0.9.8.5\glm\glm\gtc\type_ptr.hpp"

// Is this a new namespace being declared, if so why?
namespace AllCamera
{
	class camera
	{
		//basic camera
		protected:
			//posistion of camera 
			glm::vec3 posistion = glm::vec3(.0f, .0f, .0f);
			//target of camera 
			glm::vec3 target = glm::vec3(.0f, .0f, -1.0f);
			// orientation of the camera
			glm::vec3 up = glm::vec3(.0f, 1.0f, .0f);
			// The currently built view matrix since the last frame update
			glm::mat4 view;
			// The currently built projection matrix since the last call to set_projection
			glm::mat4 projection;

		public:
			//constructer
			camera() = default;
			//destructor
			virtual ~camera() {}

			const glm::vec3& get_Posistion() const { return posistion; }
			void set_Posistion(const glm::vec3 &value) { posistion = value; }

			const glm::vec3& get_Target() const { return target; }
			void set_Target(const glm::vec3 &value) { target = value; }

			const glm::vec3& get_Up() const { return up; }
			void set_Up(const glm::vec3 &value) { up = value; }

			const glm::mat4 get_View() const { return view; }
			const glm::mat4 get_Projection() const { return projection; }

			void set_projection(float fov, float aspect, float znear, float zfar)
			{
				projection = glm::perspective(fov, aspect, znear, zfar);
			}
			virtual void update(float delta_time) = 0;
	};
			
}




