#pragma once

#include "state/detail/InvokeCallback.h"
#include "FlowControl/Manipulators/CardManipulator.h"

namespace state {
	namespace detail {
		template <CardType CardType>
		inline void InvokeCallback<CardType, kCardZonePlay>::Added(
			FlowControl::Manipulate & manipulate, state::Events::Manager & event_mgr, state::CardRef card_ref, state::Cards::Card & card)
		{
			manipulate.Card(card_ref, card).AfterAddedToPlayZone(event_mgr);
		}
	}
}