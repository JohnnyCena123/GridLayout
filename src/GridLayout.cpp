#include "GridLayout.hpp"
#include "Geode/utils/cocos.hpp"

using namespace geode::prelude;

class GridLayout::Impl {
public:
	ExpandDirection m_expandDirection;
	float m_gapX;
	float m_gapY;
	int m_maxX;
	int m_maxY;
	
};

void GridLayout::apply(CCNode* on) {

}

CCSize GridLayout::getSizeHint(CCNode* on) const {
	CCSize size;

	return size;
}

ExpandDirection GridLayout::getExpandDirection() const {
	return m_impl->m_expandDirection;
}
float GridLayout::getGapX() const {
	return m_impl->m_gapX;
}
float GridLayout::getGapY() const {
	return m_impl->m_gapY;
}
int GridLayout::getMaxX() const {
	return m_impl->m_maxX;
}
int GridLayout::getMaxY() const {
	return m_impl->m_gapY;
}

GridLayout* GridLayout::setExpandDirection(ExpandDirection direction) {
	m_impl->m_expandDirection = direction;
	return this;
}
GridLayout* GridLayout::setGapX(float gapX) {
	m_impl->m_gapX = gapX;
	return this;
}
GridLayout* GridLayout::setGapY(float gapY) {
	m_impl->m_maxY = gapY;
	return this;
}
GridLayout* GridLayout::setMaxX(float maxX) {
	m_impl->m_maxX = maxX;
	return this;
}
GridLayout* GridLayout::setMaxY(float maxY) {
	m_impl->m_maxY = maxY;
	return this;
}

GridLayout::GridLayout() : m_impl(std::make_unique<Impl>()) {}
GridLayout::~GridLayout() {}

GridLayout* GridLayout::create() {
	auto ret = new GridLayout();
	ret->autorelease();
	return ret;
}