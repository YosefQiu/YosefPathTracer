#include "PathTracer.h"

namespace GLSLPT {
    PathTracer::~PathTracer() {
        delete m_scene; m_scene = nullptr;
        delete m_renderer; m_renderer = nullptr;
    }
    bool PathTracer::Init(std::vector<std::string> path, GLWindow* wnd) {
        m_assetsDir = path[0];
        m_hdrResDir = path[1];
        m_shaderDir = path[2];
        m_wnd = wnd;
        if (!InitSceneFiles()) {
            std::cerr << "[ERROR] Fail to Init SceneFiles..." << std::endl;
            return false;
        }
        
        if (!InitScene()) {
            std::cerr << "[ERROR] Fail to Init Scene..." << std::endl;
            return false;
        }
        
        return true;
    }

    bool PathTracer::InitSceneFiles() {
        std::vector<std::string> files;
        UTILS::FILE::GetDirFiles(m_assetsDir, files);
        for (int i = 0; i < files.size(); ++i) {
            std::string& file = files[i];
            std::string name  = file.substr(file.find_last_of("/\\") + 1);
            m_scene_files.push_back(file);
            m_scene_names.push_back(name);
        }
        files.clear();
        UTILS::FILE::GetDirFiles(m_hdrResDir, files);
        for (int i = 0; i < files.size(); ++i) {
            std::string& file = files[i];
            std::string name  = file.substr(file.find_last_of("/\\") + 1);
            m_env_files.push_back(file);
            m_env_names.push_back(name);
        }
        return true;
    }
    bool PathTracer::InitScene() {
        m_sampleSceneIndex = 2;
	    LoadScene(m_scene_files[m_sampleSceneIndex]);
	    return true;
    }

    void PathTracer::LoadScene(const std::string& file)
    {
        if (m_scene) {
            delete m_scene;
            m_scene = nullptr;
        }
        m_scene = new Scene;
        std::string ext = file.substr(file.find_last_of(".") + 1);
        bool useGLB = false;
        if (ext == "glb") {
            useGLB = true;
            LoadSceneFromGLTF(file.c_str(), m_scene);
        }
        else if (ext == "scene") {
            LoadSceneFromFile(file.c_str(), m_scene, m_renderOptions);
        }
        if (m_scene->hdrData == nullptr) {
            m_scene->AddHDR(m_hdrResDir + "vignaioli_night_1k.hdr");
            m_renderOptions.useEnvMap = useGLB;
        }

        m_envMapIndex = 0;
        for (int i = 0; i < m_env_files.size(); ++i) {
            if (m_env_files[i] == m_scene->hdrFile)  {
                m_envMapIndex = i;
                break;
            }
        }
        m_scene->renderOptions = m_renderOptions;
    }
    bool PathTracer::InitRenderer() {
        if (m_renderer) {
            delete m_renderer;
            m_renderer = nullptr;
        }
        m_renderer = new TiledRenderer(m_scene, m_shaderDir);
        m_renderer->Init();
        return true;
    }

    void PathTracer::UpdateUIContext() {
        for (int i = 0; i < m_scene_names.size(); ++i) {
            m_ui->mSceneItems.push_back(m_scene_names[i].c_str());
        }
		for (int i = 0; i < m_env_names.size(); ++i) {
            m_ui->mEnvItems.push_back(m_env_names[i].c_str());
		}
        //std::cout << "[Debug] Test this function only call once" << std::endl;
    }
    void PathTracer::Render(UI::Ui* ui, float deltaTime) {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
        m_ui = ui;
        m_renderer->Render();
        static bool dummy = (UpdateUIContext(), true);

        if (SceneMsg.bActive) {
            m_sampleSceneIndex = SceneMsg.mIndex;
			LoadScene(m_scene_files[m_sampleSceneIndex]);
			glfwSetWindowSize(m_wnd->GetWnd(), m_scene->renderOptions.windowSize.x, m_scene->renderOptions.windowSize.y);
			InitRenderer();
            SceneMsg.bActive = false;
        }

        if (EnvMsg.bActive) {
            m_envMapIndex = EnvMsg.mIndex;
            m_scene->AddHDR(m_env_files[m_envMapIndex]);
            EnvMsg.bActive = false;
        }

        ui->mSampleCount = m_renderer->GetSampleCount();
		m_scene->Update(deltaTime);
		m_renderer->Update(deltaTime);
    }
}
