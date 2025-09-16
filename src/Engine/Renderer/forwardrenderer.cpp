#include "Renderer/forwardrenderer.h"
#include "Renderer/Hosek/ArHosekSkyModel.h"
#include <imgui.h>
#include <cstdio> // for snprintf

#define SIMPLE_SKY
#define MAX_LIGHTS 8

void Draw3DBillboardRec(const Camera &camera, const Texture2D &texture, const Rectangle &source,
                        const Vector3 &position, const Vector2 &size, const Color &tint)
{
    rlPushMatrix();

    Matrix mat = MatrixInvert(MatrixLookAt(camera.position, camera.target, camera.up));
    Quaternion quat = QuaternionFromMatrix(mat);
    mat = QuaternionToMatrix(quat);

    rlMultMatrixf(MatrixToFloat(mat));

    Vector3 transformedPos = Vector3Transform(position, MatrixInvert(mat));
    rlTranslatef(transformedPos.x, transformedPos.y, transformedPos.z);

    float width = size.x / 2;
    float height = size.y / 2;

    rlCheckRenderBatchLimit(6);
    rlSetTexture(texture.id);

    rlBegin(RL_QUADS);
    rlColor4ub(tint.r, tint.g, tint.b, tint.a);

    rlTexCoord2f(source.x / texture.width, (source.y + source.height) / texture.height);
    rlVertex3f(-width, -height, 0);

    rlTexCoord2f((source.x + source.width) / texture.width, (source.y + source.height) / texture.height);
    rlVertex3f(width, -height, 0);

    rlTexCoord2f((source.x + source.width) / texture.width, source.y / texture.height);
    rlVertex3f(width, height, 0);

    rlTexCoord2f(source.x / texture.width, source.y / texture.height);
    rlVertex3f(-width, height, 0);

    rlEnd();
    rlSetTexture(0);
    rlPopMatrix();
}

// TODO: Clustered forward rendering, see https://github.com/DaveH355/clustered-shading
namespace BloxEngine
{
    Shader ForwardRenderer::m_forwardLitShader;
    Shader ForwardRenderer::m_alphaDiscardShader;
    Shader ForwardRenderer::m_dynamicSkyShader;

    // Simple 3d inverse cube shader
    Shader ForwardRenderer::m_skyboxShader;

    Model ForwardRenderer::m_skyboxMesh;

    int ForwardRenderer::m_lightCount = 0;

    static std::array<ForwardRenderer::LightUniformLocations, MAX_LIGHTS> g_lightUniforms;

    // Shader locations for dynamic sky
    int ForwardRenderer::loc_sunDir = -1;
    int ForwardRenderer::loc_turbidity = -1;
    int ForwardRenderer::loc_exposure = -1;
    int ForwardRenderer::loc_sunSize = -1;
    int ForwardRenderer::loc_groundAlbedo = -1;

    // TODO: Change into component
    static float g_timeOfDay = 9.0f;
    static float g_turbidity = 3.0f;
    static float g_groundAlbedo = 0.12f;
    static float g_sunSize = 0.0047f; // radians (~0.27 deg)
    static float g_exposure = 1.0f;

    void ForwardRenderer::Init()
    {
        m_forwardLitShader = LoadShader(
            "resources/engine_shaders/litmesh.vs",
            "resources/engine_shaders/litmesh.fs");

        InitializeLitShaderLocations();

        m_forwardLitShader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(m_forwardLitShader, "viewPos");

        int ambientLoc = GetShaderLocation(m_forwardLitShader, "ambient");
        SetShaderValue(m_forwardLitShader, ambientLoc, (float[4]){0.1f, 0.1f, 0.1f, 1.0f}, SHADER_UNIFORM_VEC4);

        m_alphaDiscardShader = LoadShader(NULL, "resources/engine_shaders/alpha_discard.fs");
    }

    void ForwardRenderer::InitializeLitShaderLocations()
    {
        const char *props[6] = {"enabled", "type", "position", "direction", "target", "color"};
        char nameBuffer[64];

        for (int i = 0; i < MAX_LIGHTS; i++)
        {
            snprintf(nameBuffer, sizeof(nameBuffer), "lights[%d].enabled", i);
            g_lightUniforms[i].enabled = GetShaderLocation(m_forwardLitShader, nameBuffer);

            snprintf(nameBuffer, sizeof(nameBuffer), "lights[%d].type", i);
            g_lightUniforms[i].type = GetShaderLocation(m_forwardLitShader, nameBuffer);

            snprintf(nameBuffer, sizeof(nameBuffer), "lights[%d].position", i);
            g_lightUniforms[i].position = GetShaderLocation(m_forwardLitShader, nameBuffer);

            snprintf(nameBuffer, sizeof(nameBuffer), "lights[%d].direction", i);
            g_lightUniforms[i].direction = GetShaderLocation(m_forwardLitShader, nameBuffer);

            snprintf(nameBuffer, sizeof(nameBuffer), "lights[%d].target", i);
            g_lightUniforms[i].target = GetShaderLocation(m_forwardLitShader, nameBuffer);

            snprintf(nameBuffer, sizeof(nameBuffer), "lights[%d].color", i);
            g_lightUniforms[i].color = GetShaderLocation(m_forwardLitShader, nameBuffer);
        }
    }

