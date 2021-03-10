/*
   Copyright (c) 2015-2021 hkrn All rights reserved

   This file is part of emapp component and it's licensed under Mozilla Public License. see LICENSE.md for more details.
 */

#include "emapp/model/Material.h"

#include "emapp/Model.h"
#include "emapp/StringUtils.h"
#include "emapp/private/CommonInclude.h"

namespace nanoem {
namespace model {

static const nanoem_f32_t kMiniumSpecularPower = 0.1f;

void
Material::Color::reset(nanoem_f32_t v)
{
    m_ambient = Vector3(v);
    m_diffuse = Vector3(v);
    m_specular = Vector3(v);
    m_diffuseOpacity = v;
    m_specularPower = glm::max(v, kMiniumSpecularPower);
    m_diffuseTextureBlendFactor = m_sphereTextureBlendFactor = m_toonTextureBlendFactor = Vector4(v);
}

void
Material::Edge::reset(nanoem_f32_t v)
{
    m_color = Vector3(v);
    m_opacity = v;
    m_size = v;
}

Material::~Material() NANOEM_DECL_NOEXCEPT
{
}

void
Material::bind(nanoem_model_material_t *material)
{
    nanoem_parameter_assert(material, "must not be nullptr");
    nanoem_status_t status = NANOEM_STATUS_SUCCESS;
    nanoem_user_data_t *userData = nanoemUserDataCreate(&status);
    nanoemUserDataSetOnDestroyModelObjectCallback(userData, &Material::destroy);
    nanoemUserDataSetOpaqueData(userData, this);
    nanoemModelObjectSetUserData(nanoemModelMaterialGetModelObjectMutable(material), userData);
}

void
Material::resetLanguage(
    const nanoem_model_material_t *material, nanoem_unicode_string_factory_t *factory, nanoem_language_type_t language)
{
    StringUtils::getUtf8String(nanoemModelMaterialGetName(material, language), factory, m_name);
    if (m_canonicalName.empty()) {
        StringUtils::getUtf8String(
            nanoemModelMaterialGetName(material, NANOEM_LANGUAGE_TYPE_FIRST_ENUM), factory, m_canonicalName);
        if (m_canonicalName.empty()) {
            StringUtils::format(
                m_canonicalName, "Material%d", nanoemModelObjectGetIndex(nanoemModelMaterialGetModelObject(material)));
        }
    }
    if (m_name.empty()) {
        m_name = m_canonicalName;
    }
}

void
Material::reset(const nanoem_model_material_t *material) NANOEM_DECL_NOEXCEPT
{
    nanoem_parameter_assert(material, "must not be nullptr");
    Color &cb = m_color.base;
    cb.m_ambient = glm::make_vec3(nanoemModelMaterialGetAmbientColor(material));
    cb.m_diffuse = glm::make_vec3(nanoemModelMaterialGetDiffuseColor(material));
    cb.m_specular = glm::make_vec3(nanoemModelMaterialGetSpecularColor(material));
    cb.m_diffuseOpacity = nanoemModelMaterialGetDiffuseOpacity(material);
    cb.m_specularPower = glm::max(nanoemModelMaterialGetSpecularPower(material), kMiniumSpecularPower);
    cb.m_diffuseTextureBlendFactor = cb.m_sphereTextureBlendFactor = cb.m_toonTextureBlendFactor = Vector4(1);
    Edge &eb = m_edge.base;
    eb.m_color = glm::make_vec3(nanoemModelMaterialGetEdgeColor(material));
    eb.m_opacity = nanoemModelMaterialGetEdgeOpacity(material);
    eb.m_size = nanoemModelMaterialGetEdgeSize(material);
    m_color.mul.reset(1);
    m_color.add.reset(0);
    m_edge.mul.reset(1);
    m_edge.add.reset(0);
}

void
Material::update(const nanoem_model_morph_material_t *morph, nanoem_f32_t weight) NANOEM_DECL_NOEXCEPT
{
    nanoem_parameter_assert(morph, "must not be nullptr");
    const Vector4 diffuseTextureBlendFactor(glm::make_vec4(nanoemModelMorphMaterialGetDiffuseTextureBlend(morph)));
    const Vector4 sphereTextureBlendFactor(glm::make_vec4(nanoemModelMorphMaterialGetSphereMapTextureBlend(morph)));
    const Vector4 toonTextureBlendFactor(glm::make_vec4(nanoemModelMorphMaterialGetSphereMapTextureBlend(morph)));
    switch (nanoemModelMorphMaterialGetOperationType(morph)) {
    case NANOEM_MODEL_MORPH_MATERIAL_OPERATION_TYPE_MULTIPLY: {
        Color &cm = m_color.mul;
        static const Vector3 kOne(1.0f);
        cm.m_ambient *= glm::mix(kOne, glm::make_vec3(nanoemModelMorphMaterialGetAmbientColor(morph)), weight);
        cm.m_diffuse *= glm::mix(kOne, glm::make_vec3(nanoemModelMorphMaterialGetDiffuseColor(morph)), weight);
        cm.m_specular *= glm::mix(kOne, glm::make_vec3(nanoemModelMorphMaterialGetSpecularColor(morph)), weight);
        cm.m_diffuseOpacity = glm::mix(cm.m_diffuseOpacity, nanoemModelMorphMaterialGetDiffuseOpacity(morph), weight);
        cm.m_specularPower =
            glm::max(glm::mix(cm.m_specularPower, nanoemModelMorphMaterialGetSpecularPower(morph), weight),
                kMiniumSpecularPower);
        cm.m_diffuseTextureBlendFactor *= glm::mix(Vector4(1), diffuseTextureBlendFactor, weight);
        cm.m_sphereTextureBlendFactor *= glm::mix(Vector4(1), sphereTextureBlendFactor, weight);
        cm.m_toonTextureBlendFactor *= glm::mix(Vector4(1), toonTextureBlendFactor, weight);
        Edge &em = m_edge.mul;
        em.m_color *= glm::mix(kOne, glm::make_vec3(nanoemModelMorphMaterialGetEdgeColor(morph)), weight);
        em.m_opacity = glm::mix(em.m_opacity, nanoemModelMorphMaterialGetEdgeOpacity(morph), weight);
        em.m_size = glm::mix(em.m_size, nanoemModelMorphMaterialGetEdgeSize(morph), weight);
        break;
    }
    case NANOEM_MODEL_MORPH_MATERIAL_OPERATION_TYPE_ADD: {
        Color &ca = m_color.add;
        ca.m_ambient +=
            glm::mix(Constants::kZeroV3, glm::make_vec3(nanoemModelMorphMaterialGetAmbientColor(morph)), weight);
        ca.m_diffuse +=
            glm::mix(Constants::kZeroV3, glm::make_vec3(nanoemModelMorphMaterialGetDiffuseColor(morph)), weight);
        ca.m_specular +=
            glm::mix(Constants::kZeroV3, glm::make_vec3(nanoemModelMorphMaterialGetSpecularColor(morph)), weight);
        ca.m_diffuseOpacity = glm::mix(ca.m_diffuseOpacity, nanoemModelMorphMaterialGetDiffuseOpacity(morph), weight);
        ca.m_specularPower =
            glm::max(glm::mix(ca.m_specularPower, nanoemModelMorphMaterialGetSpecularPower(morph), weight),
                kMiniumSpecularPower);
        ca.m_diffuseTextureBlendFactor += glm::mix(Vector4(0), diffuseTextureBlendFactor, weight);
        ca.m_sphereTextureBlendFactor += glm::mix(Vector4(0), sphereTextureBlendFactor, weight);
        ca.m_toonTextureBlendFactor += glm::mix(Vector4(0), toonTextureBlendFactor, weight);
        Edge &ea = m_edge.add;
        ea.m_color += glm::mix(Constants::kZeroV3, glm::make_vec3(nanoemModelMorphMaterialGetEdgeColor(morph)), weight);
        ea.m_opacity = glm::mix(ea.m_opacity, nanoemModelMorphMaterialGetEdgeOpacity(morph), weight);
        ea.m_size = glm::mix(ea.m_size, nanoemModelMorphMaterialGetEdgeSize(morph), weight);
        break;
    }
    case NANOEM_MODEL_MORPH_MATERIAL_OPERATION_TYPE_MAX_ENUM:
    default:
        break;
    }
}

void
Material::destroy() NANOEM_DECL_NOEXCEPT
{
}

String
Material::name() const
{
    return m_name;
}

String
Material::canonicalName() const
{
    return m_canonicalName;
}

const char *
Material::nameConstString() const NANOEM_DECL_NOEXCEPT
{
    return m_name.c_str();
}

const char *
Material::canonicalNameConstString() const NANOEM_DECL_NOEXCEPT
{
    return m_canonicalName.c_str();
}

Material::Color
Material::base() const NANOEM_DECL_NOEXCEPT
{
    return m_color.base;
}

Material::Color
Material::mul() const NANOEM_DECL_NOEXCEPT
{
    return m_color.mul;
}

Material::Color
Material::add() const NANOEM_DECL_NOEXCEPT
{
    return m_color.add;
}

Material::Color
Material::color() const NANOEM_DECL_NOEXCEPT
{
    Color c;
    c.m_ambient = m_color.base.m_ambient * m_color.mul.m_ambient + m_color.add.m_ambient;
    c.m_diffuse = m_color.base.m_diffuse * m_color.mul.m_diffuse + m_color.add.m_diffuse;
    c.m_specular = m_color.base.m_specular * m_color.mul.m_specular + m_color.add.m_specular;
    c.m_diffuseOpacity = m_color.base.m_diffuseOpacity * m_color.mul.m_diffuseOpacity + m_color.add.m_diffuseOpacity;
    c.m_specularPower = glm::max(
        m_color.base.m_specularPower * m_color.mul.m_specularPower + m_color.add.m_specularPower, kMiniumSpecularPower);
    c.m_diffuseTextureBlendFactor = m_color.base.m_diffuseTextureBlendFactor * m_color.mul.m_diffuseTextureBlendFactor +
        m_color.add.m_diffuseTextureBlendFactor;
    c.m_sphereTextureBlendFactor = m_color.base.m_sphereTextureBlendFactor * m_color.mul.m_sphereTextureBlendFactor +
        m_color.add.m_sphereTextureBlendFactor;
    c.m_toonTextureBlendFactor = m_color.base.m_toonTextureBlendFactor * m_color.mul.m_toonTextureBlendFactor +
        m_color.add.m_toonTextureBlendFactor;
    return c;
}

Material::Edge
Material::edge() const NANOEM_DECL_NOEXCEPT
{
    Edge e;
    e.m_color = m_edge.base.m_color * m_edge.mul.m_color + m_edge.add.m_color;
    e.m_opacity = m_edge.base.m_opacity * m_edge.mul.m_opacity + m_edge.add.m_opacity;
    e.m_size = m_edge.base.m_size * m_edge.mul.m_size + m_edge.add.m_size;
    return e;
}

Material *
Material::cast(const nanoem_model_material_t *material) NANOEM_DECL_NOEXCEPT
{
    const nanoem_model_object_t *object = nanoemModelMaterialGetModelObject(material);
    const nanoem_user_data_t *userData = nanoemModelObjectGetUserData(object);
    return static_cast<Material *>(nanoemUserDataGetOpaqueData(userData));
}

Material *
Material::create(sg_image fallbackTexture)
{
    return nanoem_new(Material(fallbackTexture));
}

const IImageView *
Material::diffuseImage() const NANOEM_DECL_NOEXCEPT
{
    return m_diffuseImagePtr;
}

void
Material::setDiffuseImage(const IImageView *value)
{
    m_diffuseImagePtr = value;
}

const IImageView *
Material::sphereMapImage() const NANOEM_DECL_NOEXCEPT
{
    return m_sphereMapImagePtr;
}

void
Material::setSphereMapImage(const IImageView *value)
{
    m_sphereMapImagePtr = value;
}

const IImageView *
Material::toonImage() const NANOEM_DECL_NOEXCEPT
{
    return m_toonImagePtr;
}

void
Material::setToonImage(const IImageView *value)
{
    m_toonImagePtr = value;
}

const Effect *
Material::effect() const NANOEM_DECL_NOEXCEPT
{
    return m_effect;
}

Effect *
Material::effect()
{
    return m_effect;
}

void
Material::setEffect(Effect *value)
{
    m_effect = value;
}

UInt32HashMap
Material::indexHash() const
{
    return m_indexHash;
}

void
Material::setIndexHash(const UInt32HashMap &value)
{
    m_indexHash = value;
}

Vector4
Material::toonColor() const NANOEM_DECL_NOEXCEPT
{
    return m_toonColor;
}

void
Material::setToonColor(const Vector4 &value)
{
    m_toonColor = value;
}

bool
Material::isVisible() const NANOEM_DECL_NOEXCEPT
{
    return m_visible;
}

void
Material::setVisible(bool value)
{
    m_visible = value;
}

void
Material::destroy(void *opaque, nanoem_model_object_t * /* material */) NANOEM_DECL_NOEXCEPT
{
    nanoem_parameter_assert(opaque, "must not be nullptr");
    Material *self = static_cast<Material *>(opaque);
    nanoem_delete(self);
}

Material::Material(sg_image fallbackImage) NANOEM_DECL_NOEXCEPT : m_effect(nullptr),
                                                                  m_diffuseImagePtr(nullptr),
                                                                  m_sphereMapImagePtr(nullptr),
                                                                  m_toonImagePtr(nullptr),
                                                                  m_toonColor(1),
                                                                  m_visible(true)
{
    m_fallbackImage = fallbackImage;
}

} /* namespace model */
} /* namespace nanoem */