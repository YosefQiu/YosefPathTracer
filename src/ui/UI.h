#pragma once
#include "ggl.h"
#include "imgui.h"
#include "imgui_internal.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include <ImGuizmo.h>


namespace UI {
    class Ui
    {
    public:
		Ui() { }
		Ui(GLFWwindow* pWind) {
		    this->pWind = pWind;
	    };
	    virtual ~Ui() {
			if (pWind) {
                glfwDestroyWindow(pWind);
                pWind = nullptr;
			}
		};
    public:
        void ImguiInit();
        void ImguiCreateWindow();
        void ImguiRender();
		void ImguiClose();
    private:
        void ImguiMenuBar();
        void ImguiStatsDlg();
        void ImguiVersionDlg();	
		void ImguiConfigDlg();
    public:
	//** ------------
	//** Imgui Window parameters
	//** ------------
		GLFWwindow* pWind;
		bool mbShowWindow;
		bool mbShowAppStats = true;
		bool mbShowAppConfig = true;
		bool mbShowOpenDiag = true;
		bool mbShowVersionInfo = false;
	public:
	//** ------------
	//** Stats parameters
	//** ------------
		float mStatsMousePosX = 0.0f;
		float mStatsMousePosY = 0.0f;
	public:
	//** ------------
	//** Config parameters
	//** ------------
		int mSampleCount = 0;
		bool mbDenoise = false;
		std::vector<const char*> mSceneItems = {};
		std::vector<const char*> mEnvItems = {};
    };
} // namespace UI

