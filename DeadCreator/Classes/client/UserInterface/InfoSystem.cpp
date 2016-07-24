//
//  InfoSystem.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 2. 29..
//
//

#include <iterator>

#include "InfoSystem.hpp"

namespace realtrick
{
    namespace userinterface
    {
        
        using namespace cocos2d;
        
        InfoSystem::InfoSystem():
        _disableTime(0.25f),
        _fontFilePath(""),
        _fontSize(0),
        _fontColor(Color3B::WHITE),
        _maxLine(0),
        _fontOffset(5.0f),
        _lastTime(std::chrono::duration<double>::zero()),
        _deathTime(5.0f)
        {
        }
        
        
        InfoSystem::~InfoSystem()
        {
        }
        
        
        InfoSystem* InfoSystem::create(const std::string& fontPath, float fontSize, const cocos2d::Color3B& color, int viewLine)
        {
            InfoSystem* ret = new (std::nothrow)InfoSystem();
            if ( ret && ret->init(fontPath, fontSize, color, viewLine) )
            {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        
        
        bool InfoSystem::init(const std::string& fontPath, float fontSize, const cocos2d::Color3B& color, int viewLine)
        {
            if ( !Node::init() || viewLine <= 0 )
                return false;
            
            _fontFilePath = fontPath;
            _fontSize = fontSize;
            _fontColor = color;
            _maxLine = viewLine;
            
            scheduleUpdate();
            
            return true;
        }
        
        
        void InfoSystem::pushMessage(const std::string& msg)
        {
            std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
            if ( curr  > _lastTime )
            {
                _discharge(msg);
                _lastTime = curr;
            }
            else
            {
                InfoWithTime info;
                info.dispatchTime = _lastTime;
                info.msg = msg;
                _pq.push(info);
            }
            
            _lastTime += std::chrono::duration<double>(_disableTime * 1.2f);
        }
        
        
        void InfoSystem::_discharge(const std::string& msg)
        {
            ui::Text* newLine = ui::Text::create(msg, _fontFilePath, _fontSize);
            newLine->setColor(_fontColor);
            newLine->setOpacity(0);
            newLine->runAction(Sequence::create(FadeTo::create(_disableTime, 255),
                                                DelayTime::create(_deathTime),
                                                FadeTo::create(_disableTime, 0),
                                                RemoveSelf::create(),
                                                CallFunc::create([&]{ _lines.erase(_lines.begin()); }),
                                                nullptr));
            addChild(newLine);
            _lines.push_back(newLine);
            
            auto iter = std::prev(_lines.end());
            for( auto i = 0 ; i < std::min((int)_lines.size(), _maxLine) ; ++i )
            {
                (*iter)->runAction(MoveTo::create(_disableTime, Vec2(0.0f, (i + 1) * (_fontOffset + _fontSize) )));
                --iter;
            }
            iter = _lines.begin();
            for(int i = 0 ; i < (int)_lines.size() - _maxLine ; ++ i)
            {
                (*iter)->runAction(Sequence::create(Spawn::create(MoveTo::create(_disableTime, Vec2(0.0f, (_maxLine + 1) * (_fontOffset + _fontSize) )),
                                                                  FadeTo::create(_disableTime, 0),
                                                                  nullptr),
                                                    RemoveSelf::create(),
                                                    CallFunc::create([&]{ iter = _lines.erase(_lines.begin()); }),
                                                    nullptr));
            }
        }
        
        
        void InfoSystem::update(float dt)
        {
            std::chrono::duration<double> curr = std::chrono::system_clock::now().time_since_epoch();
            while ( !_pq.empty() &&
                   (_pq.top().dispatchTime < curr) &&
                   (_pq.top().dispatchTime >= std::chrono::duration<double>::zero()) )
            {
                const InfoWithTime& info = _pq.top();
                _discharge(info.msg);
                _pq.pop();
            }
        }
        
    }
}









