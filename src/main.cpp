#include "Geode/binding/CCMenuItemSpriteExtra.hpp"
#include "Geode/binding/CCMenuItemToggler.hpp"
#include "Geode/binding/FLAlertLayer.hpp"
#include "Geode/cocos/CCScheduler.h"
#include "Geode/cocos/cocoa/CCObject.h"
#include <Geode/Geode.hpp>

#include "Geode/cocos/label_nodes/CCLabelBMFont.h"
#include "Geode/cocos/menu_nodes/CCMenuItem.h"
#include "Geode/cocos/sprite_nodes/CCSprite.h"
#include "Geode/ui/BasedButtonSprite.hpp"
#include "Geode/ui/TextInput.hpp"
#include "GridLayout.hpp"

using namespace geode::prelude;

class GridLayoutSettings : public Popup<GridLayout*, CCNode*> {
protected:
	GridLayout* m_layout;
	CCNode* m_node;

	CCLabelBMFont* m_gapXLabel;
	CCLabelBMFont* m_gapYLabel;
	CCMenuItemToggler* m_expandAxisToggler;
	TextInput* m_maxCountInput;
	CCMenuItemToggler* m_autoAdjustSizeToggler;
	CCMenuItemToggler* m_reverseExpandAxisToggler;
	CCMenuItemToggler* m_reverseCrossExpandAxisToggler;


