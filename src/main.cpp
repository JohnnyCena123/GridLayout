#include "Geode/cocos/CCScheduler.h"
#include "Geode/cocos/cocoa/CCObject.h"
#include <Geode/Geode.hpp>

using namespace geode::prelude;

class GridLayoutLayer : public CCLayer {

	ScrollLayer* m_scrl;

	int i;

public:
	bool init() override {
		if (!CCLayer::init()) return false;

		auto bg = CCSprite::create("GJ_gradientBG.png");
	
		bg->setAnchorPoint({0.f, 0.f});
		bg->setPosition({-5.f, -5.f});
	
		auto ws = CCDirector::sharedDirector()->getWinSize();
		auto bgSize = bg->getContentSize();
		bg->setScaleX((ws.width + 10.f) / bgSize.width);
		bg->setScaleY((ws.height + 10.f) / bgSize.height);
		bg->setColor({0, 102, 255});
		this->addChild(bg);

		auto layer = CCLayer::create();
		layer->setAnchorPoint({0.f, 0.f});
		this->addChild(layer);

		m_scrl = ScrollLayer::create({ws.width - 150.f, ws.height - 50.f});
		layer->addChildAtPosition(m_scrl, Anchor::Center);
		m_scrl->m_contentLayer->setLayout(
			geode::RowLayout::create()
			->setAxisAlignment(AxisAlignment::Start)
			->setCrossAxisAlignment(AxisAlignment::End)
			->setGap(5.f)
			->setGrowCrossAxis(true)
		);
		// m_scrl->setAnchorPoint({.5f, .5f});
		m_scrl->ignoreAnchorPointForPosition(false);

		auto scrb = Scrollbar::create(m_scrl);
		layer->addChildAtPosition(scrb, Anchor::Right, {-60.f, 0.f});


		auto btnMenu = CCMenu::create();
		layer->addChild(btnMenu);
	
		auto backBtn = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png"),
			this, menu_selector(GridLayoutLayer::onBack)
		);
		backBtn->setAnchorPoint({0.f, 1.f});
		btnMenu->addChildAtPosition(backBtn, Anchor::TopLeft, {10.f, -10.f});
	
		auto addNodeBtn = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"),
			this, menu_selector(GridLayoutLayer::onAddNode)
		);
		addNodeBtn->setAnchorPoint({0.f, 1.f});
		btnMenu->addChildAtPosition(addNodeBtn, Anchor::TopLeft, {10.f, -60.f});
	
		auto add5NodesBtn = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_plusBtn_001.png"),
			this, menu_selector(GridLayoutLayer::onAdd5Nodes)
		);
		add5NodesBtn->setAnchorPoint({0.f, 1.f});
		btnMenu->addChildAtPosition(add5NodesBtn, Anchor::TopLeft, {10.f, -120.f});


		return true;
	}

	void onAddNode(CCObject*) {
		m_scrl->m_contentLayer->addChild(CCSprite::create(fmt::format("GJ_button_0{}.png", (i % 6) + 1).c_str()));
		m_scrl->m_contentLayer->updateLayout();
		i++;

		if (m_scrl->m_contentLayer->getContentHeight() < m_scrl->getContentHeight()) {
			auto diff = m_scrl->getContentHeight() - m_scrl->m_contentLayer->getContentHeight();
			m_scrl->m_contentLayer->setContentHeight(m_scrl->getContentHeight());
			for (auto child : CCArrayExt<CCNode*>(m_scrl->m_contentLayer->getChildren()))
				child->setPositionY(child->getPositionY() + diff);
		}
	}

	void onAdd5Nodes(CCObject*) {
		CCScheduler::get()->scheduleSelector(schedule_selector(GridLayoutLayer::add5Nodes), this, 2.f, 5, 5.f, false);
	}

	void add5Nodes(float) {
		onAddNode(nullptr);
	}

	void keyBackClicked() override {
		onBack(nullptr);
	}
	
	void onBack(CCObject*) {
		CCDirector::get()->replaceScene(CCTransitionFade::create(0.5, MenuLayer::scene(false)));
	}


	static GridLayoutLayer* create() {
		auto ret = new GridLayoutLayer();
		if (ret->init()) {
			ret->autorelease();
			return ret;
		}
		delete ret;
		return nullptr;
	}
};

#include <Geode/modify/MenuLayer.hpp>
class $modify(MyMenuLayer, MenuLayer) {
	bool init() {
		if (!MenuLayer::init()) return false;

		auto btn = CCMenuItemSpriteExtra::create(
			CCSprite::create("dialogIcon_002.png"), 
			this, menu_selector(MyMenuLayer::onGridLayout)
		);
		auto menu = this->getChildByID("bottom-menu");
		menu->addChild(btn);
		menu->updateLayout();

		return true;
	}
	
	void onGridLayout(CCObject*) {
		switchToScene(GridLayoutLayer::create());
	}
};