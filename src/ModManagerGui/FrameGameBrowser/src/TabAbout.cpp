//
// Created by Adrien BLANCHET on 20/06/2020.
//

#include "TabAbout.h"
#include <Toolbox.h>

#include "Logger.h"

#include <borealis.hpp>



LoggerInit([]{
  Logger::setUserHeaderStr("[TabAbout]");
});

TabAbout::TabAbout() {
  LogWarning << "Building about tab..." << std::endl;

  // Subtitle
  auto* shortDescription = new brls::Label(
    brls::LabelStyle::REGULAR,
    "SimpleModManager 是一款 Nintendo Switch 自制软件，可帮助你管理 SD 卡上的 mod 与配置文件。",
    true
  );
  shortDescription->setHorizontalAlign(NVG_ALIGN_CENTER);
  this->addView(shortDescription);

  auto* table = new brls::BoxLayout(brls::BoxLayoutOrientation::HORIZONTAL);
  table->setSpacing(22);
  table->setHeight(260);

  auto* leftBox = new brls::BoxLayout(brls::BoxLayoutOrientation::VERTICAL);
  leftBox->setSpacing(22);
  leftBox->setWidth(500);
  leftBox->setParent(table);

  leftBox->addView(new brls::Header("版本 " + Toolbox::getAppVersion() + " - 更新内容"));
  auto *changelog = new brls::Label(
    brls::LabelStyle::DESCRIPTION,
    " - UI界面中文化\n - 稳定性改进\n",
    true
  );
  changelog->setHorizontalAlign(NVG_ALIGN_LEFT);
  leftBox->addView(changelog);

  leftBox->addView(new brls::Header("版权信息"));
  auto *copyright = new brls::Label(
    brls::LabelStyle::DESCRIPTION,
    "SimpleModManager 基于 GPL-v3.0 协议授权\n" \
        "\u00A9 2019 - 2023 Nadrino\n" \
        "\u00A9 2026 gzk_47",
    true
  );
  copyright->setHorizontalAlign(NVG_ALIGN_CENTER);
  leftBox->addView(copyright);

  auto* rightBox = new brls::BoxLayout(brls::BoxLayoutOrientation::VERTICAL);
  rightBox->setSpacing(22);
  rightBox->setWidth(200);
  rightBox->setParent(table);

  rightBox->addView(new brls::Label(brls::LabelStyle::DESCRIPTION, " "));

  auto* portrait = new brls::Image("romfs:/images/portrait.jpg");
  portrait->setScaleType(brls::ImageScaleType::SCALE);
  portrait->setHeight(200);
  portrait->setParent(rightBox);
  rightBox->addView(portrait);
  auto* portraitText = new brls::Label(brls::LabelStyle::SMALL, "作者：Nadrino, gzk_47", true);
  portraitText->setHorizontalAlign(NVG_ALIGN_CENTER);
  rightBox->addView(portraitText);

  table->addView(leftBox);
  table->addView(rightBox);

  this->addView(table);

  this->addView(new brls::Header("致谢"));
  auto *links = new brls::Label(
    brls::LabelStyle::SMALL,
    "\uE017  SimpleModManager 由 Borealis 硬件加速 UI 库提供支持\n" \
         "\uE017  特别感谢 RetroNX 团队对 Borealis 的支持\n",
    true
  );
  this->addView(links);

  LogInfo << "About tab built." << std::endl;
}
