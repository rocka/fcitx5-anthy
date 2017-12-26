//
// Copyright (C) 2017~2017 by CSSlayer
// wengxt@gmail.com
//
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2, or (at your option)
// any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.
//
#ifndef _FCITX5_ANTHY_ENGINE_H_
#define _FCITX5_ANTHY_ENGINE_H_

#include "action.h"
#include "key2kana_table.h"
#include "style_file.h"
#include <anthy/anthy.h>
#include <fcitx-config/enum.h>
#include <fcitx-config/iniparser.h>
#include <fcitx-utils/i18n.h>
#include <fcitx/addonmanager.h>
#include <fcitx/candidatelist.h>
#include <fcitx/inputcontext.h>
#include <fcitx/inputcontextproperty.h>
#include <fcitx/inputmethodengine.h>
#include <fcitx/instance.h>
#include <fcitx/menu.h>

FCITX_DECLARE_LOG_CATEGORY(anthy_logcategory);

#define FCITX_ANTHY_DEBUG() FCITX_LOGC(anthy_logcategory, Debug)

class AnthyEngine : public fcitx::InputMethodEngine {
public:
    AnthyEngine(fcitx::Instance *instance);
    ~AnthyEngine();
    void keyEvent(const fcitx::InputMethodEntry &entry,
                  fcitx::KeyEvent &keyEvent) override;
    void reloadConfig() override;
    std::string subMode(const fcitx::InputMethodEntry &,
                        fcitx::InputContext &) override;
    void activate(const fcitx::InputMethodEntry &,
                  fcitx::InputContextEvent &) override;
    void deactivate(const fcitx::InputMethodEntry &entry,
                    fcitx::InputContextEvent &event) override;

    const fcitx::Configuration *getConfig() const override { return &config_; }
    void setConfig(const fcitx::RawConfig &config) override {
        config_.load(config, true);
        saveConfig();
        reloadConfig();
    }

    void saveConfig() { fcitx::safeSaveAsIni(config_, "conf/anthy.conf"); }

    auto state(fcitx::InputContext *ic) { return ic->propertyFor(&factory_); }
    auto &config() { return config_; }
    auto keyProfile() { return keyProfileFileLoaded_ ? &keyProfile_ : nullptr; }
    auto customRomajiTable() {
        return customRomajiTableLoaded_ ? &customRomajiTable_ : nullptr;
    }
    auto customKanaTable() {
        return customKanaTableLoaded_ ? &customKanaTable_ : nullptr;
    }
    auto customNicolaTable() {
        return customNicolaTableLoaded_ ? &customNicolaTable_ : nullptr;
    }

    FCITX_ADDON_DEPENDENCY_LOADER(clipboard, instance_->addonManager());

    auto inputModeAction() { return inputModeAction_.get(); }
    auto typingMethodAction() { return typingMethodAction_.get(); }
    auto conversionModeAction() { return conversionModeAction_.get(); }
    auto periodStyleAction() { return periodStyleAction_.get(); }
    auto symbolStyleAction() { return symbolStyleAction_.get(); }

private:
    std::string keyProfileName();
    std::string romajiTableName();
    std::string kanaTableName();
    std::string nicolaTableName();
    std::string fullFileName(const std::string &name);

    fcitx::Instance *instance_;
    fcitx::FactoryFor<AnthyState> factory_;
    AnthyConfig config_;

    bool keyProfileFileLoaded_ = false;
    AnthyKeyProfile keyProfile_;

    bool customRomajiTableLoaded_ = false;
    Key2KanaTable customRomajiTable_{"custom-romaji"};

    bool customKanaTableLoaded_ = false;
    Key2KanaTable customKanaTable_{"custom-kana"};

    bool customNicolaTableLoaded_ = false;
    Key2KanaTable customNicolaTable_{"custom-nicola"};

    std::unique_ptr<fcitx::Action> inputModeAction_;
    std::unique_ptr<fcitx::Action> typingMethodAction_;
    std::unique_ptr<fcitx::Action> conversionModeAction_;
    std::unique_ptr<fcitx::Action> periodStyleAction_;
    std::unique_ptr<fcitx::Action> symbolStyleAction_;
    std::vector<std::unique_ptr<fcitx::Action>> subModeActions_;

    fcitx::Menu inputModeMenu_;
    fcitx::Menu typingMethodMenu_;
    fcitx::Menu conversionModeMenu_;
    fcitx::Menu periodStyleMenu_;
    fcitx::Menu symbolStyleMenu_;
};

#endif // _FCITX5_ANTHY_ENGINE_H_