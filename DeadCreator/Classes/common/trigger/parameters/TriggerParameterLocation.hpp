//
//  TriggerParameterLocation.hpp
//  DeadCreator
//
//  Created by NamJunHyeon on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "LocationNode.hpp"
#include "GMXLayer.hpp"

namespace realtrick
{
    namespace editor
    {
        
        class TriggerParameterLocation : public TriggerParameterBase
        {
            
        public:
            
            TriggerParameterLocation() : TriggerParameterBase(),
            _location(nullptr)
            {
                setParameterName("#invalid");
            }
            
            TriggerParameterLocation(const TriggerParameterLocation& rhs) { copyFrom(rhs); }
            TriggerParameterLocation& operator=(const TriggerParameterLocation& rhs)
            {
                if ( &rhs != this ) copyFrom(rhs);
                return *this;
            }
            
            void copyFrom(const TriggerParameterLocation& rhs)
            {
                TriggerParameterBase::copyFrom(rhs);
                _location = rhs._location;
                _currLocation = rhs._currLocation;
            }
            
            virtual ~TriggerParameterLocation() { _location = nullptr; }
            
            LocationNode* getLocation() const { return _location; }
            void setLocation(LocationNode* location)
            {
                setParameterName(location->getLocationName());
                
                _location = location;
                
                auto& locations = location->getGMXLayer().getLocations();
                for(int i = 0 ; i < locations.size() ; ++ i )
                {
                    if(  locations[i] == location ) _currLocation = i;
                }
            }
            
            virtual std::string getParameterName() const override
            {
                std::string ret = "#invalid";
                if ( _location ) ret = _location->getLocationName();
                    return ret;
            }
            
            virtual TriggerParameterLocation* clone() const override
            {
                return new TriggerParameterLocation(*this);
            }
            
            virtual void reset() override { _currLocation = -1; }
            virtual bool isItemSelected() override { return (_currLocation != -1); }
            
            virtual void drawImGui(void* opt = nullptr) override
            {
                if ( opt == nullptr ) throw std::runtime_error("location's drawImGUi() must have *opt(gmx layer's instance)");
                    auto gmxLayer = static_cast<GMXLayer*>(opt);
                
                ImGui::PushStyleColor(ImGuiCol_FrameBg, ImVec4(0.85, 0.85, 0.85, 1.0));
                ImGui::PushItemWidth(200);
                std::string locationList;
                for(auto& loc : gmxLayer->getLocations() )
                {
                    locationList += loc->getLocationName();
                    locationList += '\0';
                }
                if ( ImGui::Combo("", &_currLocation, locationList.c_str(), 5) )
                {
                    setLocation(gmxLayer->getLocations().at(_currLocation));
                }
                ImGui::PopItemWidth();
                ImGui::PopStyleColor();
            }
            
        private:
            
            LocationNode* _location;
            int _currLocation = -1;
            
        };
        
    }
}









