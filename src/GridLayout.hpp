#pragma once
#include <Geode/ui/Layout.hpp>

namespace geode {

	enum class ExpandDirection {
		// TopLeft,
		Top,
		// TopRight,
		Right,
		// BottomRight,
		Bottom,
		// BottomLeft,
		Left,
	};

	class GridLayout : public Layout {
	protected:
		class Impl;

		std::unique_ptr<Impl> m_impl;

		GridLayout();

	public:
		static GridLayout* create();
	
		virtual ~GridLayout();

		void apply(cocos2d::CCNode* on) override;
		cocos2d::CCSize getSizeHint(cocos2d::CCNode* on) const override;

		ExpandDirection getExpandDirection() const;
		float getGapX() const;
		float getGapY() const;
		int getMaxX() const;
		int getMaxY() const;

		GridLayout* setExpandDirection(ExpandDirection direction);
		GridLayout* setGapX(float gapX);
		GridLayout* setGapY(float gapY);
		GridLayout* setMaxX(float maxX);
		GridLayout* setMaxY(float maxY);
};

}