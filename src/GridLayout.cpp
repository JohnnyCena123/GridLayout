#include "GridLayout.hpp"
#include "Geode/utils/cocos.hpp"

using namespace geode::prelude;

class GridLayout::Impl {
public:
	Axis m_mainAxis = Axis::Column;
	float m_gapX;
	float m_gapY;
	int m_maxCountCrossMainAxis = 1;
	bool m_autoExpand = true;
	bool m_reverseMainAxis;
	bool m_reverseCrossMainAxis;
};

void GridLayout::apply(CCNode* on) {

}

CCSize GridLayout::getSizeHint(CCNode* on) const {
	auto nodes = getNodesToPosition(on);

	if (nodes->count() == 0 || !m_impl->m_autoExpand) {
		return on->getContentSize();
	} 

	std::vector<std::vector<CCNode*>> grid;
	int lineOnMainAxis = 0; // row if expands on the x, column if expands on the y
	int lineCrossMainAxis = -1; // column if expands on the x, row if expands on the y
	for (auto node : CCArrayExt<CCNode*>(nodes)) {
		if (lineOnMainAxis % m_impl->m_maxCountCrossMainAxis == 0) { // need to add a new line
			lineCrossMainAxis++;
			grid.push_back(std::vector<CCNode*>());
		}
		grid[lineCrossMainAxis].push_back(node);
		lineOnMainAxis++;
	}

	bool vertical = m_impl->m_mainAxis == Axis::Column;

	float totalLengthCrossMainAxis = -(vertical ? m_impl->m_gapX : m_impl->m_gapY); // start with minus the gap once
	for (auto lineCrossMainAxis : grid) {
		float maxHeightForLine = 0.f;
		for (auto node : lineCrossMainAxis) {
			auto length = vertical ? node->getContentWidth() : node->getContentHeight();
			if (length > maxHeightForLine) maxHeightForLine = length;
		}
		totalLengthCrossMainAxis += vertical ? m_impl->m_gapX : m_impl->m_gapY;
		totalLengthCrossMainAxis += maxHeightForLine;
	}
	
	float totalLengthOnMainAxis = -(vertical ? m_impl->m_gapY : m_impl->m_gapX); // start with minus the gap once
	for (auto lineOnMainAxis : grid) {
		float maxHeightForLine = 0.f;
		for (auto node : lineOnMainAxis) {
			auto length = vertical ? node->getContentHeight() : node->getContentWidth();
			if (length > maxHeightForLine) maxHeightForLine = length;
		}
		totalLengthOnMainAxis += vertical ? m_impl->m_gapY : m_impl->m_gapX;
		totalLengthOnMainAxis += maxHeightForLine;
	}

	return vertical ? 
	CCSize(totalLengthCrossMainAxis, totalLengthOnMainAxis) : 
	CCSize(totalLengthOnMainAxis, totalLengthCrossMainAxis);
}

Axis GridLayout::getMainAxis() const {
	return m_impl->m_mainAxis;
}
bool GridLayout::doesExpand() const {
	return m_impl->m_autoExpand;
}
float GridLayout::getGapX() const {
	return m_impl->m_gapX;
}
float GridLayout::getGapY() const {
	return m_impl->m_gapY;
}
int GridLayout::getMaxCountCrossMainAxis() const {
	return m_impl->m_maxCountCrossMainAxis;
}
bool GridLayout::isReverseMainAxis() const {
	return m_impl->m_reverseMainAxis;
}
bool GridLayout::isReverseCrossMainAxis() const {
	return m_impl->m_reverseCrossMainAxis;
}

GridLayout* GridLayout::setMainAxis(Axis axis) {
	m_impl->m_mainAxis = axis;
	return this;
}
GridLayout* GridLayout::setGapX(float gapX) {
	m_impl->m_gapX = gapX;
	return this;
}
GridLayout* GridLayout::setGapY(float gapY) {
	m_impl->m_gapY = gapY;
	return this;
}
GridLayout* GridLayout::setMaxCountCrossMainAxis(int value) {
	m_impl->m_maxCountCrossMainAxis = value;
	return this;
}
GridLayout* GridLayout::reverseMainAxis(bool reverse) {
	m_impl->m_reverseMainAxis = reverse;
	return this;
}
GridLayout* GridLayout::reverseCrossMainAxis(bool reverse) {
	m_impl->m_reverseCrossMainAxis = reverse;
	return this;
}

GridLayout::GridLayout() : m_impl(std::make_unique<Impl>()) {}
GridLayout::~GridLayout() {}

GridLayout* GridLayout::create() {
	auto ret = new GridLayout();
	ret->autorelease();
	return ret;
}