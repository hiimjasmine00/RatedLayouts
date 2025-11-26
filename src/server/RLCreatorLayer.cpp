#include "RLCreatorLayer.hpp"

#include "LevelSearchLayer.cpp"
#include "RLLeaderboardLayer.hpp"

bool RLCreatorLayer::init() {
      if (!CCLayer::init())
            return false;

      auto winSize = CCDirector::sharedDirector()->getWinSize();

      addSideArt(this, SideArt::All, SideArtStyle::LayerGray, false);

      auto backMenu = CCMenu::create();
      backMenu->setPosition({0, 0});

      auto backButtonSpr =
          CCSprite::createWithSpriteFrameName("GJ_arrow_03_001.png");
      auto backButton = CCMenuItemSpriteExtra::create(
          backButtonSpr, this, menu_selector(RLCreatorLayer::onBackButton));
      backButton->setPosition({25, winSize.height - 25});
      backMenu->addChild(backButton);
      this->addChild(backMenu);

      auto mainMenu = CCMenu::create();
      mainMenu->setPosition({winSize.width / 2, winSize.height / 2});
      mainMenu->setContentSize({300.f, 240.f});
      mainMenu->setLayout(RowLayout::create()
                              ->setGap(6.f)
                              ->setGrowCrossAxis(true)
                              ->setCrossAxisOverflow(false));

      this->addChild(mainMenu);

      auto featuredSpr = CCSprite::create("RL_featuredBtn.png"_spr);
      if (!featuredSpr) featuredSpr = CCSprite::create("RL_featuredBtn.png"_spr);
      auto featuredItem = CCMenuItemSpriteExtra::create(
          featuredSpr, this, menu_selector(RLCreatorLayer::onFeaturedLayouts));
      featuredItem->setID("featured-button");
      mainMenu->addChild(featuredItem);

      auto leaderboardSpr = CCSprite::create("RL_leaderboardBtn.png"_spr);
      if (!leaderboardSpr) leaderboardSpr = CCSprite::create("RL_leaderboardBtn.png"_spr);
      auto leaderboardItem = CCMenuItemSpriteExtra::create(
          leaderboardSpr, this, menu_selector(RLCreatorLayer::onLeaderboard));
      leaderboardItem->setID("leaderboard-button");
      mainMenu->addChild(leaderboardItem);

      auto newlySpr = CCSprite::create("RL_newRatedBtn.png"_spr);
      if (!newlySpr) newlySpr = CCSprite::create("RL_newRatedBtn.png"_spr);
      auto newlyItem = CCMenuItemSpriteExtra::create(
          newlySpr, this, menu_selector(RLCreatorLayer::onNewRated));
      newlyItem->setID("newly-rated-button");
      mainMenu->addChild(newlyItem);

      auto sendSpr = CCSprite::create("RL_sendLayoutsBtn.png"_spr);
      if (!sendSpr) sendSpr = CCSprite::create("RL_sendLayoutsBtn.png"_spr);
      auto sendItem = CCMenuItemSpriteExtra::create(
          sendSpr, this, menu_selector(RLCreatorLayer::onSendLayouts));
      sendItem->setID("send-layouts-button");
      mainMenu->addChild(sendItem);
      mainMenu->updateLayout();

      // test the ground moving thingy :o
      // idk how gd actually does it correctly but this is close enough i guess
      m_bgContainer = CCNode::create();
      m_bgContainer->setContentSize(winSize);
      this->addChild(m_bgContainer, -4);

      std::string bgName = "game_bg_01_001.png";
      auto testBg = CCSprite::create(bgName.c_str());
      if (!testBg) {
            testBg = CCSprite::create("game_bg_01_001.png");
      }
      if (testBg) {
            float tileW = testBg->getContentSize().width;
            int tiles = static_cast<int>(ceil(winSize.width / tileW)) + 2;
            for (int i = 0; i < tiles; ++i) {
                  auto bgSpr = CCSprite::create(bgName.c_str());
                  if (!bgSpr) bgSpr = CCSprite::create("game_bg_01_001.png");
                  if (!bgSpr) continue;
                  bgSpr->setAnchorPoint({0.f, 0.f});
                  bgSpr->setPosition({i * tileW, 0.f});
                  bgSpr->setColor({40, 125, 255});
                  m_bgContainer->addChild(bgSpr);
                  m_bgTiles.push_back(bgSpr);
            }
      }

      m_groundContainer = CCNode::create();
      m_groundContainer->setContentSize(winSize);
      this->addChild(m_groundContainer, -3);

      std::string groundName = "groundSquare_01_001.png";
      auto testGround = CCSprite::create(groundName.c_str());
      if (!testGround) testGround = CCSprite::create("groundSquare_01_001.png");
      if (testGround) {
            float tileW = testGround->getContentSize().width;
            int tiles = static_cast<int>(ceil(winSize.width / tileW)) + 2;
            for (int i = 0; i < tiles; ++i) {
                  auto gSpr = CCSprite::create(groundName.c_str());
                  if (!gSpr) gSpr = CCSprite::create("groundSquare_01_001.png");
                  if (!gSpr) continue;
                  gSpr->setAnchorPoint({0.f, 0.f});
                  gSpr->setPosition({i * tileW, -70.f});
                  gSpr->setColor({0, 102, 255});
                  m_groundContainer->addChild(gSpr);
                  m_groundTiles.push_back(gSpr);
            }
      }

      auto floorLineSpr = CCSprite::createWithSpriteFrameName("floorLine_01_001.png");
      floorLineSpr->setPosition({winSize.width / 2, 58});
      m_groundContainer->addChild(floorLineSpr, 0);

      this->scheduleUpdate();
      this->setKeypadEnabled(true);

      return true;
}

