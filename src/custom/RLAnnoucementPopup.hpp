#include <Geode/Geode.hpp>

using namespace geode::prelude;

class RLAnnoucementPopup : public geode::Popup<> {
     public:
      static RLAnnoucementPopup* create();

     private:
      bool setup() override;
      void onClick(CCObject* sender);
};