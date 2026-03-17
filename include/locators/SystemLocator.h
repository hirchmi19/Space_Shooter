//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include <memory>
#include "IAssetLocator.h"
#include "IEntityLocator.h"
#include "IRenderLocator.h"
#include "IScoreLocator.h"
#include "ITimerLocator.h"
#include "IWaveLocator.h"

struct SystemLocator {
    static std::shared_ptr<IAssetLocator> assetLocator;
    static std::shared_ptr<IScoreLocator> scoreLocator;
    static std::shared_ptr<IEntityLocator> entityLocator;
    static std::shared_ptr<IWaveLocator> waveLocator;
    static std::shared_ptr<ITimerLocator> timerLocator;
    static std::shared_ptr<IRenderLocator> renderLocator;
};