void RLCreatorLayer::onBackButton(CCObject* sender) {
      CCDirector::sharedDirector()->popSceneWithTransition(
          0.5f, PopTransition::kPopTransitionFade);
}

void RLCreatorLayer::onLeaderboard(CCObject* sender) {
      auto leaderboardLayer = RLLeaderboardLayer::create();
      auto scene = CCScene::create();
      scene->addChild(leaderboardLayer);
      auto transitionFade = CCTransitionFade::create(0.5f, scene);
      CCDirector::sharedDirector()->pushScene(transitionFade);
}

void RLCreatorLayer::onFeaturedLayouts(CCObject* sender) {
      web::WebRequest()
          .param("type", 2)
          .param("amount", 1000)
          .get("https://gdrate.arcticwoof.xyz/getLevels")
          .listen([this](web::WebResponse* res) {
                if (res && res->ok()) {
                      auto jsonResult = res->json();

                      if (jsonResult) {
                            auto json = jsonResult.unwrap();
                            std::string levelIDs;
                            bool first = true;

                            if (json.contains("levelIds")) {
                                  auto levelsArr = json["levelIds"];

                                  // iterate
                                  for (auto levelIDValue : levelsArr) {
                                        auto levelID = levelIDValue.as<int>();
                                        if (levelID) {
                                              if (!first)
                                                    levelIDs += ",";
                                              levelIDs += numToString(levelID.unwrap());
                                              first = false;
                                        }
                                  }
                            }

                            if (!levelIDs.empty()) {
                                  auto searchObject =
                                      GJSearchObject::create(SearchType::Type19, levelIDs);
                                  auto browserLayer = LevelBrowserLayer::create(searchObject);
                                  auto scene = CCScene::create();
                                  scene->addChild(browserLayer);
                                  auto transitionFade = CCTransitionFade::create(0.5f, scene);
                                  CCDirector::sharedDirector()->pushScene(transitionFade);
                            } else {
                                  log::warn("No levels found in response");
                                  Notification::create("No featured levels found",
                                                       NotificationIcon::Warning)
                                      ->show();
                            }
                      } else {
                            log::error("Failed to parse response JSON");
                      }
                } else {
                      log::error("Failed to fetch levels from server");
                      Notification::create("Failed to fetch levels from server",
                                           NotificationIcon::Error)
                          ->show();
                }
          });
}

