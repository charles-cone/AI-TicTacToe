# tictactoes

A tictactoe AI written in C++ using a min-max algorithm with an alpha-beta pruned game tree. The whole game tree fits in about 1/2 MB of RAM and it *should* be impossible to win.

__Possible Improvements:__
  - Currently the AI can only generate a tree assuming the AI moves first
  - The easy_timer submodule needs to be removed, as it was just for testing purposes
  - Currently has a lot of allocations and deallocations, objects could be recycled
    
