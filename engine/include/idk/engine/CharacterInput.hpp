#pragma once

namespace idk::engine
{
    struct CharacterInput
    {
        float moveX = 0.0f;
        float moveY = 0.0f;

        float lookX = 0.0f;
        float lookY = 0.0f;

        bool jump = false;
        bool sprint = false;
    };


    // class Actor
    // {
    // public:
        
    // };

    // class CharacterController
    // {
    // public:
    //     virtual ~CharacterController() = default;
    //     virtual void Update(float dt, const CharacterInput& input) = 0;
    // };

    // class PlayerCharacterController : public CharacterController
    // {
    // private:
    //     Actor *mActor;

    // public:
    //     PlayerCharacterController(Actor *A): mActor(A) {  };

    //     void Update(float dt, const CharacterInput& input) override
    //     {
    //         mActor->move(input.moveX, input.moveY, dt);
    //         mActor->look(input.lookX, input.lookY, dt);

    //         if (input.jump)
    //         {
    //             mActor->jump();
    //         }

    //         if (input.sprint)
    //         {
    //             mActor->sprint();
    //         }
    //     }
    // };
}

