#pragma once
#include "ggl.h"

struct GLFWDelete {
    void operator() (GLFWwindow* pWnd) {
        std::cout << "Destroy GLFW Window Context" << std::endl;
		glfwDestroyWindow(pWnd);
    }
};

class GLWindow 
{
public:
    std::unique_ptr<GLFWwindow, GLFWDelete> mWnd;
	const char* mScreenName;
	int mScreenWidth;
	int mScreenHeight;
    int mFrameBufferWidth;
    int mFrameBufferHeight;
public:
    GLWindow(int width, int height, const char* name)  :
        mScreenWidth(width), mScreenHeight(height), mScreenName(name)
    { }
    virtual ~GLWindow() {
        Close();
    }
public:
    bool Init(bool bShowVersion = true) {
		if (!InitGLFW())
			return false;
		if (!InitGLContext())
			return false;
		if (!InitGLAD())
			return false;
		InitOpenGLVersion(bShowVersion);
		glViewport(0, 0, mScreenWidth, mScreenHeight);
		glEnable(GL_DEPTH_TEST);
		glDepthFunc(GL_LEQUAL);
		return true;
	}
	bool IsClose() const {
		return glfwWindowShouldClose(GetWnd());
	}
	void InitInteraction(std::function<void()> foo) {
		foo();
	}
	void PollEvents() const {
		glfwPollEvents();
	}
	void SwapBuffer() const {
		glfwSwapBuffers(GetWnd());
	}
	void Close() const {
		glfwTerminate();
	}
	void Update(float r, float g, float b, float a = 1.0f) {
		glClearColor(r, g, b, a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}
    void GetFrameBufferSize(GLFWwindow* wnd, int& width, int& height) {
        glfwGetFramebufferSize(wnd, &mFrameBufferWidth, &mFrameBufferHeight);
        width = mFrameBufferWidth;
        height = mFrameBufferHeight;
    }
    GLFWwindow* GetWnd() const  {
        return mWnd.get();
    }
private:
    static void OnGLFWErrorCallback(int error, const char* description) {
        std::cerr << "[ERROR] GLFW " << error << " " << description << std::endl;
    }
	bool InitGLFW() {
        glfwSetErrorCallback(OnGLFWErrorCallback);
		if (!glfwInit()) {
			std::cout << "[ERROR] Failed to Init GLFW" << std::endl;
			return false;
		}
		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 1);
		glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
		glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
		glfwWindowHint(GLFW_RESIZABLE, false);

		std::cout << "[1] Init GLFW...." << std::endl;
		return true;
	}
	bool InitGLContext() {
		mWnd.reset(glfwCreateWindow(mScreenWidth, mScreenHeight, mScreenName, NULL, NULL));
		if (mWnd == NULL) {
			std::cout << "[ERROR] Failed to create OpenGL Context" << std::endl;
			return false;
		}
		else {
			glfwMakeContextCurrent(GetWnd());
			std::cout << "[2] Init OpenGL Context..." << std::endl;
			return true;
		}
	}
	bool InitGLAD() {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			std::cout << "[ERROR] Failed to initialize GLAD" << std::endl;
			return false;
		}
		else {
			std::cout << "[3] Init GLAD..." << std::endl;
			return true;
		}
	}
	void InitOpenGLVersion(bool bShowVersion) {
		if (bShowVersion) {
			std::cout << "[4] Getting Version Information..." << std::endl;
			std::cout << "================================================================================================" << std::endl;
			std::cout << "==      OpenGL Version: " << glGetString(GL_VERSION) << std::endl;
			std::cout << "==      OpenGL Shading Language Version: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << std::endl;
			std::cout << "==      GPU Version: " << glGetString(GL_VENDOR) << " " << glGetString(GL_RENDERER) << std::endl;
			std::cout << "================================================================================================" << std::endl;
		}
	}
};