	bool setup(GridLayout* layout, CCNode* node) override {

		m_layout = layout;
		m_node = node;

		this->setTitle("Grid Layout Settings");


		// expand axis

		auto expandAxisColumnSpr = CCSprite::create("GJ_moveBtn.png");
		expandAxisColumnSpr->setRotation(90.f);
		expandAxisColumnSpr->setFlipY(true);
		auto expandAxisRowSpr = CCSprite::create("GJ_moveBtn.png");
		m_expandAxisToggler = CCMenuItemToggler::create(
			expandAxisRowSpr, expandAxisColumnSpr, this, nullptr
		);
		m_expandAxisToggler->toggle(m_layout->getExpandAxis() == Axis::Column);
		m_buttonMenu->addChildAtPosition(m_expandAxisToggler, Anchor::TopRight, {-30.f, -30.f});


		// gaps

		auto gapX = m_layout->getGapX();
		auto gapXSlider = Slider::create(this, menu_selector(GridLayoutSettings::onChangeGap));
		gapXSlider->setValue(gapX / 100.f);
		gapXSlider->setTag(1);
		m_mainLayer->addChildAtPosition(gapXSlider, Anchor::Bottom, {0.f, 110.f});
		auto gapXLabel = CCLabelBMFont::create("Gap X", "bigFont.fnt");
		gapXLabel->setScale(.5f);
		m_mainLayer->addChildAtPosition(gapXLabel, Anchor::Bottom, {-150.f, 110.f});
		m_gapXLabel = CCLabelBMFont::create(fmt::format("{}", gapX).c_str(), "bigFont.fnt");
		m_gapXLabel->setScale(.75f);
		m_gapXLabel->setAnchorPoint({0.f, .5f});
		m_mainLayer->addChildAtPosition(m_gapXLabel, Anchor::Bottom, {120.f, 110.f});
		

		auto gapY = m_layout->getGapY();
		auto gapYSlider = Slider::create(this, menu_selector(GridLayoutSettings::onChangeGap));
		gapYSlider->setValue(gapY / 100.f);
		gapYSlider->setTag(2);
		m_mainLayer->addChildAtPosition(gapYSlider, Anchor::Bottom, {0.f, 75.f});
		auto gapYLabel = CCLabelBMFont::create("Gap Y", "bigFont.fnt");
		gapYLabel->setScale(.5f);
		m_mainLayer->addChildAtPosition(gapYLabel, Anchor::Bottom, {-150.f, 75.f});
		m_gapYLabel = CCLabelBMFont::create(fmt::format("{}", gapY).c_str(), "bigFont.fnt");
		m_gapYLabel->setScale(.75f);
		m_gapYLabel->setAnchorPoint({0.f, .5f});
		m_mainLayer->addChildAtPosition(m_gapYLabel, Anchor::Bottom, {120.f, 75.f});


		/** 
		 *  max count
		 */

		// input
		m_maxCountInput = TextInput::create(50.f, "Max ");
		m_maxCountInput->setString(fmt::format("{}", m_layout->getMaxCountCrossExpandAxis()));
		m_maxCountInput->setFilter("0123456789");
		m_maxCountInput->setLabel("Max count");
		// container
		auto maxCountContainer = CCNode::create();
		maxCountContainer->setContentSize(m_maxCountInput->getContentSize());
		maxCountContainer->setScale(2.5f);
		maxCountContainer->setAnchorPoint({.5f, .5f});
		maxCountContainer->addChildAtPosition(m_maxCountInput, Anchor::Center);
		m_mainLayer->addChildAtPosition(maxCountContainer, Anchor::Right, {-130.f, 40.f});
		// menu
		auto maxCountArrowsMenu = CCMenu::create();
		maxCountArrowsMenu->setContentSize(m_maxCountInput->getContentSize());
		maxCountContainer->addChildAtPosition(maxCountArrowsMenu, Anchor::Center);
		// arrow 1
		auto maxCountArrow1Spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
		maxCountArrow1Spr->setScale(.5f);
		auto maxCountArrow1 = CCMenuItemSpriteExtra::create(
			maxCountArrow1Spr, this, menu_selector(GridLayoutSettings::onMaxCountArrow)
		);
		maxCountArrow1->setAnchorPoint({1.f, .5f});
		maxCountArrow1->setTag(-1);
		maxCountArrowsMenu->addChildAtPosition(maxCountArrow1, Anchor::Left, {-5.f, 0.f});
		// arrow 2
		auto maxCountArrow2Spr = CCSprite::createWithSpriteFrameName("GJ_arrow_02_001.png");
		maxCountArrow2Spr->setScale(.5f);
		maxCountArrow2Spr->setFlipX(true);
		auto maxCountArrow2 = CCMenuItemSpriteExtra::create(
			maxCountArrow2Spr, this, menu_selector(GridLayoutSettings::onMaxCountArrow)
		);
		maxCountArrow2->setAnchorPoint({0.f, .5f});
		maxCountArrow2->setTag(1);
		maxCountArrowsMenu->addChildAtPosition(maxCountArrow2, Anchor::Right, {5.f, 0.f});
		// info
		auto maxCountInfoSpr = CCSprite::createWithSpriteFrameName("GJ_infoIcon_001.png");
		maxCountInfoSpr->setScale(.375f);
		auto maxCountInfoBtn = CCMenuItemSpriteExtra::create(
			maxCountInfoSpr, this, menu_selector(GridLayoutSettings::onMaxCountInfo)
		);
		maxCountInfoBtn->setAnchorPoint({1.f, 0.f});
		maxCountArrowsMenu->addChildAtPosition(maxCountInfoBtn, Anchor::TopLeft, {-1.f, 1.f});
		

		// auto adjust size

		m_autoAdjustSizeToggler = CCMenuItemToggler::createWithStandardSprites(
			this, nullptr, 1.f
		);
		m_autoAdjustSizeToggler->toggle(m_layout->isAutoAdjustSize());
		m_buttonMenu->addChildAtPosition(m_autoAdjustSizeToggler, Anchor::TopLeft, {40.f, -60.f});
		auto autoAdjustSizeLabel = CCLabelBMFont::create("auto adjust size", "bigFont.fnt");
		autoAdjustSizeLabel->limitLabelWidth(170.f, 1.f, 0.f);
		m_mainLayer->addChildAtPosition(autoAdjustSizeLabel, Anchor::TopLeft, {150.f, -60.f});


		// reverse axes

		m_reverseExpandAxisToggler = CCMenuItemToggler::createWithStandardSprites(
			this, nullptr, 1.f
		);
		m_reverseExpandAxisToggler->toggle(m_layout->isReverseExpandAxis());
		m_buttonMenu->addChildAtPosition(m_reverseExpandAxisToggler, Anchor::TopLeft, {40.f, -100.f});
		auto reverseExpandAxisLabel = CCLabelBMFont::create("reverse expand axis", "bigFont.fnt");
		reverseExpandAxisLabel->limitLabelWidth(170.f, 1.f, 0.f);
		m_mainLayer->addChildAtPosition(reverseExpandAxisLabel, Anchor::TopLeft, {150.f, -100.f});

		m_reverseCrossExpandAxisToggler = CCMenuItemToggler::createWithStandardSprites(
			this, nullptr, 1.f
		);
		m_reverseCrossExpandAxisToggler->toggle(m_layout->isReverseCrossExpandAxis());
		m_buttonMenu->addChildAtPosition(m_reverseCrossExpandAxisToggler, Anchor::TopLeft, {40.f, -140.f});
		auto reverseCrossExpandAxisLabel = CCLabelBMFont::create("reverse cross expand axis", "bigFont.fnt");
		reverseCrossExpandAxisLabel->limitLabelWidth(170.f, 1.f, 0.f);
		m_mainLayer->addChildAtPosition(reverseCrossExpandAxisLabel, Anchor::TopLeft, {150.f, -140.f});


		// apply

		auto applyBtn = CCMenuItemSpriteExtra::create(
			ButtonSprite::create("Apply"),
			this, menu_selector(GridLayoutSettings::onApply)	
		);
		m_buttonMenu->addChildAtPosition(applyBtn, Anchor::Bottom, {0.f, 30.f});

		return true;
	}
	

	void onChangeGap(CCObject* sender) {
		auto slider = static_cast<Slider*>(static_cast<CCNode*>(sender)->getParent()->getParent());
		if (slider->getTag() == 1) m_gapXLabel->setString(fmt::format("{}", slider->getValue() * 100.f).c_str());
		else m_gapYLabel->setString(fmt::format("{}", slider->getValue() * 100.f).c_str());
	}

