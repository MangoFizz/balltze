// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__NETGAME_HPP
#define BALLTZE_API__EVENTS__NETGAME_HPP

#include <cstdint>
#include <string>
#include "../engine/netgame.hpp"
#include "../event.hpp"

namespace Balltze::Event {
    using NetworkGameChatMessage = Engine::NetworkGameMessages::HudChat;

    struct NetworkGameChatMessageEventContext {
        NetworkGameChatMessage const *chat_message;

        NetworkGameChatMessageEventContext(NetworkGameChatMessage *msg) : chat_message(msg) {}
    };

    class NetworkGameChatMessageEvent : public EventData<NetworkGameChatMessageEvent> {
    public:
        NetworkGameChatMessageEventContext context;

        bool cancellable() const {
            return true;
        }

        NetworkGameChatMessageEvent(EventTime time, const NetworkGameChatMessageEventContext &context) : EventData(time), context(context) {}
    };

    struct NetworkGameMultiplayerSoundEventContext {
        Engine::NetworkGameMultiplayerSound sound;
    };

    class NetworkGameMultiplayerSoundEvent : public EventData<NetworkGameMultiplayerSoundEvent> {
    public:
        NetworkGameMultiplayerSoundEventContext context;

        bool cancellable() const {
            return true;
        }

        NetworkGameMultiplayerSoundEvent(EventTime time, const NetworkGameMultiplayerSoundEventContext &context) : EventData(time), context(context) {}
    };

    struct NetworkGameHudMessageEventContext {
        Engine::NetworkGameMultiplayerHudMessage message_type;
        Engine::PlayerHandle causer;
        Engine::PlayerHandle victim;
        Engine::PlayerHandle local_player;
    };

    class NetworkGameHudMessageEvent : public EventData<NetworkGameHudMessageEvent> {
    public:
        NetworkGameHudMessageEventContext context;

        bool cancellable() const {
            return true;
        }

        NetworkGameHudMessageEvent(EventTime time, const NetworkGameHudMessageEventContext &context) : EventData(time), context(context) {}
    };
}

#endif
