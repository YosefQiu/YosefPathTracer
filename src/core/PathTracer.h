#pragma once
#include "ggl.h"

#include "core/Scene.h"
#include "core/Renderer.h"
#include "core/TiledRenderer.h"
#include "core/Window.h"
#include "utils/Tools.h"

#include "ui/UI.h"

#include "parser/SceneLoader.h"
#include "parser/GLBLoader.h"
#include "file/tinydir.h"


namespace GLSLPT
{   
    class PathTracer
    {
    public:
        PathTracer() { }
        virtual ~PathTracer();
        bool Init(std::vector<std::string> path, GLWindow* wnd);
        void LoadScene(const std::string& file);
        bool InitSceneFiles();
        bool InitScene();
        bool InitRenderer();
        void Render(UI::Ui* ui, float deltaTime);
    private:
        void UpdateUIContext();
    public:
        int				m_sampleSceneIndex = -1; 
        int				m_envMapIndex = 0; 
        int				m_selectedInstance = 0;
        std::string		m_shaderDir;
        std::string		m_assetsDir;
        std::string     m_hdrResDir;
        std::vector<std::string> m_scene_files;
        std::vector<std::string> m_scene_names;
        std::vector<std::string> m_env_files;
        std::vector<std::string> m_env_names;
        Scene*          m_scene = nullptr;
        Renderer*		m_renderer = nullptr;
        RenderOptions	m_renderOptions;
        UI::Ui*         m_ui = nullptr;
        GLWindow* m_wnd = nullptr;
    };
} // namespace GLSLPT