    // TODO: When developer who wants to use a dynamic sky shouldn't have unnecessary shaders and stuff like the simple cubic skybox
    //       So abstract this into a separate DynamicSky class or something
    void ForwardRenderer::InitSkybox()
    {
#ifndef SIMPLE_SKY
        // Setup dynamic sky shader
        m_dynamicSkyShader = LoadShader(
            "resources/engine_shaders/skymodel.vs",
            "resources/engine_shaders/skymodel.fs");

        loc_sunDir = GetShaderLocation(m_dynamicSkyShader, "u_sunDir");
        loc_turbidity = GetShaderLocation(m_dynamicSkyShader, "u_turbidity");
        loc_exposure = GetShaderLocation(m_dynamicSkyShader, "u_exposure");
        loc_sunSize = GetShaderLocation(m_dynamicSkyShader, "u_sunSize");
        loc_groundAlbedo = GetShaderLocation(m_dynamicSkyShader, "u_groundAlbedo");
#endif

        m_skyboxShader = LoadShader(
            "resources/engine_shaders/skybox.vs",
            "resources/engine_shaders/skybox.fs");

        m_skyboxMesh = LoadModelFromMesh(GenMeshCube(1.0f, 1.0f, 1.0f));
        m_skyboxMesh.materials[0].shader = m_skyboxShader;

        SetShaderValue(
            m_skyboxMesh.materials[0].shader,
            GetShaderLocation(m_skyboxMesh.materials[0].shader, "environmentMap"),
            (int[1]){MATERIAL_MAP_CUBEMAP},
            SHADER_UNIFORM_INT);

        SetShaderValue(
            m_skyboxMesh.materials[0].shader,
            GetShaderLocation(m_skyboxMesh.materials[0].shader, "doGamma"),
            (int[1]){0},
            SHADER_UNIFORM_INT);

        SetShaderValue(
            m_skyboxMesh.materials[0].shader,
            GetShaderLocation(m_skyboxMesh.materials[0].shader, "vflipped"),
            (int[1]){0},
            SHADER_UNIFORM_INT);

        m_skyboxMesh.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture = LoadTextureCubemap(LoadImage("resources/engine_textures/def_sky.png"), CUBEMAP_LAYOUT_AUTO_DETECT);
    }

    void ForwardRenderer::Shutdown()
    {
        UnloadShader(m_forwardLitShader);
        UnloadShader(m_alphaDiscardShader);
        UnloadShader(m_dynamicSkyShader);
        UnloadShader(m_skyboxShader);
        UnloadModel(m_skyboxMesh);
        UnloadTexture(m_skyboxMesh.materials[0].maps[MATERIAL_MAP_CUBEMAP].texture);
    }

    void ForwardRenderer::RenderGizmo(const Camera3D &camera, const Texture2D &icon, const Vector3 &position, float size, const Color &tint)
    {
        BeginShaderMode(m_alphaDiscardShader);
        {
            Draw3DBillboardRec(camera, icon,
                               {0, 0, (float)icon.width, (float)icon.height},
                               position, {size, size}, tint);
        }
        EndShaderMode();
    }

    void ForwardRenderer::BeginFrame(const Camera3D &camera)
    {
        m_lightCount = 0;
        SetShaderValue(m_forwardLitShader,
                       m_forwardLitShader.locs[SHADER_LOC_VECTOR_VIEW],
                       &camera.position,
                       SHADER_UNIFORM_VEC3);
    }

    // TODO: Merge with UpdateLightShader
    void ForwardRenderer::SubmitLight(const Vector3 &position, const Vector3 &direction, const LightComponent &light)
    {
        if (m_lightCount >= MAX_LIGHTS)
            return;
        UpdateLightShader(m_lightCount, position, direction, light);
        m_lightCount++;
    }

    void ForwardRenderer::UpdateLightShader(int index, const Vector3 &position, const Vector3 &direction, const LightComponent &light)
    {
        LightUniformLocations &loc = g_lightUniforms[index];

        int enabled = 1;
        int typeInt = static_cast<int>(light.type);
        Vector3 target = light.target;
        float color[4] = {
            light.color.r / 255.0f,
            light.color.g / 255.0f,
            light.color.b / 255.0f,
            light.color.a / 255.0f};

        SetShaderValue(m_forwardLitShader, loc.enabled, &enabled, SHADER_UNIFORM_INT);
        SetShaderValue(m_forwardLitShader, loc.type, &typeInt, SHADER_UNIFORM_INT);
        SetShaderValue(m_forwardLitShader, loc.position, &position, SHADER_UNIFORM_VEC3);
        SetShaderValue(m_forwardLitShader, loc.direction, &direction, SHADER_UNIFORM_VEC3);
        SetShaderValue(m_forwardLitShader, loc.target, &target, SHADER_UNIFORM_VEC3);
        SetShaderValue(m_forwardLitShader, loc.color, color, SHADER_UNIFORM_VEC4);
    }

