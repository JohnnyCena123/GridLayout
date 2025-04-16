#pragma once
#include <Geode/ui/Layout.hpp>

namespace geode {

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

		Axis getExpandAxis() const;
		float getGapX() const;
		float getGapY() const;
		int getMaxCountCrossExpandAxis() const;
		// bool doesExpand() const;
		bool isAutoAdjustSize() const;
		bool isReverseExpandAxis() const;
		bool isReverseCrossExpandAxis() const;

		GridLayout* setExpandAxis(Axis axis);
		GridLayout* setGapX(float gapX);
		GridLayout* setGapY(float gapY);
		GridLayout* setMaxCountCrossExpandAxis(int value);
		// GridLayout* expand(bool allow);
		GridLayout* autoAdjustSize(bool allow);
		GridLayout* reverseExpandAxis(bool reverse);
		GridLayout* reverseCrossExpandAxis(bool reverse);
};

}