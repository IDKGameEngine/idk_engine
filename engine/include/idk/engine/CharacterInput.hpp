#pragma once

#include "CharacterController.hpp"


namespace idk
{
    class Command
    {
    public:
        virtual ~Command() {}
        virtual void execute(CharacterController*) = 0;
    };

}


// class JumpCommand : public idk::Command
// {
// public:
//     virtual void execute(idk::CharacterController *ctl) { ctl->jump(); }
// };

// class AttackCommand : public idk::Command
// {
// public:
//     virtual void execute(idk::CharacterController *ctl) { ctl->attack(); }
// };



// class InputHandler
// {
// private:
//     idk::Command *mBtnX;
//     idk::Command *mBtnY;
//     idk::Command *mBtnA;
//     idk::Command *mBtnB;

// public:
//     idk::Command *handleInput()
//     {
//         if (0) return mBtnX;
//         if (0) return mBtnY;
//         if (0) return mBtnA;
//         if (0) return mBtnB;
//         return nullptr;
//     }
// };


// void sauftad()
// {
//     using namespace idk;
//     InputHandler ih;

//     if (Command *cmd = ih.handleInput())
//     {
//         cmd->execute(actor);
//     }
// }
