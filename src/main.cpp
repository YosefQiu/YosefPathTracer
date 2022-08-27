#include <time.h>
#include <math.h>
#include <string>

#include <glad/glad.h>
#include <GLFW/glfw3.h>


#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <ImGuizmo.h>

#include "ui/UI.h"

#include "core/Window.h"
#include "core/Scene.h"
#include "core/Renderer.h"
#include "core/TiledRenderer.h"
#include "core/PathTracer.h"

#include "utils/Tools.h"

#include "parser/SceneLoader.h"
#include "parser/GLBLoader.h"

#include "file/tinydir.h"


double			lastTime = 0;
std::string		shaderDir;
std::string		assetsDir;
std::string     hdrResDir;

GLWindow* 		wnd = nullptr;
GLSLPT::PathTracer*     pt = nullptr;
UI::Ui*         	myui = nullptr;

void Cleanup() {
	wnd->Close();
	myui->ImguiClose();
	glfwTerminate();
	delete pt;
}


void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	glViewport(0, 0, width, height);
}

void GL_Interaction() {
	glfwSetFramebufferSizeCallback(wnd->GetWnd(), framebuffer_size_callback);
	glfwSetKeyCallback(wnd->GetWnd(), key_callback);
}


int main(int argc, char** argv)
{
	
	std::string exePath = argv[0];
	std::string dirPath = exePath.substr(0, exePath.find_last_of("/\\")) + "/";
	for (int i = 0; i < dirPath.size(); ++i) {
		if (dirPath[i] == '\\') {
			dirPath[i] = '/';
		}
	}
	assetsDir = dirPath + "assets/";
	shaderDir = dirPath + "shaders/";
	hdrResDir = assetsDir + "HDR/";
	std::vector<std::string> pathDir = {assetsDir, hdrResDir, shaderDir};

	// 1. init and create wnd
	wnd = new GLWindow(1280, 720, "YosefPathTracer");
	wnd->Init();
	wnd->InitInteraction(GL_Interaction);

	// 2. init and create path tracing 
	pt = new GLSLPT::PathTracer;
	if (!pt->Init(pathDir, wnd)) return 1;
	// TODO TEST in INIT
	int frameWidth;
    int frameHeight;
    wnd->GetFrameBufferSize(wnd->GetWnd(), frameWidth, frameHeight);
    pt->m_scene->renderOptions.frameSize = Vector2(frameWidth, frameHeight);
	
    
	
	myui = new UI::Ui(wnd->GetWnd());
	myui->ImguiInit();


	pt->InitRenderer();

	
	while (!wnd->IsClose()) {
		myui->ImguiCreateWindow();
		wnd->Update(0.0f, 0.34f, 0.57f);

		double currTime = glfwGetTime();
		double passTime = currTime - lastTime;
		lastTime = currTime;

		pt->Render(myui, (float)passTime);

		myui->ImguiRender();

		wnd->SwapBuffer();
		wnd->PollEvents();
	}
    
	Cleanup();
    
    return 0;
}