    void ForwardRenderer::EndFrame()
    {
        // Future: send total light count to shader for clustered shading
        SetShaderValue(m_forwardLitShader, GetShaderLocation(m_forwardLitShader, "lightCount"), &m_lightCount, SHADER_UNIFORM_INT);

        ImGui::Begin("Renderer Info");
        ImGui::Text("Active lights: %d", m_lightCount);
        ImGui::Text("Current fps: %d", GetFPS());
        ImGui::End();
    }

    void ForwardRenderer::RenderSkybox()
    {
        // Instead of doing complex calculations we just draw the inside instead
        BeginShaderMode(m_skyboxShader);
        rlDisableDepthMask();
        rlDisableBackfaceCulling();

        DrawModel(m_skyboxMesh, {0, 0, 0}, 1000.0f, WHITE);

        rlEnableBackfaceCulling();
        rlEnableDepthMask();
        EndShaderMode();
    }

    void ForwardRenderer::RenderDynamicSky(const Camera3D &camera)
    {
        // ------------------- Compute Sun Direction -------------------
        float hour = fmodf(g_timeOfDay, 24.0f);
        float azimuth = (hour / 24.0f) * 2.0f * PI;
        float elevation = sinf((hour - 6.0f) / 12.0f * PI) * (70.0f * PI / 180.0f);
        elevation = Clamp(elevation, -PI / 2.0f, PI / 2.0f);

        Vector3 sunDir = {
            cosf(elevation) * cosf(azimuth),
            sinf(elevation),
            cosf(elevation) * sinf(azimuth)};

        SetShaderValue(m_dynamicSkyShader, loc_sunDir, &sunDir, SHADER_UNIFORM_VEC3);
        SetShaderValue(m_dynamicSkyShader, loc_turbidity, &g_turbidity, SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_dynamicSkyShader, loc_exposure, &g_exposure, SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_dynamicSkyShader, loc_sunSize, &g_sunSize, SHADER_UNIFORM_FLOAT);
        SetShaderValue(m_dynamicSkyShader, loc_groundAlbedo, &g_groundAlbedo, SHADER_UNIFORM_FLOAT);

        // ------------------- Generate Coefficients -------------------
        ArHosekSkyModelState *state = arhosek_rgb_skymodelstate_alloc_init(
            g_turbidity, g_groundAlbedo, elevation);

        float coeffsR[9], coeffsG[9], coeffsB[9];
        for (int i = 0; i < 9; i++)
        {
            coeffsR[i] = state->configs[0][i];
            coeffsG[i] = state->configs[1][i];
            coeffsB[i] = state->configs[2][i];
        }

        SetShaderValueV(m_dynamicSkyShader, GetShaderLocation(m_dynamicSkyShader, "skyR"),
                        coeffsR, SHADER_UNIFORM_FLOAT, 9);
        SetShaderValueV(m_dynamicSkyShader, GetShaderLocation(m_dynamicSkyShader, "skyG"),
                        coeffsG, SHADER_UNIFORM_FLOAT, 9);
        SetShaderValueV(m_dynamicSkyShader, GetShaderLocation(m_dynamicSkyShader, "skyB"),
                        coeffsB, SHADER_UNIFORM_FLOAT, 9);

        arhosekskymodelstate_free(state);

        // ------------------- Draw Sky Sphere -------------------
        BeginShaderMode(m_dynamicSkyShader);
        DrawSphere({0, 0, 0}, -1000.0f, WHITE);
        EndShaderMode();

        // ------------------- ImGui Editor -------------------
        if (ImGui::Begin("Dynamic Sky Settings"))
        {
            ImGui::Text("Sun Dir: (%.2f, %.2f, %.2f)", sunDir.x, sunDir.y, sunDir.z);
            ImGui::Text("Sun Zenith: %.2f rad", acosf(sunDir.y));

            ImGui::SliderFloat("Daytime", &g_timeOfDay, 0.0f, 24.0f);
            ImGui::SliderFloat("Turbidity", &g_turbidity, 1.0f, 10.0f);
            ImGui::SliderFloat("Sun Size (rad)", &g_sunSize, 0.0005f, 0.02f);
            ImGui::SliderFloat("Ground Albedo", &g_groundAlbedo, 0.0f, 1.0f);
            ImGui::SliderFloat("Exposure", &g_exposure, 0.1f, 8.0f);
        }
        ImGui::End();
    }
}
