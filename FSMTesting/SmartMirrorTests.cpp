#include "FSMTesting.h"
#include "IntentChunkedUploader.h"

typedef struct {
  // state inputs
  bool recordButtonPressed;
} state_inputs;

typedef struct {
  // state vars
  String voiceCommand;
} state_vars;

bool testTransition(State startState,
                     State endState,
                     state_inputs testStateInputs, 
                     state_vars startStateVars,
                     state_vars endStateVars,
                     bool verbos);


char* s2str(State s) {
  switch(s) {
    case STATE_INIT:
    return "(0) STATE_INIT";
    case STATE_STANDBY:
    return "(1) STATE_STANDBY";
    case STATE_RECORDING:
    return "(2) STATE_RECORDING";
    case STATE_VOICE_RECOGNITION:
    return "(3) STATE_VOICE_RECOGNITION";
    case STATE_EXECUTE_COMMAND:
    return "(4) STATE_EXECUTE_COMMAND";
    case STATE_UNRECOGNIZED_COMMAND:
    return "(5) STATE_UNRECOGNIZED_COMMAND";
    case STATE_RESET:
    return "(6) STATE_RESET";
    case STATE_TRANSLATING:
    return "(7) STATE_TRANSLATING";
    default:
    return "???";
  }
}


bool testTransition(State startState,
                     State endState,
                     state_inputs testStateInputs, 
                     state_vars startStateVars,
                     state_vars endStateVars,
                     bool verbos) {
  // get start_state vars
  voiceCommand = startStateVars.voiceCommand;
  State resultState = updateFSM(startState, testStateInputs.recordButtonPressed);
  bool passedTest = (endState == resultState and voiceCommand == endStateVars.voiceCommand
                     );
  if (! verbos) {
    return passedTest;
  } else if (passedTest) {
    char sToPrint[200];
    sprintf(sToPrint, "Test from %s to %s PASSED", s2str(startState), s2str(endState));
    Serial.println(sToPrint);
    return true;
  } else {
    char sToPrint[200];
    //change strings for our variables
    Serial.println(s2str(startState));
    sprintf(sToPrint, "Test from %s to %s FAILED", s2str(startState), s2str(endState));
    Serial.println(sToPrint);
    sprintf(sToPrint, "End state expected: %s | actual: %s", s2str(endState), s2str(resultState));
    Serial.println(sToPrint);
    //sprintf(sToPrint, "Inputs: mils %ld | numButtons %d | lastButton %s", testStateInputs.mils, testStateInputs.numButtons, o2str(testStateInputs.lastButton));
    Serial.println(sToPrint);
    sprintf(sToPrint, "          %2s | %2s | %5s | %3s | %3s | %5s | %9s | %11s | %9s", "x", "y", "o", "lxb", "uxb", "level", "timeStep", "savedClock", "countdown");
    Serial.println(sToPrint);
    //sprintf(sToPrint, "expected: %2d | %2d | %5s | %3d | %3d | %5d | %9d | %11d | %9d", endStateVars.x, endStateVars.y, o2str(endStateVars.o), endStateVars.lxb, endStateVars.uxb, endStateVars.level, endStateVars.timeStep, endStateVars.savedClock, endStateVars.countdown);
    Serial.println(sToPrint);
    //sprintf(sToPrint, "actual:   %2d | %2d | %5s | %3d | %3d | %5d | %9d | %11d | %9d", x, y, o2str(o), lxb, uxb, level, timeStep, savedClock, countdown);
    Serial.println(sToPrint);
    return false;
  }
}

const State testStatesIn[23] = {(State) 2, (State) 2, (State) 3, (State) 3, (State) 3, (State) 3, (State) 4, (State) 4, (State) 5 /*(State) 4*/};

const State testStatesOut[23] = {(State) 1, (State) 3, (State) 4, (State) 4, (State) 4, (State) 5, (State) 1, (State) 1, (State) 1 /*(State) 1*/};

const state_inputs testInputs[23] = {{false}, {true}, {false}, {false}, {false}, {false}, {false}, {"false"}, {false} /*{"false"}*/};

const state_vars testVarsIn[23] = {{""}, {""}, {"on"}, {"off"}, {"calendar"}, {"unrecognized"}, {"on"}, {"off"}, {""} /*{"calendar"}*/};

const state_vars testVarsOut[23] = {{""}, {""}, {"on"}, {"off"}, {"calendar"}, {"unrecognized"}, {"on"}, {"off"}, {""} /*{"calendar"}*/};

const int numTests = 9;

bool testAllTests() {
  for (int i = 0; i < numTests; i++) {
    Serial.print("Running test ");
    Serial.println(i);
    if (!testTransition(testStatesIn[i], testStatesOut[i], testInputs[i], testVarsIn[i], testVarsOut[i], true)) {
      return false;
    }
    Serial.println();
  }
  Serial.println("All tests passed!");
  return true;
}