class events
{
 public:
  enum class CHARACTER_EVENT
  {
    NONE = 0,
    MOVE_LEFT = 1,
    MOVE_RIGHT = 2,
    JUMP = 4,
    JUMP_LEFT = MOVE_LEFT + JUMP,
    JUMP_RIGHT = MOVE_RIGHT + JUMP,
    CROUCH = 8,
    PUNCH = 16,
    KICK = 32
  };
};
