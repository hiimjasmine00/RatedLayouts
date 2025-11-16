
#include "ModRatePopup.hpp"

bool ModRatePopup::setup(std::string title)
{
    m_title = title;
    m_difficultySprite = nullptr;
    m_isDemonMode = false;
    m_selectedRating = -1;

    // title
    auto titleLabel = CCLabelBMFont::create(m_title.c_str(), "bigFont.fnt");
    titleLabel->setPosition({m_mainLayer->getContentSize().width / 2, m_mainLayer->getContentSize().height - 20});
    m_mainLayer->addChild(titleLabel);

    // menu time
    auto menuButtons = CCMenu::create();
    menuButtons->setPosition({0, 0});

    // normal and demon buttons
    m_normalButtonsContainer = CCMenu::create();
    m_normalButtonsContainer->setPosition({0, 0});
    m_demonButtonsContainer = CCMenu::create();
    m_demonButtonsContainer->setPosition({0, 0});

    // first row: ratings 1-5
    float startX = 50.f;
    float buttonSpacing = 55.f;
    float firstRowY = 110.f;

    for (int i = 1; i <= 5; i++)
    {
        auto buttonBg = CCSprite::create("GJ_button_04.png");
        auto buttonLabel = CCLabelBMFont::create(std::to_string(i).c_str(), "bigFont.fnt");
        buttonLabel->setScale(0.75f);
        buttonLabel->setPosition(buttonBg->getContentSize() / 2);
        buttonBg->addChild(buttonLabel);
        buttonBg->setID("button-bg-" + std::to_string(i));

        auto ratingButtonItem = CCMenuItemSpriteExtra::create(
            buttonBg,
            this,
            menu_selector(ModRatePopup::onRatingButton));

        ratingButtonItem->setPosition({startX + (i - 1) * buttonSpacing, firstRowY});
        ratingButtonItem->setTag(i);
        ratingButtonItem->setID("rating-button-" + std::to_string(i));
        m_normalButtonsContainer->addChild(ratingButtonItem);
    }

    // second row: ratings 6-9
    float secondRowY = 60.f;

    for (int i = 6; i <= 9; i++)
    {
        auto buttonBg = CCSprite::create("GJ_button_04.png");
        auto buttonLabel = CCLabelBMFont::create(std::to_string(i).c_str(), "bigFont.fnt");
        buttonLabel->setScale(0.75f);
        buttonLabel->setPosition(buttonBg->getContentSize() / 2);
        buttonBg->addChild(buttonLabel);
        buttonBg->setID("button-bg-" + std::to_string(i));

        auto ratingButtonItem = CCMenuItemSpriteExtra::create(
            buttonBg,
            this,
            menu_selector(ModRatePopup::onRatingButton));

        ratingButtonItem->setPosition({startX + (i - 6) * buttonSpacing, secondRowY});
        ratingButtonItem->setTag(i);
        ratingButtonItem->setID("rating-button-" + std::to_string(i));
        m_normalButtonsContainer->addChild(ratingButtonItem);
    }

    // demon buttons (ratings 10-14)
    for (int i = 10; i <= 14; i++)
    {
        auto buttonBg = CCSprite::create("GJ_button_04.png");
        auto buttonLabel = CCLabelBMFont::create(std::to_string(i).c_str(), "bigFont.fnt");
        buttonLabel->setScale(0.75f);
        buttonLabel->setPosition(buttonBg->getContentSize() / 2);
        buttonBg->addChild(buttonLabel);
        buttonBg->setID("button-bg-" + std::to_string(i));

        auto ratingButtonItem = CCMenuItemSpriteExtra::create(
            buttonBg,
            this,
            menu_selector(ModRatePopup::onRatingButton));

        ratingButtonItem->setPosition({startX + (i - 10) * buttonSpacing, firstRowY});
        ratingButtonItem->setTag(i);
        ratingButtonItem->setID("rating-button-" + std::to_string(i));
        m_demonButtonsContainer->addChild(ratingButtonItem);
    }

    menuButtons->addChild(m_normalButtonsContainer);
    menuButtons->addChild(m_demonButtonsContainer);
    m_demonButtonsContainer->setVisible(false);

    // demon toggle
    auto offDemonSprite = CCSpriteGrayscale::createWithSpriteFrameName("GJ_demonIcon_001.png");
    auto onDemonSprite = CCSprite::createWithSpriteFrameName("GJ_demonIcon_001.png");
    auto demonToggle = CCMenuItemToggler::create(
        offDemonSprite,
        onDemonSprite,
        this,
        menu_selector(ModRatePopup::onToggleDemon));

    demonToggle->setPosition({m_mainLayer->getContentSize().width, 0});
    demonToggle->setScale(1.2f);
    menuButtons->addChild(demonToggle);

    // submit button
    auto submitButtonSpr = ButtonSprite::create("Submit", 1.f);
    auto submitButtonItem = CCMenuItemSpriteExtra::create(
        submitButtonSpr,
        this,
        menu_selector(ModRatePopup::onSubmitButton));

    submitButtonItem->setPosition({m_mainLayer->getContentSize().width / 2, 0});
    menuButtons->addChild(submitButtonItem);

    // toggle between normal or demon difficulty
    auto offSprite = CCSpriteGrayscale::create("rlfeaturedCoin.png"_spr);
    auto onSprite = CCSprite::create("rlfeaturedCoin.png"_spr);
    auto toggleDif = CCMenuItemToggler::create(
        offSprite,
        onSprite,
        this,
        menu_selector(ModRatePopup::onToggleDifficulty));

    toggleDif->setPosition({0, 0});
    menuButtons->addChild(toggleDif);

    m_mainLayer->addChild(menuButtons);

    // difficulty sprite on the right side (NA face by default)
    m_difficultySprite = GJDifficultySprite::create(0, (GJDifficultyName)-1);
    m_difficultySprite->setPosition({m_mainLayer->getContentSize().width - 50.f, 90.f});
    m_difficultySprite->setScale(1.2f);
    m_mainLayer->addChild(m_difficultySprite);

    return true;
}

