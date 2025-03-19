#pragma once
#include "ComponentBase.h"
#include "BoundingVolume.h"
#include "GameObject.h"
#include "common/DebugRender.h"
#include "SphereVolume.h"

#include "iostream"
#include "sstream"
#include "tinyxml.h"

namespace Common {

    class BoundingVolumeComponent : public ComponentBase {
    public:
        BoundingVolumeComponent(SphereVolume* volume)
            : m_volume(volume)
        {}

        static Common::ComponentBase* CreateComponent(TiXmlNode* pNode)
        {
            if (std::strcmp(pNode->Value(), "BoundingVolumeComponent") != 0 &&
                std::strcmp(pNode->Value(), "GOC_BoundingVolume") != 0)
            {
                return nullptr;
            }

            // Defaults
            std::string volumeType = "Sphere";
            glm::vec3 center(0, 0, 0);
            float radius = 1.0f;

            TiXmlNode* pChild = pNode->FirstChild();
            while (pChild)
            {
                if (std::strcmp(pChild->Value(), "Param") == 0)
                {
                    TiXmlElement* elem = pChild->ToElement();
                    const char* szName = elem->Attribute("name");
                    const char* szValue = elem->Attribute("value");
                    if (szName && szValue)
                    {
                        if (std::strcmp(szName, "volumeType") == 0)
                        {
                            volumeType = szValue; //"Sphere"
                        }
                        else if (std::strcmp(szName, "center") == 0)
                        {
                            float x, y, z;
                            std::stringstream sstream(szValue);
                            sstream >> x >> y >> z;
                            center = glm::vec3(x, y, z);
                        }
                        else if (std::strcmp(szName, "radius") == 0)
                        {
                            radius = float(atof(szValue));
                        }
                    }
                }
                pChild = pChild->NextSibling();
            }

            if (volumeType == "Sphere")
            {
                std::cout << "\nSPHERE BOUNDING VOLUME ADDED\n\n";
                SphereVolume* sphere = new SphereVolume(center, radius);
                BoundingVolumeComponent* pBVC = new BoundingVolumeComponent(sphere);
                return pBVC;
            }

            //return nullptr;
        }

        virtual ~BoundingVolumeComponent() {
            delete m_volume;
        }

        virtual void Update(float deltaTime) override {

            if (GetGameObject()) {
                glm::mat4 worldTransform = GetGameObject()->GetTransform().GetTransformation();
                m_volume->UpdateVolume(worldTransform);

                //m_volume->DrawMe(); //REMOVE LATER DELETE LATER
            }   
        }

        BoundingVolume* GetVolume() const { return m_volume; }

        virtual const std::string FamilyID() override { return "GOC_BoundingVolume"; }
        virtual const std::string ComponentID() override { return "GOC_BoundingVolume"; }

    private:
        BoundingVolume* m_volume;
    };

} // namespace Common
