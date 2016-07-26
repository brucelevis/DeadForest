//
//  TriggerParameterLocation.hpp
//  DeadCreator
//
//  Created by mac on 2016. 7. 19..
//
//

#pragma once

#include "TriggerParameterBase.hpp"
#include "LocationNode.hpp"
#include "GMXLayer.hpp"

namespace realtrick
{
    
    class TriggerParameterLocation : public TriggerParameterBase
    {
        
    public:
        
        TriggerParameterLocation() : TriggerParameterBase(),
        _location(nullptr)
        {
            _parameterType = TriggerParameterBase::Type::ENTITY;
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
            _location = location;
            setParameterName(location->getLocationName());
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
            std::vector<std::string> locationNames;
            for(auto& loc : gmxLayer->getLocations() )
            {
                locationList += loc->getLocationName();
                locationList += '\0';
            }
            
            if ( ImGui::Combo("##", &_currLocation, locationList.c_str(), 5) )
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









