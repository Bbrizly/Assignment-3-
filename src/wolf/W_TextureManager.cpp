//-----------------------------------------------------------------------------
// File:			W_TextureManager.cpp
// Original Author:	Gordon Wood
//
// See header for notes
//-----------------------------------------------------------------------------
#include "W_TextureManager.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace wolf
{

std::map<std::string, TextureManager::Entry*>	TextureManager::m_textures;

//----------------------------------------------------------
// Creates a new texture or returns an existing copy if already
// loaded previously
//----------------------------------------------------------
Texture* TextureManager::CreateTexture(const std::string& p_strFile)
{
	std::map<std::string, Entry*>::iterator iter = m_textures.find(p_strFile);

	if( iter != m_textures.end() )
	{
		iter->second->m_iRefCount++;
		return iter->second->m_pTex;
	}

	Texture* pTex = new Texture(p_strFile);
	Entry* pEntry = new Entry(pTex);
	m_textures[p_strFile] = pEntry;
	return pTex;
}

//----------------------------------------------------------
// When creating directly from data, we don't check for duplicates
// and just delegate directly to the texture class
//----------------------------------------------------------
Texture* TextureManager::CreateTexture(void* p_pData, unsigned int p_uiWidth, unsigned int p_uiHeight, Texture::Format p_eFormat)
{
	return new Texture(p_pData, p_uiWidth, p_uiHeight, p_eFormat);
}

Texture* TextureManager::CreateArrayTexture(void* pData, unsigned int width, unsigned int height, unsigned int layers, Texture::Format fmt) {
    return new Texture(pData, width, height, layers, fmt);
}
Texture* TextureManager::CreateAutoArrayTexture(const std::vector<std::string>& filePaths) {

    if (filePaths.empty()) {
        printf("Error: No file paths provided for array texture creation.\n");
        return nullptr;
    }

    int texWidth, texHeight, texChannels;
    std::vector<unsigned char*> imageDataList;
    texWidth = texHeight = texChannels = 0;

    for (const auto& path : filePaths) {
        unsigned char* data = stbi_load(path.c_str(), &texWidth, &texHeight, &texChannels, 4);
        if (!data) {
            printf("Error: Failed to load texture '%s'\n", path.c_str());
            for (auto& imgData : imageDataList) {
                stbi_image_free(imgData);
            }
            return nullptr;
        }
        imageDataList.push_back(data);
    }

    size_t singleLayerSize = texWidth * texHeight * 4;
    size_t totalSize = singleLayerSize * imageDataList.size();
    unsigned char* arrayData = new unsigned char[totalSize];

    for (size_t i = 0; i < imageDataList.size(); ++i) {
        memcpy(arrayData + i * singleLayerSize, imageDataList[i], singleLayerSize);
    }

    wolf::Texture* arrayTexture = wolf::TextureManager::CreateArrayTexture(
        arrayData, texWidth, texHeight, (unsigned int)imageDataList.size(), wolf::Texture::FMT_8888
    );

    if (!arrayTexture) {
        printf("Error: Failed to create array texture.\n");
        delete[] arrayData;
        for (auto& imgData : imageDataList) {
            stbi_image_free(imgData);
        }
        return nullptr;
    }

    arrayTexture->SetWrapMode(wolf::Texture::WM_Repeat, wolf::Texture::WM_Repeat);
    arrayTexture->SetFilterMode(wolf::Texture::FM_TrilinearMipmap, wolf::Texture::FM_Linear);

    delete[] arrayData;
    for (auto& imgData : imageDataList) {
        stbi_image_free(imgData);
    }
    return arrayTexture;
}


//----------------------------------------------------------
// Destroys a texture. Only actually deletes it if the refcount
// is down to 0.
//----------------------------------------------------------
void TextureManager::DestroyTexture(Texture* p_pTex)
{
	std::map<std::string, Entry*>::iterator iter;
	for( iter = m_textures.begin(); iter != m_textures.end(); iter++ )
	{
		if( iter->second->m_pTex == p_pTex )
		{
			iter->second->m_iRefCount--;
			if( iter->second->m_iRefCount == 0 )
			{
				delete iter->second->m_pTex;
				m_textures.erase(iter);
			}
			return;
		}
	}

	// If we got here, it can only be via a from-data texture which wasn't in
	// the list (unless the pointer being passed in is dodgy in which case we're
	// in trouble anyway).
	delete p_pTex;
}

}


