#pragma once
#include "ggl.h"
#include "Shader.h"
#include "utils/Tools.h"
#include <vector>

namespace GLSLPT
{
	struct UniformTexture {
		unsigned int mTexture;
		int mLocation;
		UniformTexture() {
			mLocation = -1;
			mTexture = 0;
		}
	};

	struct UniformTextureCube {
		unsigned int mTexture;
		int mLocation;
		UniformTextureCube() {
			mLocation = -1;
			mTexture = 0;
		}
	};

	struct UniformVec4 {
		int mLocation;
		float v[4];
		UniformVec4() {
			mLocation = -1;
			memset(v, 0, sizeof(float) * 4);
		}
	};

	struct UniformFloat {
		int mLocation;
		float v;
		UniformFloat() {
			mLocation = -1;
			v = 0;
		}
	};

	struct UniformBool {
		int mLocation;
		bool v;
		UniformBool() {
			mLocation = -1;
			v = false;
		}
	};

    class Program
    {
    public:
        Program(const std::vector<Shader>& shaders);
        ~Program();

        void Active();
        void Deactive();
        GLuint Object();

	private:
		GLuint m_Object;
    };

    class CProgram {
    public:
		CProgram(std::string vertex_shader_file, std::string fragment_shader_file) {
			Init(vertex_shader_file, fragment_shader_file);
		}
		~CProgram() {
			glDeleteProgram(m_Program);
		}
		std::map<std::string, UniformTexture*>		mUniformTextures;
		std::map<std::string, UniformTextureCube*>	mUniformTextureCubes;
		std::map<std::string, UniformVec4*>			mUniformVec4s;
		std::map<std::string, UniformFloat*>		mUniformFloats;
		std::map<std::string, UniformBool*>			mUniformBools;
	public:
		void Init(std::string vertex_shader_file, std::string fragment_shader_file);
		void Active();
		void Deactive();
		GLuint Object();
		void SetTexture(const char* name, const char* image);
		unsigned int SetTexture(const char* name, unsigned int texture);
		void SetVec4(const char* name, float x, float y, float z, float w);
		void SetFloat(const char* name, float x);
		void SetBool(const char* name, bool x);
	private:
		unsigned int CompileShader(GLenum shaderType, const char* shaderCode);
		unsigned int CreateProgram(unsigned int vsShader, unsigned int fsShader);
		unsigned int CheckError(unsigned int candidate, GLenum pname);
		std::string PrewProcess(const std::string& path, bool bDebug = false);
	private:
		unsigned int m_Program;
    };

}
