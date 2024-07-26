// SPDX-License-Identifier: GPL-3.0-only

#ifndef BALLTZE_API__EVENTS__NETGAME_MESSAGE_HPP
#define BALLTZE_API__EVENTS__NETGAME_MESSAGE_HPP

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
}

#endif
