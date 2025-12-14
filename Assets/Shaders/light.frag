#version 330 core

layout (location = 0) out vec4 FragColor;

in vec4 o_Color;
in vec2 o_UV;
in float o_TexIndex;
in vec3 o_WorldPos;

uniform sampler2D u_Textures[32];

#define MAX_LIGHTS 8

struct PointLight {
    vec3 position;
    vec3 color;
    float intensity;
    float radius;
};

uniform int u_LightCount;
uniform PointLight u_Lights[MAX_LIGHTS];

// 🌞 ضوء شمسي (اتجاهي)
uniform vec3 u_DirectionalDir;
uniform vec3 u_DirectionalColor;
uniform float u_DirectionalIntensity;

// 🔸 إضاءة بيئية (Ambient)
uniform vec3 u_AmbientColor;
uniform float u_AmbientStrength;

// 🔸 ظل عام
uniform float u_ShadowStrength;

void main()
{
    int index = clamp(int(o_TexIndex), 0, 31);
    vec4 texColor = texture(u_Textures[index], o_UV) * o_Color;

    // 🟢 نبدأ بالإضاءة البيئية
    vec3 totalLight = u_AmbientColor * u_AmbientStrength;

    // 🔆 نحسب مساهمة الضوء الاتجاهي
    vec3 dirLight = normalize(-u_DirectionalDir);
    float directionalFactor = clamp(dot(normalize(vec3(0.0, 0.0, 1.0)), dirLight), 0.0, 1.0);
    totalLight += u_DirectionalColor * directionalFactor * u_DirectionalIntensity;

    float closestDist = 999999.0;
    vec3 closestLightColor = vec3(1.0);
    float maxIntensity = 0.0;

    // 💡 نحسب كل ضوء نقطي
    for (int i = 0; i < u_LightCount; i++)
    {
        float dist = length(u_Lights[i].position.xy - o_WorldPos.xy);
        if (dist < u_Lights[i].radius)
        {
            float attenuation = 1.0 - (dist / u_Lights[i].radius);
            vec3 light = u_Lights[i].color * attenuation * u_Lights[i].intensity;
            totalLight += light;

            if (dist < closestDist)
            {
                closestDist = dist;
                closestLightColor = u_Lights[i].color;
            }

            maxIntensity = max(maxIntensity, attenuation);
        }
    }

    // 🌑 نحسب الظل بناءً على البُعد عن الضوء الأقرب
    float shadowFactor = (1.0 - maxIntensity) * u_ShadowStrength;
    vec3 shadowColor = mix(vec3(1.0), closestLightColor * 0.25, shadowFactor);

    vec3 finalColor = texColor.rgb * totalLight * shadowColor;
    FragColor = vec4(finalColor, texColor.a);
}
