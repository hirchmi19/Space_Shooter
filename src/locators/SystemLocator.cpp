//
// Created by Michael Hirsch on 26.02.26.
//

#include "locators/SystemLocator.h"
#include "locators/IEntityLocator.h"

std::shared_ptr<IAssetLocator> SystemLocator::assetLocator = nullptr;
std::shared_ptr<IScoreLocator> SystemLocator::scoreLocator = nullptr;
std::shared_ptr<IEntityLocator> SystemLocator::entityLocator = nullptr;
std::shared_ptr<IWaveLocator> SystemLocator::waveLocator = nullptr;
std::shared_ptr<ITimerLocator> SystemLocator::timerLocator = nullptr;