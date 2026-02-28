//
// Created by Michael Hirsch on 26.02.26.
//

#pragma once
#include <memory>

#include "IAssetLocator.h"
#include "IEntityLocator.h"
#include "IScoreLocator.h"

struct SystemLocator {

    static std::shared_ptr<IAssetLocator> assetLocator;
    static std::shared_ptr<IScoreLocator> scoreLocator;
    static std::shared_ptr<IEntityLocator> entityLocator;
};


