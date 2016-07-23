//
//  ItemSlot.cpp
//  TheDeadForest-v2
//
//  Created by mac on 2016. 2. 20..
//
//

#include "ItemSlot.hpp"
#include "ItemBase.hpp"
#include "AnimatedButton.hpp"
#include "ItemExtensionButtons.hpp"
#include "GameManager.hpp"

namespace realtrick
{
    namespace userinterface
    {
        
        using namespace cocos2d;
        
        ItemSlot::ItemSlot(GameManager* mgr) :
        _gameMgr(mgr),
        _item(nullptr),
        _itemImage(nullptr),
        _equippedImage(nullptr),
        _slotSize(Size::ZERO),
        _isEquippedItem(false),
        _numOfItems(nullptr)
        {}
        
        
        ItemSlot::~ItemSlot()
        {
        }
        
        
        ItemSlot* ItemSlot::create(GameManager* mgr, Inventory* owner, const char* normal, const char* selected, cocos2d::ui::Widget::TextureResType texType)
        {
            ItemSlot* ret = new (std::nothrow) ItemSlot(mgr);
            if ( ret && ret->init(owner, normal, selected, texType) )
            {
                ret->autorelease();
                return ret;
            }
            CC_SAFE_DELETE(ret);
            return nullptr;
        }
        
        
        bool ItemSlot::init(Inventory* owner, const char* normal, const char* selected, cocos2d::ui::Widget::TextureResType texType)
        {
            if ( !Widget::init() )
            {
                return false;
            }
            
            _owner = owner;
            
            setSelected(false);
            loadTextures(normal, "", selected, "", "", texType);
            setZoomScale(0.0f);
            
            _itemImage = Sprite::create();
            _itemImage->setPosition(Vec2(getContentSize() / 2.0f));
            addChild(_itemImage, 1);
            
            _equippedImage = Sprite::create("active_slot.png");
            _equippedImage->setPosition(Vec2(getContentSize() / 2.0f));
            _equippedImage->setVisible(false);
            addChild(_equippedImage, 2);
            
            _extensionButtons = ItemExtensionButtons::create(_gameMgr, this);
            _extensionButtons->setPosition(Vec2(getContentSize() / 2.0f));
            addChild(_extensionButtons, 3);
            
            _numOfItems = ui::Text::create("", "SpecialElite.TTF", 32);
            _numOfItems->setVisible(false);
            _numOfItems->setPosition(Vec2(_owner->getSlotSize().width * 1.2f, 18.0f));
            addChild(_numOfItems);
            
            return true;
        }
        
        
        void ItemSlot::setSlotSize(const cocos2d::Size& size)
        {
            float sx, sy;
            sx = size.width / getContentSize().width;
            sy = size.height / getContentSize().height;
            setScale(sx, sy);
            
            _slotSize = Size(getContentSize().width * sx, getContentSize().height * sy);
        }
        
        void ItemSlot::disableSelected()
        {
            // disable event
            _extensionButtons->hideButtons();
            setSelected(false);
        }
        
        
        void ItemSlot::enableSelected()
        {
            // enable event
            if ( isExistItem() )
            {
                _extensionButtons->showButtons();
                setSelected(true);
            }
        }
        
        
        void ItemSlot::setItem(ItemBase* item)
        {
            _item = item->clone();
            
            std::string frameName = _item->getInSlotFrameName();
            if ( item->getTexType() == ui::Widget::TextureResType::LOCAL )
            {
                _itemImage->setTexture(frameName);
            }
            else
            {
                _itemImage->setSpriteFrame(frameName);
            }
            _itemImage->setScale(_slotSize.width / _item->getInSlotSpriteSize().width, _slotSize.height / _item->getInSlotSpriteSize().height);
            _itemImage->setVisible(true);
        }
        
        
        void ItemSlot::throwItem()
        {
            _item->discard();
            CC_SAFE_DELETE(_item);
            _itemImage->setVisible(false);
            _numOfItems->setVisible(false);
        }
        
        
        void ItemSlot::useItem()
        {
            _item->use();
            disableSelected();
        }
        
        
        void ItemSlot::releaseWeapon()
        {
            _item->releaseWeapon();
            disableSelected();
        }
        
        
        void ItemSlot::enableEquippedItem()
        {
            // 장착중 이미지를 보여준다.
            _isEquippedItem = true;
            _equippedImage->setVisible(true);
            _owner->disableOtherEquipedItem(getTag());
        }
        
        
        void ItemSlot::disableEquippedItem()
        {
            _isEquippedItem = false;
            _equippedImage->setVisible(false);
        }
        
        
        void ItemSlot::setItemAmount(int amount)
        {
            // 존재하고, 무기종류가 아닌 아이템에 대해서만 아이템 개수를 설정할 수 있다.
            if ( isExistItem() && !isMasked(_item->getFamilyMask(), FamilyMask::WEAPON_BASE))
            {
                if ( amount <= 0)
                {
                    // 0이하면 소모해버린다.(없앤다)
                    consumeItem();
                }
                else
                {
                    _item->setAmount(amount);
                    _numOfItems->setString(GameManager::_to_string(amount));
                    _numOfItems->setVisible(true);
                }
            }
            else
            {
                log("<ItemSlot::setItemAmount> not exist item [OR] item is weapon");
            }
        }
        
        void ItemSlot::consumeItem()
        {
            // 무기종류는 소모할 수없다.
            if ( isMasked(_item->getFamilyMask(), FamilyMask::WEAPON_BASE ) )
            {
                log("<ItemSlot::consumeItem> Can not consume kind of weapon.");
                return ;
            }
            
            CC_SAFE_DELETE(_item);
            _itemImage->setVisible(false);
            _numOfItems->setVisible(false);
        }
        
    }
}









