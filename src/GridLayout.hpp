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

		Axis getMainAxis() const;
		bool doesExpand() const;
		float getGapX() const;
		float getGapY() const;
		int getMaxCountCrossMainAxis() const;
		bool isReverseMainAxis() const;
		bool isReverseCrossMainAxis() const;

		GridLayout* setMainAxis(Axis direction);
		GridLayout* expand(bool allow);
		GridLayout* setGapX(float gapX);
		GridLayout* setGapY(float gapY);
		GridLayout* setMaxCountCrossMainAxis(int value);
		GridLayout* reverseMainAxis(bool reverse);
		GridLayout* reverseCrossMainAxis(bool reverse);
};

}