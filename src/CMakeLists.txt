SET(BVH_HDRS
    bvh/BvhTranslator.h
    bvh/Bvh.h
    bvh/SplitBvh.h
)
SET(BVH_SRCS
    bvh/BvhTranslator.cpp
    bvh/Bvh.cpp
    bvh/SplitBvh.cpp
)

SET(CORE_HDRS
    core/Light.h
    core/Camera.h
    core/Material.h
    core/Mesh.h
    core/Program.h
    core/Quad.h
    core/Renderer.h
    core/Scene.h
    core/Shader.h
    core/ShaderIncludes.h
    core/Texture.h
    core/TiledRenderer.h
    core/Window.h
    core/PathTracer.h
)
SET(CORE_SRCS
    core/Camera.cpp
    core/Mesh.cpp
    core/Program.cpp
    core/Quad.cpp
    core/Renderer.cpp
    core/Scene.cpp
    core/Shader.cpp
    core/Texture.cpp
    core/TiledRenderer.cpp
    core/PathTracer.cpp
)

SET(PARSER_HDRS
    parser/stb_image_resize.h
    parser/stb_image_write.h
    parser/stb_image.h
    parser/SceneLoader.h
    parser/HDRLoader.h
    parser/tiny_obj_loader.h
    parser/tiny_gltf.h
    parser/json.hpp
    parser/GLBLoader.h
)
SET(PARSER_SRCS
    parser/SceneLoader.cpp
    parser/HDRLoader.cpp
    parser/GLBLoader.cpp
)

SET(MATH_HDRS
    math/Bounds3D.h
    math/Math.h
    math/Axis.h
    math/Vector2.h
    math/Vector3.h
    math/Vector4.h
    math/Matrix4x4.h
    math/Quat.h
)
SET(MATH_SRCS
    math/Math.cpp
)

SET(UTILS_HDRS
    utils/Tools.h
)

SET(UTILS_SRCS
    utils/Tools.cpp
)


SET(JOB_HDRS
    job/Runnable.h
    job/RunnableThread.h
    job/TaskThread.h
    job/TaskThreadPool.h
    job/ThreadEvent.h
    job/ThreadManager.h
    job/ThreadTask.h
)
SET(JOB_SRCS
    job/RunnableThread.cpp
    job/TaskThread.cpp
    job/TaskThreadPool.cpp
    job/ThreadEvent.cpp
    job/ThreadManager.cpp
)

SET(FILE_HDRS
    file/tinydir.h
)
SET(FILE_SRCS
  
)

set(UI_HDRS
    ui/UI.h
)
SET(UI_SRCS
    ui/UI.cpp
)

SET(GFX_HDRS
    gfx/GfxShader.h
    gfx/GfxTexture.h
)
SET(GFX_SRCS
    gfx/GfxShader.cpp
    gfx/GfxTexture.cpp
)

ADD_LIBRARY(Core STATIC
	${BVH_HDRS}
	${BVH_SRCS}

	${CORE_HDRS}
    ${CORE_SRCS}
    
    ${PARSER_SRCS}
    ${PARSER_HDRS}

    ${MATH_HDRS}
    ${MATH_SRCS}

    ${UTILS_HDRS}
    ${UTILS_SRCS}
	
	${UI_HDRS}
    ${UI_SRCS}

    ${JOB_HDRS}
    ${JOB_SRCS}

    ${FILE_HDRS}
    ${FILE_SRCS}

    ${GFX_HDRS}
    ${GFX_SRCS}
)

SOURCE_GROUP(src\\gfx FILES ${GFX_SRCS} ${GFX_HDRS})
SOURCE_GROUP(src\\file FILES ${FILE_HDRS} ${FILE_SRCS})
SOURCE_GROUP(src\\job FILES ${JOB_HDRS} ${JOB_SRCS})
SOURCE_GROUP(src\\utils FILES ${UTILS_HDRS} ${UTILS_SRCS})
SOURCE_GROUP(src\\ui FILES ${UI_HDRS} ${UI_SRCS})
SOURCE_GROUP(src\\math FILES ${MATH_SRCS} ${MATH_HDRS})
SOURCE_GROUP(src\\parser FILES ${PARSER_SRCS} ${PARSER_HDRS})
SOURCE_GROUP(src\\bvh FILES ${BVH_HDRS} ${BVH_SRCS})
SOURCE_GROUP(src\\core FILES ${CORE_HDRS} ${CORE_SRCS})
