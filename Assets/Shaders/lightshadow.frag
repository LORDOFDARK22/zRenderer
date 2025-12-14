#version 330 core

layout (location = 0) out vec4 FragColor;

in vec4 o_Color;
in vec2 o_UV;
in float o_TexIndex;
in vec2 o_FragPos;

uniform sampler2D u_Textures[32];

// Directional Light - إضاءة اتجاهية
uniform vec2 lightDir;          // اتجاه الضوء (مثلاً: (1.0, -1.0))
uniform vec3 lightColor;        // لون الضوء
uniform float dirIntensity;     // شدة الضوء

// Ambient - إضاءة محيطة
uniform vec3 ambientColor;
uniform float ambientIntensity;

// Point Lights - أضواء النقاط
#define MAX_POINT_LIGHTS 8
uniform int numPointLights;
uniform vec2 pointLightPos[MAX_POINT_LIGHTS];
uniform vec3 pointLightColor[MAX_POINT_LIGHTS];
uniform float pointLightIntensity[MAX_POINT_LIGHTS];
uniform float pointLightRadius[MAX_POINT_LIGHTS];

#define MAX_SPOT_LIGHTS 4
uniform int numSpotLights;
uniform vec2 spotLightPos[MAX_SPOT_LIGHTS];
uniform vec2 spotLightDir[MAX_SPOT_LIGHTS];     // اتجاه الكشاف
uniform vec3 spotLightColor[MAX_SPOT_LIGHTS];
uniform float spotLightIntensity[MAX_SPOT_LIGHTS];
uniform float spotLightRadius[MAX_SPOT_LIGHTS]; // نصف قطر الضوء
uniform float spotLightAngle[MAX_SPOT_LIGHTS];  // نصف زاوية الشعاع بالدرجات


vec3 CalcSpecular(vec3 lightDir, vec3 lightColor, float intensity, float shininess)
{
    // Normal في 2D دائمًا للأمام
    vec3 normal = vec3(0.0, 0.0, 1.0);
    vec3 viewDir = vec3(0.0, 0.0, 1.0);

    // حساب الانعكاس
    vec3 reflectDir = reflect(-lightDir, normal);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), shininess);

    return lightColor * intensity * spec;
}


// إضاءة اتجاهية مبسطة لـ 2D
vec3 CalcDirectionalLight()
{
    vec2 normalizedLightDir2D = normalize(lightDir);
    vec3 dirLightDir = vec3(normalizedLightDir2D, -1.0); // تحويل إلى 3D تقريبي

    float lightFactor = 0.7 + 0.3 * dot(normalizedLightDir2D, vec2(0.0, -1.0));
    vec3 diffuse = lightColor * dirIntensity * lightFactor;

    vec3 specular = CalcSpecular(dirLightDir, lightColor, dirIntensity, 16.0);

    return diffuse + specular;
}


// إضاءة النقاط (بدون normals)
vec3 CalcPointLights()
{
    vec3 result = vec3(0.0);

    for (int i = 0; i < numPointLights; i++)
    {
        vec2 toLight = pointLightPos[i] - o_FragPos;
        float distance = length(toLight);
        
        // إذا كانت المسافة أكبر من نصف قطر الضوء، تخطى
        if (distance > pointLightRadius[i]) 
            continue;

        // تخفيف شدة الضوء حسب المسافة
        float attenuation = 1.0 - smoothstep(0.0, pointLightRadius[i], distance);
        
        // تأثير إضافي: جعل الضوء أكثر طبيعية
        attenuation = attenuation * attenuation;
        
        result += pointLightColor[i] * pointLightIntensity[i] * attenuation;
    }

    return result;
}

vec3 CalcSpotLights()
{
    vec3 result = vec3(0.0);

    for (int i = 0; i < numSpotLights; i++)
    {
        vec2 toLight = o_FragPos - spotLightPos[i];
        float distance = length(toLight);

        // خارج نصف القطر → تجاهل
        if (distance > spotLightRadius[i])
            continue;

        // اتجاه البكسل بالنسبة للكشاف
        vec2 lightDirNorm = normalize(spotLightDir[i]);
        vec2 toPixelDir = normalize(-toLight); // من البكسل نحو مصدر الضوء
        float theta = dot(lightDirNorm, toPixelDir);

        // نصف زاوية الكشاف
        float cutoff = cos(radians(spotLightAngle[i]));

        if (theta < cutoff)
            continue; // خارج زاوية الكشاف

        // تخفيف حسب المسافة
        float attenuation = 1.0 - smoothstep(0.0, spotLightRadius[i], distance);
        attenuation = attenuation * attenuation;

        // تخفيف حسب زاوية الشعاع (مركزي أكثر → سطوع أعلى)
        float intensityFactor = smoothstep(cutoff, 1.0, theta);

        result += spotLightColor[i] * spotLightIntensity[i] * attenuation * intensityFactor;
    }

    return result;
}


void main()
{
    int index = clamp(int(o_TexIndex), 0, 31);
    vec4 texColor = texture(u_Textures[index], o_UV) * o_Color;
    
    // تجاهل البكسلات الشفافة تماماً
    if (texColor.a < 0.01)
        discard;

    // حساب الإضاءة
    vec3 lighting = ambientColor * ambientIntensity;
    lighting += CalcDirectionalLight();
    lighting += CalcPointLights();
    lighting += CalcSpotLights();  // <-- إضافة الكشاف هنا
    lighting = clamp(lighting, 0.0, 2.0);
    lighting = pow(lighting, vec3(1.0 / 2.2));
    FragColor = vec4(texColor.rgb * lighting, texColor.a);
}