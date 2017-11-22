## Introduction

The is an AI for the card game HearthStone.

## Demo Videos
* First demo video is on!!! https://youtu.be/z0I1nM6_k0w
* Another demo video with higher quality: https://youtu.be/L6kr_zJKCQI
* First demo video with expert warlock: https://youtu.be/wLvBlKChFW0
* Another demo video played with expert warlock innkeeper: https://youtu.be/yVX8nTo8o00

The AI beats the basic InnKeeper mage-to-mage duel so far (8 - 0).

## Motivation
* AlphaGo successfully combines MCTS and deep neural networks to beat human on Go.
* Games with hidden information are still a big challenge in many ways.
* Give it a try on Hearhstone.

## Components

### Simulation Engine
* There's no HearthStone simulation engine written in C++, so I decided to write one.
* Header-only implementation. No dependency. No need to compile anything!
* The most important goal is **speed**.
* Written in C++14/C++17.
  * Need gcc 7.0+ or Visual Studio 2017 Preview 2.1+ to compile.
  * Heavily use template programming.
* Profile-guided optimization is integrated in the build script.
* Used in AAIA'17 Data Mining Challenge: Helping AI to Play Hearthstone (https://knowledgepit.fedcsis.org/mod/page/view.php?id=1022)

### AI Engine
* Monte Carlo tree search
* Use Multiple-Observer MCTS to handle hidden information
* Share tree nodes for identical boards
* Use neural network in default policy to *greatly* boost play strength
  * An example: AI should *NOT* play *arcane missiles* in first turn.
  * If using random default policy, it takes more than 300k iterations (8G+ RAM) to realize this.
  * If using neural network as default policy, it only takes < 15k iterations (less than 5 seconds) to realize this.
  
### Neural Network

The goal of the neural network is to guess who is going to win this game. Currently, the accuracy is about 79% for expert-practice decks.

Input data:
* Minions on board
  * HP, Max-HP, Attack
  * [TODO] Taunt / Charge / Attackable
  * [TODO] Card id, enchantment ids, deathrattle ids
* [TODO] Hand cards
  * Card ids, enchantment ids
* [TODO] Deck cards
  * Card ids

### Game Board Recognition
* Use the logging feature in HearthStone
* Written in C# since no critical performance issue occurs here.
* Parse the logs to get a picture of the game board.
* Use the C# coroutine to parse the logs in a cleaner way.

### Automatic Play bot
* The automatic play bot is not implemented yet
* Just refer to the move the AI suggested, and do it manually on the game client.

## Installation
1. Install HearthStone on Windows.
2. Enable logging to HearthStone, so we can know what's the board looks like.
3. Open the C# project under 'ui' folder.
4. Run it.

## Next Step

### Neural Network

The goal of the neural network is to guess who is going to win this game, by looking at only the current board. Several improvements could be done:

1. Take history data into account: secret cards, played cards, etc.
2. Take hand cards into account.
3. Take card id into account. Currently, only HP/Max-HP/Attack are considered.
4. Take cards in deck into account.

Hope we can have a better accuracy than current result (~79%, which also aligned to the result of AAIA'17 Data Mining Challenge: Helping AI to Play Hearthstone (https://knowledgepit.fedcsis.org/mod/page/view.php?id=1022)).

We can use embedding to encode battlecry and deathrattle features for each different card IDs. TensorFlow has many great articles. Like https://www.tensorflow.org/tutorials/wide.

### Balance Between Wide and Deep

Why wide? Due to randomness, the branch factor is quite large (~4000 when drawing a card). So, there are many tree nodes in the game tree.

Why deep? The neural network by itself are not strength enough. We need to think ahead more steps to overcome the weakness in simulation.

In a naive implementation of MCTS, all the children nodes must be expanded before we use UCB formula to choose a child node and continues in selection stage. Few ideas here:
1. A fixed possibility to continue in selection stage. Even not all children are expanded.
2. A dynamic possibility based on **rest of thinking time** and **current expansion progress**.

### Share information between nodes

Even if there are only one card is different, we still need two tree nodes. Otherwise, we will fuse the strategy decision in Monte-Carlo tree search. However, this does *not* means that, we cannot share information between nodes. On the contrary, AMAF (all-move-as-first) and RAVE (rapid action value estimation) are based on this basic idea.


### Tensor Flow

Try out Tensor Flow. It's a great machine learning library, with some great visualization support.

## Contributors

Just me. Any idea/help is always welcomed.

## License

The newest GPL license is applied to this project.

Some third party libraries are used, please also obey to their licenses.
