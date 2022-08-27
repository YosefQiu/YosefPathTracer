#include "UI.h"

Ui_PT_Msg SceneMsg;
Ui_PT_Msg EnvMsg;
Ui_PT_Msg DenoiseMsg;

namespace UI {
    void Ui::ImguiInit() {
        // Create and bind ImGui's CONTEXT
		#if defined(PLATFORM_MAC)
            const char* glsl_version = "#version 150";
        #else
            const char* glsl_version = "#version 130";
        #endif
		 IMGUI_CHECKVERSION();
		 ImGui::CreateContext();
		 ImGuiIO& io = ImGui::GetIO(); (void)io;
		 io.Fonts->AddFontFromFileTTF("../Res/Fronts/data-latin.ttf", 16.0f);
		 // Set imgui style
		 ImGui::StyleColorsDark();
		 ImGui_ImplGlfw_InitForOpenGL(pWind, true);
		 ImGui_ImplOpenGL3_Init(glsl_version);
    }

    void Ui::ImguiCreateWindow() {
        // create imgui window
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		ImGuizmo::SetOrthographic(true);
		ImGuizmo::BeginFrame();

		ImguiMenuBar();
		ImguiStatsDlg();
		ImguiVersionDlg();
		ImguiConfigDlg();
    }

	void Ui::ImguiClose() {
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

    void Ui::ImguiRender() {
        // Render imgui
		ImGui::Render();
		int display_w, display_h;
		glfwMakeContextCurrent(pWind);
		glfwGetFramebufferSize(pWind, &display_w, &display_h);
		glViewport(0, 0, display_w, display_h);
		auto& io = ImGui::GetIO();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }

    void Ui::ImguiMenuBar() {
        if (ImGui::BeginMainMenuBar())
		{
			if (ImGui::BeginMenu("Menu"))
			{
				ImGui::MenuItem("Config", NULL, &mbShowAppConfig);
				ImGui::MenuItem("Stats", NULL, &mbShowAppStats);

				if (ImGui::MenuItem("Quit", "Alt+F4")) {
					glfwSetWindowShouldClose(pWind, true);
				}

				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("File"))
			{
				ImGui::MenuItem("Open", NULL, &mbShowOpenDiag);
				ImGui::MenuItem("Save");
				if (mbShowOpenDiag)
				{
					//ImGuiFileDialog::Instance()->OpenDialog("ChooseFileDlgKey", "Choose File", nullptr, ".");
				}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Edit"))
			{
				if (ImGui::MenuItem("Undo", "CTRL+Z")) {}
				if (ImGui::MenuItem("Redo", "CTRL+Y", false, false)) {}  // Disabled item
				ImGui::Separator();
				if (ImGui::MenuItem("Cut", "CTRL+X")) {}
				if (ImGui::MenuItem("Copy", "CTRL+C")) {}
				if (ImGui::MenuItem("Paste", "CTRL+V")) {}
				ImGui::EndMenu();
			}
			if (ImGui::BeginMenu("Help"))
			{
				
				// Dlg -> Print Version info
				ImGui::MenuItem("Version", NULL, &mbShowVersionInfo);
				

				ImGui::EndMenu();
			}

			ImGui::EndMainMenuBar();
		}
    }

    void Ui::ImguiStatsDlg() {
		if (mbShowAppStats)
		{
			ImGuiIO& io = ImGui::GetIO();
			ImVec2 window_pos = ImVec2(3.0f, io.DisplaySize.y - 3.0f);
			ImVec2 window_pos_pivot = ImVec2(0.0f, 1.0f);
			ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);

			if (ImGui::Begin("Example: Simple overlay", NULL, ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav | ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoBackground | ImGuiWindowFlags_NoInputs))
			{
				ImGui::Text("%.3f ms/frame", 1000.0f / ImGui::GetIO().Framerate);
				ImGui::Text("%.1f FPS", ImGui::GetIO().Framerate);

				if (ImGui::IsMousePosValid())
				{
					mStatsMousePosX = io.MousePos.x;
					mStatsMousePosY = io.MousePos.y;
					ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
				}
				else {
					ImGui::Text("Mouse Position: <invalid>");
				}
			}
			ImGui::End();
		}
	}
	
    void Ui::ImguiVersionDlg() {
		if (mbShowVersionInfo)
		{
			ImGui::SetNextWindowSize(ImVec2(345, 365), ImGuiCond_FirstUseEver);
			ImGui::SetNextWindowPos(ImVec2(10, 275), ImGuiCond_FirstUseEver);

			if (ImGui::Begin("Version Info", &mbShowVersionInfo, ImGuiWindowFlags_None))
			{
				
				ImGui::Text("Program Name: OpenGL Path Tracing");
				ImGui::Text("Author: Yosef Qiu");
				ImGui::Text("Version: 1.0");
			}
			ImGui::End();
		}
	}

	void Ui::ImguiConfigDlg() {
		ImGui::SetNextWindowSize(ImVec2(300, 150), ImGuiCond_FirstUseEver);
		ImGui::SetNextWindowPos(ImVec2(10, 30), ImGuiCond_FirstUseEver);

		ImGui::Begin("Config", &mbShowAppConfig, ImGuiWindowFlags_HorizontalScrollbar);
		ImGui::Text("Samples: %d ", mSampleCount);
		static int scene_item_current = 2;
		if (ImGui::Combo("Scene", &scene_item_current, mSceneItems.data(), mSceneItems.size())) {
			SceneMsg.bActive = true;
			SceneMsg.mIndex = scene_item_current;
		}
		static int env_item_current = 0;
		if (ImGui::Combo("EnvMap", &env_item_current, mEnvItems.data(), mEnvItems.size())) {
			EnvMsg.bActive = true;
			EnvMsg.mIndex = env_item_current;
		}

		if (ImGui::Checkbox("Denoise", &mbDenoise)) {
			DenoiseMsg.bActive = mbDenoise;
		}

		ImGui::End();
	}





}
