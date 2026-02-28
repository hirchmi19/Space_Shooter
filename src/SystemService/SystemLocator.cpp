//
// Created by Michael Hirsch on 26.02.26.
//

#include "SystemLocator.h"

#include "IEntityLocator.h"

std::shared_ptr<IAssetLocator> SystemLocator::assetLocator = nullptr;
std::shared_ptr<IScoreLocator> SystemLocator::scoreLocator = nullptr;
std::shared_ptr<IEntityLocator> SystemLocator::entityLocator = nullptr;