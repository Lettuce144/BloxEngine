#ifndef FORWARDRENDERER_H
#define FORWARDRENDERER_H
#pragma once

#include <raylib.h>
#include <rlgl.h>
#include <raymath.h>
#include "components/components.h"

void Draw3DBillboardRec(const Camera &camera, const Texture2D &texture, const Rectangle &source,
                        const Vector3 &position, const Vector2 &size, const Color &tint);

namespace BloxEngine
{
    class ForwardRenderer
    {
    public:
        static void Init();
        static void InitSkybox();
        static void Shutdown();

        static void BeginFrame(const Camera3D &camera);
        static void SubmitLight(const Vector3 &position, const Vector3 &direction, const LightComponent &light);
        static void EndFrame();
        static void RenderGizmo(const Camera3D &camera, const Texture2D &icon, const Vector3 &position, float size, const Color &tint);

        static void RenderSkybox();
        static void RenderDynamicSky(const Camera3D &camera);

        static Shader &GetForwardLitShader() { return m_forwardLitShader; }

        struct LightUniformLocations
        {
            int enabled;
            int type;
            int position;
            int direction;
            int target;
            int color;
        };

    private:
        static void UpdateLightShader(int index, const Vector3 &position, const Vector3 &direction, const LightComponent &light);
        static void InitializeLitShaderLocations();

        static Shader m_forwardLitShader;
        static Shader m_alphaDiscardShader;
        static Shader m_dynamicSkyShader;
        static Shader m_skyboxShader;

        static Model m_skyboxMesh;
        static Texture m_skyboxImage;

        static int m_lightCount;

        static int loc_sunDir;
        static int loc_turbidity;
        static int loc_exposure;
        static int loc_sunSize;
        static int loc_groundAlbedo;
    };
}
#endif
