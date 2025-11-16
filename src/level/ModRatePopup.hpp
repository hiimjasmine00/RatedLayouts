#pragma once

#include <Geode/Geode.hpp>
#include <argon/argon.hpp>

using namespace geode::prelude;

class ModRatePopup : public geode::Popup<std::string>
{
public:
    static ModRatePopup *create(std::string title = "Rate Layout");

private:
    std::string m_title;
    GJDifficultySprite* m_difficultySprite;
    bool m_isDemonMode;
    CCMenu* m_normalButtonsContainer;
    CCMenu* m_demonButtonsContainer;
    int m_selectedRating;
    bool setup(std::string title) override;
    void onSubmitButton(CCObject* sender);
    void onToggleDifficulty(CCObject* sender);
    void onToggleDemon(CCObject* sender);
    void onRatingButton(CCObject* sender);
    void updateDifficultySprite(int rating);
};
