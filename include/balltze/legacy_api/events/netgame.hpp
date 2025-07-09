// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_LEGACY_API__EVENTS__NETGAME_HPP
#define BALLTZE_LEGACY_API__EVENTS__NETGAME_HPP

#include <cstdint>
#include <string>
#include "../engine/netgame.hpp"
#include "../event.hpp"

namespace Balltze::LegacyApi::Event {
    using NetworkGameChatMessage = LegacyApi::Engine::NetworkGameMessages::HudChat;

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
        LegacyApi::Engine::NetworkGameMultiplayerSound sound;
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
        LegacyApi::Engine::NetworkGameMultiplayerHudMessage message_type;
        LegacyApi::Engine::PlayerHandle causer;
        LegacyApi::Engine::PlayerHandle victim;
        LegacyApi::Engine::PlayerHandle local_player;
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
