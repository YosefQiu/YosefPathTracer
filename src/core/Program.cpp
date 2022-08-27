#include <stdexcept>

#include "Program.h"

namespace GLSLPT
{
    Program::Program(const std::vector<Shader>& shaders)
    {
        m_Object = glCreateProgram();
		for (unsigned i = 0; i < shaders.size(); i++) {
			glAttachShader(m_Object, shaders[i].Object());
		}
        
        glLinkProgram(m_Object);
		printf("Linking program %d\n", int(m_Object));
		for (unsigned i = 0; i < shaders.size(); i++) {
			glDetachShader(m_Object, shaders[i].Object());
		}
        
        GLint success = 0;
        glGetProgramiv(m_Object, GL_LINK_STATUS, &success);
        if (success == GL_FALSE)
        {
            std::string msg("Error while linking program\n");
            GLint logSize = 0;
            glGetProgramiv(m_Object, GL_INFO_LOG_LENGTH, &logSize);
            char* info = new char[logSize + 1];
            glGetShaderInfoLog(m_Object, logSize, NULL, info);
            msg += info;
            delete[] info;
            glDeleteProgram(m_Object);
            m_Object = 0;
            throw std::runtime_error(msg);
        }
    }

    Program::~Program()
    {
        glDeleteProgram(m_Object);
    }

    void Program::Active()
    {
        glUseProgram(m_Object);
    }

    void Program::Deactive()
    {
        glUseProgram(0);
    }

    GLuint Program::Object()
    {
        return m_Object;
    }

    std::string CProgram::PrewProcess(const std::string& path, bool bDebug) {
        UTILS::FILE::CFILE shaderFile(path, UTILS::FILE::CFILE::Mode::READ);
        std::vector<std::string> contents = shaderFile.ReadAllLines();

        std::string root = UTILS::STR::DelTailAfter(path, '/') + "/";
        std::string name = UTILS::STR::TailAfter(path, '/');

        std::string rst;

        for (const auto& line : contents) {
            if (UTILS::STR::IsBeginWith(line, "#include")) {
                std::string incPath = UTILS::STR::Between(line, '"', '"');
                std::string inc = PrewProcess(root + incPath);
                if (inc.empty()) {
                    std::cout << "[WARNING] #include is empty or failed" << "\n"
                        << "\t" << "code: " << line.c_str()
                        << "\t" << "path: " << incPath.c_str() << std::endl;
                    continue;
                }
                rst += inc;
            }
            else
                rst += line;
        }
        if (bDebug) {
            UTILS::FILE::CFILE debugShadeFile(root + "_DEBUG_" + name, UTILS::FILE::CFILE::Mode::WRITE);
            debugShadeFile.Printf("%s", rst.c_str());
            debugShadeFile.Close();
        }

        return rst;
    }

    unsigned int CProgram::CheckError(unsigned int candidate, GLenum pname) {
        int err = 0;
        int szlogLen = 0;
        char* szLog = new char[1024];
        memset(szLog, 0, 1024);
        switch (pname) {
        case GL_COMPILE_STATUS:
        {
            err = GL_TRUE;
            glGetShaderiv(candidate, GL_COMPILE_STATUS, &err);
            if (err == GL_FALSE) {
                glGetShaderInfoLog(candidate, 1024, &szlogLen, szLog);
                std::cout << "ERROR::Compile Shader fail-->error log : " << szLog << std::endl;
                return -1;
            }
        }
        break;
        case GL_LINK_STATUS:
        {
            err = GL_TRUE;
            glGetProgramiv(candidate, GL_LINK_STATUS, &err);
            if (err == GL_FALSE) {
                glGetProgramInfoLog(candidate, 1024, &szlogLen, szLog);
                std::cout << "[ERROR] Create gpu program fail-->error log : " << szLog << std::endl;
                return -1;
            }
        }
        break;
        }
        return 0;
    }

    unsigned int CProgram::CompileShader(GLenum shaderType, const char* shaderCode) {
        unsigned int shader = glCreateShader(shaderType);
        glShaderSource(shader, 1, &shaderCode, nullptr);
        glCompileShader(shader);
        if (CheckError(shader, GL_COMPILE_STATUS) == -1) {
            glDeleteShader(shader);
            shader = 0;
        }
        return shader;
    }

    unsigned int CProgram::CreateProgram(unsigned int vsShader, unsigned int fsShader) {
        unsigned int program = glCreateProgram();
        glAttachShader(program, vsShader);
        glAttachShader(program, fsShader);
       
        glLinkProgram(program);
        glDetachShader(program, vsShader);
        glDetachShader(program, fsShader);
        if (CheckError(program, GL_LINK_STATUS) == -1) {
            glDeleteProgram(program);
            program = 0;
        }
        return program;
    }

    void CProgram::Init(std::string vertex_shader_file, std::string fragment_shader_file) {
        unsigned int vsShader = -1, fsShader = -1, gsShader = -1, csShader = -1, esShader = -1;
        std::string shaderCode = PrewProcess(std::string(vertex_shader_file));
        vsShader = CompileShader(GL_VERTEX_SHADER, shaderCode.c_str());
        shaderCode.clear();
        shaderCode = PrewProcess(std::string(fragment_shader_file));
        fsShader = CompileShader(GL_FRAGMENT_SHADER, shaderCode.c_str());
        shaderCode.clear();
        
        m_Program = CreateProgram(vsShader, fsShader);
        glDeleteShader(vsShader);
        glDeleteShader(fsShader);
    }

    void CProgram::Active() {
        glUseProgram(m_Program);
    }

    void CProgram::Deactive() {
        glUseProgram(0);
    }

    GLuint CProgram::Object() {
        return m_Program;
    }


    void CProgram::SetVec4(const char* name, float x, float y, float z, float w) {
        auto iter = mUniformVec4s.find(name);
        if (iter == mUniformVec4s.end()) {
            int location = glGetUniformLocation(Object(), name);
            if (location != -1) {
                UniformVec4* v = new UniformVec4;
                v->v[0] = x;
                v->v[1] = y;
                v->v[2] = z;
                v->v[3] = w;
                v->mLocation = location;
                mUniformVec4s.insert(std::pair<std::string, UniformVec4*>(name, v));
            }
        }
        else {
            iter->second->v[0] = x;
            iter->second->v[1] = y;
            iter->second->v[2] = z;
            iter->second->v[3] = w;
        }
    }

    void CProgram::SetFloat(const char* name, float x) {
        auto iter = mUniformFloats.find(name);
        if (iter == mUniformFloats.end()) {
            int location = glGetUniformLocation(Object(), name);
            if (location != -1) {
                UniformFloat* v = new UniformFloat;
                v->v = x;
                v->mLocation = location;
                mUniformFloats.insert(std::pair<std::string, UniformFloat*>(name, v));
            }
        }
        else {
            iter->second->v = x;
        }
    }

    void CProgram::SetBool(const char* name, bool x) {
        auto iter = mUniformBools.find(name);
        if (iter == mUniformBools.end()) {
            int location = glGetUniformLocation(Object(), name);
            if (location != -1) {
                UniformBool* v = new UniformBool;
                v->v = x;
                v->mLocation = location;
                mUniformBools.insert(std::pair<std::string, UniformBool*>(name, v));
            }
        }
        else {
            iter->second->v = x;
        }
    }


    void CProgram::SetTexture(const char* name, const char* image) {

    }

    unsigned int CProgram::SetTexture(const char* name, unsigned int texture) { return 0; }





}