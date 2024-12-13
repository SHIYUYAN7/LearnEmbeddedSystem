// FSM States

#include <Arduino.h>

typedef enum State {
  STATE_INIT,
  STATE_STANDBY,
  STATE_RECORDING,
  STATE_VOICE_RECOGNITION,
  STATE_EXECUTE_COMMAND,
  STATE_UNRECOGNIZED_COMMAND,
  STATE_RESET,
  STATE_TRANSLATING
};

State updateFSM(State current, bool buttonPressed);

extern String voiceCommand;
extern volatile bool recordButtonPressed;