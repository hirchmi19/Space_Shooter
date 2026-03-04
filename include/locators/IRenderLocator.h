#pragma once

struct MessageUi;

struct IRenderLocator {

    virtual ~IRenderLocator() = default;

    virtual void AddMessage(const MessageUi& msg) = 0;
    virtual  void AddScore(const MessageUi& score) = 0;
    virtual void ClearUi() = 0;


};
