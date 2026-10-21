//
// Created by Adrien BLANCHET on 22/06/2020.
//

#include "TabModOptions.h"
#include "FrameModBrowser.h"




#include "Logger.h"

LoggerInit([]{
  Logger::setUserHeaderStr("[TabModOptions]");
});


TabModOptions::TabModOptions(FrameModBrowser* owner_) : _owner_(owner_) {  }

void TabModOptions::buildFolderInstallPresetItem() {

  _itemConfigPreset_ = new brls::ListItem(
    "\uE255 配置预设",
    "指定此子文件夹（游戏）中的 mod 将从哪个安装文件夹安装。\n",
    ""
  );

  _itemConfigPreset_->setValue("继承自主菜单");
  if( not this->getModManager().getCurrentPresetName().empty() ){
    _itemConfigPreset_->setValue( this->getModManager().getCurrentPresetName() );
  }

  // On click : show scrolling up menu
  _itemConfigPreset_->getClickEvent()->subscribe([this](View* view) {
    LogInfo << "Opening config preset selector..." << std::endl;

    // build the choice list + preselection
    int preSelection{0};
    std::vector<std::string> menuList;
    menuList.reserve( 1 + this->getModManager().getConfig().presetList.size() );
    menuList.emplace_back( "继承自主菜单" );
    int iPreset{-1};
    for( auto& preset: this->getModManager().getConfig().presetList ){
      iPreset++;
      menuList.emplace_back( preset.name + " \uE090 \"" + preset.installBaseFolder + "\"" );
      if( preset.name == this->getModManager().getCurrentPresetName() ){ preSelection = 1 + iPreset; }
    }

    // function that will set the config preset from the Dropdown menu selection (int result)
    brls::ValueSelectedEvent::Callback valueCallback = [this](int result) {
      if( result == -1 ){
        LogDebug << "Not selected. Return." << std::endl;
        // auto pop view
        return;
      }

      if( result == 0 ){
        LogDebug << "Same as config selected. Deleting file..." << std::endl;
        this->getModManager().setCustomPreset("");
        _itemConfigPreset_->setValue( "继承自主菜单" );
      }
      else{
        LogDebug << "Selected " << result - 1 << std::endl;
        this->getModManager().setCustomPreset( this->getModManager().getConfig().presetList[result - 1].name );
        _itemConfigPreset_->setValue( this->getModManager().getCurrentPresetName() );
      }

      _owner_->getGuiModManager().checkAllMods( true );
      brls::Application::popView();
      return;
    }; // Callback sequence

    brls::Dropdown::open(
        "请选择此文件夹要使用的配置预设",
        menuList, valueCallback,
        preSelection,
        true
    );

  });

}
void TabModOptions::buildResetModsCacheItem() {

  _itemResetModsCache_ = new brls::ListItem(
    "\uE877 重新检查所有 mod",
    "\t此选项会重置所有 mod 的缓存状态，并重新检查每个文件是否正确应用。\n"
              "\t当文件并非通过 SimpleModManager 管理时，显示的 mod 状态可能不准确。"
              "这通常发生在你修改了某些 mod 文件，或其他程序覆盖了部分已应用的文件时。",
    ""
  );

  _itemResetModsCache_->getClickEvent()->subscribe([this](View* view){

    auto* dialog = new brls::Dialog("是否要重置 mod 缓存状态并重新检查所有 mod 文件？");

    dialog->addButton("是", [&, dialog](brls::View* view) {
      // first, close the dialog box before the async routine starts
      dialog->close();

      // starts the async routine
      _owner_->getGuiModManager().startCheckAllModsThread();
    });
    dialog->addButton("否", [dialog](brls::View* view) {
      dialog->close();
    });

    dialog->setCancelable(true);
    dialog->open();

  });

}
void TabModOptions::buildDisableAllMods() {

  _itemDisableAllMods_ = new brls::ListItem(
    "\uE872 禁用所有已安装的 mod",
    "此选项会移除所有已安装的 mod 文件。\n"
              "注意：只有与此文件夹中完全相同的已安装 mod 文件才会被删除。",
    ""
  );

  _itemDisableAllMods_->getClickEvent()->subscribe([this](View* view){

    auto* dialog = new brls::Dialog("是否要禁用所有已安装的 mod？");

    dialog->addButton("是", [&, dialog](brls::View* view) {
      // first, close the dialog box before the async routine starts
      dialog->close();

      // starts the async routine
      _owner_->getGuiModManager().startRemoveAllModsThread();
    });
    dialog->addButton("否", [dialog](brls::View* view) {
      dialog->close();
    });

    dialog->setCancelable(true);
    dialog->open();

  });

}
void TabModOptions::buildGameIdentificationItem(){

  _itemGameIdentification_ = new brls::ListItem(
    "关联 TitleID",
    "",
    "当前值："
  );

  if( _owner_->getIcon() != nullptr ){
    _itemGameIdentification_->setThumbnail( _owner_->getIcon(), 0x20000 );
    _itemGameIdentification_->setSubLabel(
        _itemGameIdentification_->getSubLabel() + _owner_->getTitleId()
    );
  }
  else{
    _itemGameIdentification_->setSubLabel(
        _itemGameIdentification_->getSubLabel() + "未找到"
    );
  }

}

void TabModOptions::initialize() {

  this->buildFolderInstallPresetItem();
  this->buildResetModsCacheItem();
  this->buildDisableAllMods();
  this->buildGameIdentificationItem();

  // finally add to view
  this->addView(_itemResetModsCache_);
  this->addView(_itemConfigPreset_);
  this->addView(_itemDisableAllMods_);
  this->addView(_itemGameIdentification_);

}

void TabModOptions::draw(NVGcontext *vg, int x, int y, unsigned int width, unsigned int height, brls::Style *style,
                         brls::FrameContext *ctx) {
  ScrollView::draw(vg, x, y, width, height, style, ctx);
}

const ModManager &TabModOptions::getModManager() const {
  return _owner_->getGameBrowser().getModManager();
}
ModManager &TabModOptions::getModManager() {
  return _owner_->getGameBrowser().getModManager();
}




