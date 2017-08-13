#pragma once

#include <assert.h>
#include <array>
#include "state/Types.h"

namespace state
{
	namespace detail { template <CardType TargetCardType, CardZone TargetCardZone> struct PlayerDataStructureMaintainer; }

	namespace board
	{
		class Hand
		{
			template <CardType TargetCardType, CardZone TargetCardZone> friend struct state::detail::PlayerDataStructureMaintainer;

		public:
			static constexpr size_t max_cards_ = 10;
			Hand() : cards_(), size_(0), change_id_(0) {}

			void FillWithBase(Hand const& base) {
				cards_ = base.cards_; // TODO: copy on write?
				size_ = base.size_;
				change_id_ = base.change_id_;
			}

			size_t Size() const { return size_; }
			bool Empty() const { return size_ == 0; }
			bool Full() const { return size_ >= max_cards_; }
			size_t LeftSpaces() const {
				assert(size_ <= max_cards_);
				return max_cards_ - size_;
			}

			CardRef Get(size_t idx) const { return cards_[idx]; }
			int GetChangeId() const { return change_id_; }

			template <typename Functor>
			void ForEach(Functor&& functor) const {
				for (size_t i = 0; i < size_; ++i) {
					if (!functor(cards_[i])) return;
				}
			}

		private:
			size_t PushBack(CardRef ref)
			{
				assert(size_ < max_cards_);
				size_t ret = size_;
				cards_[size_] = ref;
				++size_;
				++change_id_;
				return ret;
			}

			template <typename AdjustFunctor>
			void Remove(size_t pos, AdjustFunctor&& functor)
			{
				assert(pos < size_);

				++change_id_;
				size_t spot = pos;
				for (size_t i = pos + 1; i < size_; ++i) {
					cards_[spot] = cards_[i];
					functor(cards_[spot], spot);
					++spot;
				}
				--size_;
			}

		private:
			std::array<CardRef, max_cards_> cards_;
			size_t size_;
			int change_id_;
		};
	}
}