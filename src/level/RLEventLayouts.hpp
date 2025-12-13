#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RLEventLayouts : public geode::Popup<> {
     public:
      static RLEventLayouts* create();

     private:
      bool setup() override;
      void update(float dt) override;
      void onPlayEvent(CCObject* sender);

      struct EventSection {
            LevelCell* root = nullptr;
            CCLayer* container = nullptr;
            GJDifficultySprite* diff = nullptr;
            CCLabelBMFont* timerLabel = nullptr;
            CCLabelBMFont* levelNameLabel = nullptr;
            CCLabelBMFont* creatorLabel = nullptr;
            int levelId = -1;
            time_t createdAt = 0;
            double secondsLeft = 0.0;
      };
      EventSection m_sections[3];
      CCMenu* m_eventMenu = nullptr;
      bool m_setupFinished = false;
      std::vector<std::pair<Ref<LevelInfoLayer>, GJGameLevel*>> m_pendingLayers;
};