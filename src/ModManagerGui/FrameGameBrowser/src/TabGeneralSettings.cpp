//
// Created by Adrien BLANCHET on 21/06/2020.
//

#include <TabGeneralSettings.h>

#include "FrameRoot.h"


#include "Logger.h"

LoggerInit([]{
  Logger::setUserHeaderStr("[TabGeneralSettings]");
});

namespace{
  std::string getSortGameListLabel(const std::string& sortEnumStr_){
    if( sortEnumStr_ == "NbMods" )       { return "按 mod 数量"; }
    if( sortEnumStr_ == "Alphabetical" ) { return "按名称"; }
    if( sortEnumStr_ == "NoSort" )       { return "不排序"; }
    return sortEnumStr_;
  }
}

TabGeneralSettings::TabGeneralSettings(FrameRoot* owner_) : _owner_(owner_) {
  LogWarning << "Building general settings tab..." << std::endl;
  this->rebuildLayout();
  LogInfo << "General settings tab build." << std::endl;
}

void TabGeneralSettings::rebuildLayout() {

  itemInstallLocationPreset = new brls::ListItem(
    "\uE255 安装位置：",
    "指定 mod 从哪个基准文件夹安装。\n"\
    "- 如果你使用 Atmosphere，mod 需要安装到 /atmosphere/，这对应“default”预设。你需要在 mod 目录结构中考虑此路径。\n"\
    "- 如果你想为某个游戏单独指定安装文件夹，请进入该游戏的“选项”标签页，选择“配置预设”。",
    ""
  );
  itemInstallLocationPreset->setValue(this->getConfig().getCurrentPresetName() );
  itemInstallLocationPreset->getClickEvent()->subscribe([this](View* view) {
    brls::ValueSelectedEvent::Callback valueCallback = [this](int result) {
      if (result == -1) {
        return;
      }

      this->getConfig().setSelectedPresetIndex( result );
      _owner_->getGuiModManager().getGameBrowser().getConfigHandler().dumpConfigToFile();
      LogInfo << "Selected: " << this->getConfig().getCurrentPreset().name << " -> " << this->getConfig().getCurrentPreset().installBaseFolder << std::endl;
      this->itemInstallLocationPreset->setValue(this->getConfig().getCurrentPresetName() );
    };

    std::vector<std::string> presetNameList;
    presetNameList.reserve( this->getConfig().presetList.size() );
    for( auto& preset : this->getConfig().presetList ){
      presetNameList.emplace_back( preset.name + " \uE090 \"" + preset.installBaseFolder + "\"" );
    }

    brls::Dropdown::open(
      "当前安装预设：",
      presetNameList,
      valueCallback,
      this->getConfig().selectedPresetIndex
    );
  });
  this->addView(itemInstallLocationPreset);

  auto* itemStoredModsBaseFolder = new brls::ListItem(
    "\uE431 Mod 存放位置：",
    "这是 SimpleModManager 查找 mod 的位置。在此文件夹内，目录结构必须如下：\n"\
    "./<游戏或分类名称>/<mod 名称>/<mod 目录结构>。",
    "");
  itemStoredModsBaseFolder->setValue( this->getConfig().baseFolder );
  this->addView(itemStoredModsBaseFolder);


  auto*  itemSortGames = new brls::ListItem(
      "\uE255 游戏排序方式",
      "设置游戏列表中游戏的显示顺序。\n",
      ""
  );
itemSortGames->setValue( getSortGameListLabel( this->getConfig().sortGameList.toString() ) );


  // On click : show scrolling up menu
  itemSortGames->getClickEvent()->subscribe([this, itemSortGames](View* view) {
    LogInfo << "Opening itemSortGames selector..." << std::endl;

    // build the choice list + preselection
    int preSelection{0};
    std::vector<std::string> menuList;
	
    std::vector<std::string> enumStrList;
menuList.reserve( ConfigHolder::SortGameList::getEnumSize() );
    enumStrList.reserve( ConfigHolder::SortGameList::getEnumSize() );
for( int iEnum = 0 ; iEnum < ConfigHolder::SortGameList::getEnumSize() ; iEnum++ ){
      enumStrList.emplace_back( ConfigHolder::SortGameList::toString(iEnum) );
      menuList.emplace_back( getSortGameListLabel( enumStrList.back() ) );
      if( enumStrList.back() == this->getConfig().sortGameList.toString() ){ preSelection = iEnum; }
}

// function that will set the config preset from the Dropdown menu selection (int result)
    brls::ValueSelectedEvent::Callback valueCallback = [this, itemSortGames, enumStrList](int result) {
if( result == -1 ){
LogDebug << "Not selected. Return." << std::endl;
// auto pop view
return;
}

      LogInfo << "Selected: " << enumStrList[result] << std::endl;
      this->getConfig().sortGameList = ConfigHolder::SortGameList::toEnum( enumStrList[result] );
_owner_->getGuiModManager().getGameBrowser().getConfigHandler().dumpConfigToFile();
      itemSortGames->setValue( getSortGameListLabel( this->getConfig().sortGameList.toString() ) );

      brls::Application::popView();
      return;
    }; // Callback sequence

    brls::Dropdown::open(
        "请选择你想要的排序方式",
        menuList, valueCallback,
        preSelection,
        true
    );

  });
  this->addView(itemSortGames);


  auto* itemUseUI = new brls::ListItem("\uE072 禁用图形界面", "如果你想返回旧的命令行界面，请选择此选项。");

  itemUseUI->updateActionHint(brls::Key::B, "返回");
  itemUseUI->registerAction("选择", brls::Key::A, [&](){

    auto* dialog = new brls::Dialog("是否要禁用当前图形界面，并切换回命令行界面？");

    dialog->addButton("是", [&](brls::View* view) {
      this->getConfig().useGui = false;
      _owner_->getGuiModManager().getGameBrowser().getConfigHandler().dumpConfigToFile();
      brls::Application::quit();
    });
    dialog->addButton("否", [dialog](brls::View* view) {
      dialog->close();
    });

    dialog->setCancelable(true);
    dialog->open();

    return true;
  });
  this->addView(itemUseUI);

}

const ConfigHolder& TabGeneralSettings::getConfig() const{
  return _owner_->getGuiModManager().getGameBrowser().getConfigHandler().getConfig();
}
ConfigHolder& TabGeneralSettings::getConfig(){
  return _owner_->getGuiModManager().getGameBrowser().getConfigHandler().getConfig();
}
