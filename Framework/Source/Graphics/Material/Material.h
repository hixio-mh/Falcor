/***************************************************************************
# Copyright (c) 2015, NVIDIA CORPORATION. All rights reserved.
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#  * Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
#  * Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#  * Neither the name of NVIDIA CORPORATION nor the names of its
#    contributors may be used to endorse or promote products derived
#    from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS ``AS IS'' AND ANY
# EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
# IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
# PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE COPYRIGHT OWNER OR
# CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
# PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
# PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
# OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
***************************************************************************/
#pragma once
#include "glm/vec3.hpp"
#include <map>
#include <vector>
#include "glm/common.hpp"
#include "glm/geometric.hpp"
#include "API/Texture.h"
#include "glm/mat4x4.hpp"
#include "API/Sampler.h"
#include "Data/HostDeviceData.h"
#include "Graphics/Program/ParameterBlock.h"

namespace Falcor
{
    class Texture;
    class ProgramVars;
    class ConstantBuffer;

    class Material : public std::enable_shared_from_this<Material>
    {
    public:
        using SharedPtr = std::shared_ptr<Material>;
        using SharedConstPtr = std::shared_ptr<const Material>;

        /** Create a new material.
            \param[in] name The material name
        */
        static SharedPtr create(const std::string& name);

        ~Material();

        /** Set the material name.
        */
        void setName(const std::string& name) { mName = name; }

        /** Get the material name.
        */
        const std::string& getName() const { return mName; }

        /** Get the material ID.
        */
        const int32_t getId() const { return mData.id; }

        /** Set the material ID
        */
        void setID(int32_t id);

        /** Reset all global id counter of model, mesh and material
        */
        static void resetGlobalIdCounter();

        /** Set the diffuse texture
        */
        void setDiffuseTexture(Texture::SharedPtr& pDiffuse);

        /** Get the diffuse texture
        */
        Texture::SharedPtr getDiffuseTexture() const { return mData.textures.diffuse; }

        /** Set the specular texture
        */
        void setSpecularTexture(Texture::SharedPtr pSpecular);

        /** Get the specular texture
        */
        Texture::SharedPtr getSpecularTexture() const { return mData.textures.specular; }

        /** Set the emissive texture
        */
        void setEmissiveTexture(Texture::SharedPtr& pEmissive);

        /** Get the emissive texture
        */
        Texture::SharedPtr getEmissiveTexture() const { return mData.textures.emissive; }

        /** Set the shading model
        */
        void setShadingModel(uint32_t model);

        /** Get the shading model
        */
        uint32_t getShadingModel() const { return EXTRACT_SHADING_MODEL(mData.flags); }

        /** Set the normal map
        */
        void setNormalMap(Texture::SharedPtr pNormalMap);

        /** Get the normal map
        */
        Texture::SharedPtr getNormalMap() const { return mData.textures.normalMap; }

        /** Set the occlusion map
        */
        void setOcclusionMap(Texture::SharedPtr pOcclusionMap);

        /** Get the occlusion map
        */
        Texture::SharedPtr getOcclusionMap() const { return mData.textures.occlusionMap; }

        /** Set the reflection map
        */
        void setReflectionMap(Texture::SharedPtr pReflectionMap);

        /** Get the occlusion map
        */
        Texture::SharedPtr getReflectionnMap() const { return mData.textures.reflectionMap; }

        /** Set the light map
        */
        void setLightMap(Texture::SharedPtr pLightMap);

        /** Get the light map
        */
        Texture::SharedPtr getLightMap() const { return mData.textures.lightMap; }

        /** Set the height map
        */
        void setHeightMap(Texture::SharedPtr pHeightMap);

        /** Get the height map
        */
        Texture::SharedPtr getHeightMap() const { return mData.textures.heightMap; }

        /** Set the diffuse color
        */
        void setDiffuseColor(const vec4& color);

        /** Get the diffuse color
        */
        const vec4& getDiffuseColor() const { return mData.diffuse; }

        /** Set the specular color
        */
        void setSpecularColor(const vec4& color);

        /** Get the specular color
        */
        const vec4& getSpecularColor() const { return mData.specular; }

        /** Set the emissive color
        */
        void setEmissiveColor(const vec3& color);

        /** Get the emissive color
        */
        const vec3& getSEmissiveColor() const { return mData.emissive; }

        /** Set the alpha mode
        */
        void setAlphaMode(uint32_t alphaMode);

        /** Get the alpha mode
        */
        uint32_t getAlphaMode() const { return EXTRACT_ALPHA_MODE(mData.flags); }

        /** Set the double-sided flag. This flag doesn't affect the rasterizer state, just the shading
        */
        void setDoubleSided(bool doubleSided);

        /** Returns true if the material is double-sided
        */
        bool getDoubleSided() const { return EXTRACT_DOUBLE_SIDED(mData.flags); }

        /** Set the alpha threshold. The threshold is only used if the alpha mode is `AlphaModeMask`
        */
        void setAlphaThreshold(float alpha);

        /** Get the alpha threshold
        */
        float getAlphaThreshold() const { return mData.alphaThreshold; }

        /** Set the NDF type
        */
        void setNdfType(uint32_t ndfType);

        /** Get the NDF type
        */
        uint32_t getNdfType() const { return EXTRACT_NDF_TYPE(mData.flags); }

        /** Get the flags
        */
        uint32_t getFlags() const { return mData.flags; }

        /** Set the height scale and offset
        */
        void setHeightScaleOffset(float scale, float offset);

        /** Get the height scale
        */
        float getHeightScale() const { return mData.heightScaleOffset.x; }

        /** Get the height offset
        */
        float getHeightOffset() const { return mData.heightScaleOffset.y; }

        /** Set the index of refraction
        */
        void setIndexOfRefraction(float IoR);

        /** Get the index of refraction
        */
        float getIndexOfRefraction() const { return mData.IoR; }

        /** Comparison operator
        */
        bool operator==(const Material& other) const;

        /** Bind a sampler to the material
        */
        void setSampler(Sampler::SharedPtr pSampler);

        /** Get the sampler attached to the material
        */
        Sampler::SharedPtr getSampler() const { return mData.samplerState; }

        /** Bind the material to a program variables object
        */
        void setIntoProgramVars(ProgramVars* pVars, ConstantBuffer* pCB, const char varName[]) const;
        
        /** Get the ParameterBlock object for the material. Each material is created with a parameter-block. Using it is more efficient than assigning data to a custom constant-buffer.
        */
        ParameterBlock::SharedConstPtr getParameterBlock() const;
    private:
        void updateDiffuseType();
        void updateSpecularType();
        void updateEmissiveType();
        void setIntoParameterBlock(ParameterBlock* pBlock) const;
        
        Material(const std::string& name);
        std::string mName;
        MaterialData mData;
        mutable bool mParamBlockDirty = true;
        ParameterBlock::SharedPtr mpParameterBlock;
        static uint32_t sMaterialCounter;
        static ParameterBlockReflection::SharedConstPtr spBlockReflection;
    };

#undef Texture2D
}