	void onMaxCountArrow(CCObject* sender) {
		m_maxCountInput->setString(
			fmt::format(
				"{}", 
				stoi(m_maxCountInput->getString()) + sender->getTag()
			)
		);
	}

	void onMaxCountInfo(CCObject*) {
		FLAlertLayer::create(
			nullptr, "Max Count Help", 
			"The 'max count' setting lets you specify how many\n"
			"different nodes should fit into one line accross the expand axis\n"
			"in the layout.\n"
			"For example, if the expand axis is Axis::Column, and you set the\n"
			"max count setting to 3, in every horizontal line there will be up to 3 nodes.\n"
			"P.S. I didn't have enough space to make it clear enough without an info button ._.", 
			"OK", "OK", 500.f
		)->show();
	}

	void onApply(CCObject*) {
		(void) m_layout
		->setExpandAxis(m_expandAxisToggler->isToggled() ? Axis::Column : Axis::Row)
		->setGapX(std::stoi(m_gapXLabel->getString()))
		->setGapY(std::stoi(m_gapYLabel->getString()))
		->setMaxCountCrossExpandAxis(stoi(m_maxCountInput->getString()))
		->autoAdjustSize(m_autoAdjustSizeToggler->isToggled())
		->reverseExpandAxis(m_reverseExpandAxisToggler->isToggled())
		->reverseCrossExpandAxis(m_reverseCrossExpandAxisToggler->isToggled());

		m_node->updateLayout();
	}

public:
	static GridLayoutSettings* create(GridLayout* layout, CCNode* node) {
		auto ret = new GridLayoutSettings();
		if (ret->initAnchored(500.f, 300.f, layout, node)) {
			ret->autorelease();
			return ret;
		}
		delete ret;
		return nullptr;
	}
};

class GridLayoutLayer : public CCLayer {

	ScrollLayer* m_scrollLayer;

public:
	bool init() override {
		if (!CCLayer::init()) return false;

		auto bg = CCSprite::create("GJ_gradientBG.png");
	
		bg->setAnchorPoint({0.f, 0.f});
		bg->setPosition({-5.f, -5.f});
	
		auto winSize = CCDirector::sharedDirector()->getWinSize();
		auto bgSize = bg->getContentSize();
		bg->setScaleX((winSize.width + 10.f) / bgSize.width);
		bg->setScaleY((winSize.height + 10.f) / bgSize.height);
		bg->setColor({0, 102, 255});
		this->addChild(bg);

		auto layer = CCLayer::create();
		layer->setAnchorPoint({0.f, 0.f});
		this->addChild(layer);

		m_scrollLayer = ScrollLayer::create({winSize.width - 150.f, winSize.height - 50.f});
		layer->addChildAtPosition(m_scrollLayer, Anchor::Center);
		m_scrollLayer->m_contentLayer->setLayout(
			GridLayout::create()
			->setMaxCountCrossExpandAxis(3)
			->setExpandAxis(Axis::Column)
			->setGapX(13.f)
			->setGapY(26.f)
			->reverseCrossExpandAxis(true)
			->reverseExpandAxis(true)
		);
		m_scrollLayer->ignoreAnchorPointForPosition(false);

		auto scrollbar = Scrollbar::create(m_scrollLayer);
		layer->addChildAtPosition(scrollbar, Anchor::Right, {-60.f, 0.f});


		for (int i = 0; i < 5; i++) {
			addNode();
		}
		onAddNode(nullptr);


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
	
		auto settingsBtn = CCMenuItemSpriteExtra::create(
			CCSprite::createWithSpriteFrameName("GJ_optionsBtn_001.png"),
			this, menu_selector(GridLayoutLayer::onSettings)
		);
		settingsBtn->setAnchorPoint({0.f, 1.f});
		btnMenu->addChildAtPosition(settingsBtn, Anchor::TopLeft, {10.f, -180.f});

		return true;
	}

	void onSettings(CCObject*) {
		GridLayoutSettings::create(static_cast<GridLayout*>(m_scrollLayer->m_contentLayer->getLayout()), m_scrollLayer->m_contentLayer)->show();
	}

	void addNode() {
		static int i = 0;
		m_scrollLayer->m_contentLayer->addChild(CCSprite::create(fmt::format("dialogIcon_0{:02}.png", (i % 56) + 1).c_str()));
		// m_scrollLayer->m_contentLayer->addChild(CCSprite::create(fmt::format("GJ_button_0{}.png", (i % 6) + 1).c_str()));
		i++;
	}

	void onAddNode(CCObject*) {
		addNode();
		m_scrollLayer->m_contentLayer->updateLayout();
	}

	void onAdd5Nodes(CCObject*) {
		CCScheduler::get()->scheduleSelector(schedule_selector(GridLayoutLayer::add5Nodes), this, .25f, 5, 0.f, false);
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
			CircleButtonSprite::create(
				CCLabelBMFont::create("Grid\nLayout", "bigFont.fnt"), 
				CircleBaseColor::Green, CircleBaseSize::MediumAlt
			), 
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