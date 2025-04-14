#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GridLayoutLayer : public CCLayer {

    bool init() override {
        if (!CCLayer::init()) return false;

        auto ws = CCDirector::get()->getWinSize();

        auto sl = ScrollLayer::create({ws.width, ws.height});
        return true;
    }

    static create() {
        auto ret = new GridLayoutLayer();
        if (ret->init()) {
            ret->autorelease();
            return ret;
        }
        delete ret;
        return nullptr
    }
};