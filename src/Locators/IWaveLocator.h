//
// Created by Michael Hirsch on 01.03.26.
//

#pragma once

struct IWaveLocator {

  virtual ~IWaveLocator() = default;

  virtual int GetWaveCounter() const = 0;

};