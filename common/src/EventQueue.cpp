#include "idk/engine-message/EventQueue.hpp"

idk::core::Queue<idk::EngineEvent, 128> idk::EngineEvent::gEngineEventQueue;