void RLCreatorLayer::onNewRated(CCObject* sender) {
      web::WebRequest()
          .param("type", 3)
          .param("amount", 1000)
          .get("https://gdrate.arcticwoof.xyz/getLevels")
          .listen([this](web::WebResponse* res) {
                if (res && res->ok()) {
                      auto jsonResult = res->json();

                      if (jsonResult) {
                            auto json = jsonResult.unwrap();
                            std::string levelIDs;
                            bool first = true;

                            if (json.contains("levelIds")) {
                                  auto levelsArr = json["levelIds"];

                                  // iterate
                                  for (auto levelIDValue : levelsArr) {
                                        auto levelID = levelIDValue.as<int>();
                                        if (levelID) {
                                              if (!first)
                                                    levelIDs += ",";
                                              levelIDs += numToString(levelID.unwrap());
                                              first = false;
                                        }
                                  }
                            }

                            if (!levelIDs.empty()) {
                                  auto searchObject =
                                      GJSearchObject::create(SearchType::Type19, levelIDs);
                                  auto browserLayer = LevelBrowserLayer::create(searchObject);
                                  auto scene = CCScene::create();
                                  scene->addChild(browserLayer);
                                  auto transitionFade = CCTransitionFade::create(0.5f, scene);
                                  CCDirector::sharedDirector()->pushScene(transitionFade);
                            } else {
                                  log::warn("No levels found in response");
                                  Notification::create("No levels found",
                                                       NotificationIcon::Warning)
                                      ->show();
                            }
                      } else {
                            log::error("Failed to parse response JSON");
                      }
                } else {
                      log::error("Failed to fetch levels from server");
                      Notification::create("Failed to fetch levels from server",
                                           NotificationIcon::Error)
                          ->show();
                }
          });
}

void RLCreatorLayer::onSendLayouts(CCObject* sender) {
      web::WebRequest()
          .param("type", 1)
          .param("amount", 1000)
          .get("https://gdrate.arcticwoof.xyz/getLevels")
          .listen([this](web::WebResponse* res) {
                if (res && res->ok()) {
                      auto jsonResult = res->json();

                      if (jsonResult) {
                            auto json = jsonResult.unwrap();
                            std::string levelIDs;
                            bool first = true;
                            if (json.contains("levelIds")) {
                                  auto levelsArr = json["levelIds"];

                                  // iterate
                                  for (auto levelIDValue : levelsArr) {
                                        auto levelID = levelIDValue.as<int>();
                                        if (levelID) {
                                              if (!first)
                                                    levelIDs += ",";
                                              levelIDs += numToString(levelID.unwrap());
                                              first = false;
                                        }
                                  }
                            }

                            if (!levelIDs.empty()) {
                                  auto searchObject =
                                      GJSearchObject::create(SearchType::Type19, levelIDs);
                                  auto browserLayer = LevelBrowserLayer::create(searchObject);
                                  auto scene = CCScene::create();
                                  scene->addChild(browserLayer);
                                  auto transitionFade = CCTransitionFade::create(0.5f, scene);
                                  CCDirector::sharedDirector()->pushScene(transitionFade);
                            } else {
                                  log::warn("No levels found in response");
                                  Notification::create("No send layouts found",
                                                       NotificationIcon::Warning)
                                      ->show();
                            }
                      } else {
                            log::error("Failed to parse response JSON");
                      }
                } else {
                      log::error("Failed to fetch levels from server");
                      Notification::create("Failed to fetch levels from server",
                                           NotificationIcon::Error)
                          ->show();
                }
          });
}

void RLCreatorLayer::keyBackClicked() { this->onBackButton(nullptr); }

void RLCreatorLayer::update(float dt) {
      // scroll background tiles
      if (m_bgTiles.size()) {
            float move = m_bgSpeed * dt;
            int num = static_cast<int>(m_bgTiles.size());
            for (auto spr : m_bgTiles) {
                  if (!spr)
                        continue;
                  float tileW = spr->getContentSize().width;
                  float x = spr->getPositionX();
                  x -= move;
                  if (x <= -tileW) {
                        x += tileW * num;
                  }
                  spr->setPositionX(x);
            }
      }

      // scroll ground tiles
      if (m_groundTiles.size()) {
            float move = m_groundSpeed * dt;
            int num = static_cast<int>(m_groundTiles.size());
            for (auto spr : m_groundTiles) {
                  if (!spr)
                        continue;
                  float tileW = spr->getContentSize().width;
                  float x = spr->getPositionX();
                  x -= move;
                  if (x <= -tileW) {
                        x += tileW * num;
                  }
                  spr->setPositionX(x);
            }
      }
}

RLCreatorLayer* RLCreatorLayer::create() {
      auto ret = new RLCreatorLayer();
      if (ret && ret->init()) {
            ret->autorelease();
            return ret;
      }
      CC_SAFE_DELETE(ret);
      return nullptr;
}
