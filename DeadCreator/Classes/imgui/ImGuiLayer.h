#ifndef __IMGUILAYER_H__
#define __IMGUILAYER_H__

#include "cocos2d.h"

USING_NS_CC;

class ImGuiLayer : public cocos2d::Layer
{
public:
    
    virtual bool init() override;

    virtual void visit(Renderer *renderer, const Mat4& parentTransform, uint32_t parentFlags) override;

    void onDraw();

    CREATE_FUNC(ImGuiLayer);

private:
    
    CustomCommand _command;
    
};

#endif // __IMGUILAYER_H__