void ModRatePopup::onSubmitButton(CCObject *sender)
{
    return;
}

void ModRatePopup::onToggleDifficulty(CCObject *sender)
{
    return;
}

void ModRatePopup::onToggleDemon(CCObject *sender)
{
    m_isDemonMode = !m_isDemonMode;
    log::info("Demon mode: {}", m_isDemonMode);

    m_normalButtonsContainer->setVisible(!m_isDemonMode);
    m_demonButtonsContainer->setVisible(m_isDemonMode);
}

void ModRatePopup::onRatingButton(CCObject *sender)
{
    auto button = static_cast<CCMenuItemSpriteExtra *>(sender);
    int rating = button->getTag();

    // reset the bg of the previously selected button i think
    if (m_selectedRating != -1)
    {
        auto prevButton = m_normalButtonsContainer->getChildByID("rating-button-" + std::to_string(m_selectedRating));
        if (!prevButton && m_selectedRating >= 10)
        {
            prevButton = m_demonButtonsContainer->getChildByID("rating-button-" + std::to_string(m_selectedRating));
        }

        if (prevButton)
        {
            auto prevButtonItem = static_cast<CCMenuItemSpriteExtra *>(prevButton);
            auto prevButtonBg = CCSprite::create("GJ_button_04.png");
            auto prevButtonLabel = CCLabelBMFont::create(std::to_string(m_selectedRating).c_str(), "bigFont.fnt");
            prevButtonLabel->setScale(0.75f);
            prevButtonLabel->setPosition(prevButtonBg->getContentSize() / 2);
            prevButtonBg->addChild(prevButtonLabel);
            prevButtonBg->setID("button-bg-" + std::to_string(m_selectedRating));
            prevButtonItem->setNormalImage(prevButtonBg);
        }
    }

    auto currentButton = static_cast<CCMenuItemSpriteExtra *>(sender);
    auto currentButtonBg = CCSprite::create("GJ_button_01.png");
    auto currentButtonLabel = CCLabelBMFont::create(std::to_string(button->getTag()).c_str(), "bigFont.fnt");
    currentButtonLabel->setScale(0.75f);
    currentButtonLabel->setPosition(currentButtonBg->getContentSize() / 2);
    currentButtonBg->addChild(currentButtonLabel);
    currentButtonBg->setID("button-bg-" + std::to_string(button->getTag()));
    currentButton->setNormalImage(currentButtonBg);

    m_selectedRating = button->getTag();

    log::info("Rating button clicked: {}", rating);
    updateDifficultySprite(rating);
}

void ModRatePopup::updateDifficultySprite(int rating)
{
    if (m_difficultySprite)
    {
        m_difficultySprite->removeFromParent();
    }

    int difficultyLevel;
    GJDifficultyName difficulty;

    switch (rating)
    {
    case 1:
        difficultyLevel = -1;
        difficulty = (GJDifficultyName)-1; // Auto
        break;
    case 2:
        difficultyLevel = 1;
        difficulty = (GJDifficultyName)1; // Easy
        break;
    case 3:
        difficultyLevel = 2;
        difficulty = (GJDifficultyName)2; // Normal
        break;
    case 4:
    case 5:
        difficultyLevel = 3;
        difficulty = (GJDifficultyName)3; // Hard
        break;
    case 6:
    case 7:
        difficultyLevel = 4;
        difficulty = (GJDifficultyName)4; // Harder
        break;
    case 8:
    case 9:
        difficultyLevel = 5;
        difficulty = (GJDifficultyName)5; // Insane
        break;
    case 10:
        difficultyLevel = 7;
        difficulty = (GJDifficultyName)7; // Easy Demon
        break;
    case 11:
        difficultyLevel = 8;
        difficulty = (GJDifficultyName)8; // Medium Demon
        break;
    case 12:
        difficultyLevel = 6;
        difficulty = (GJDifficultyName)6; // Hard Demon
        break;
    case 13:
        difficultyLevel = 9;
        difficulty = (GJDifficultyName)9; // Insane Demon
        break;
    case 14:
        difficultyLevel = 10;
        difficulty = (GJDifficultyName)10; // Extreme Demon
        break;
    default:
        difficultyLevel = 0;
        difficulty = (GJDifficultyName)0; // Default to NA
        break;
    }

    m_difficultySprite = GJDifficultySprite::create(difficultyLevel, difficulty);
    m_difficultySprite->setPosition({m_mainLayer->getContentSize().width - 50.f, 90.f});
    m_difficultySprite->setScale(1.2f);
    m_mainLayer->addChild(m_difficultySprite);
}

ModRatePopup *ModRatePopup::create(std::string title)
{
    auto ret = new ModRatePopup();

    if (ret && ret->initAnchored(380.f, 180.f, title, "GJ_square02.png"))
    {
        ret->autorelease();
        return ret;
    };

    CC_SAFE_DELETE(ret);
    return nullptr;
};