#include "FSMTesting.h"
#include "IntentChunkedUploader.h"

typedef struct {
  // state inputs
} state_inputs;

typedef struct {
  // state vars
  String voiceCommand;
  IntentChunkedUploader uploader;
} state_vars;

bool testTransition(state startState,
                     state endState,
                     state_inputs testStateInputs, 
                     state_vars startStateVars,
                     state_vars endStateVars,
                     bool verbos);


char* s2str(state s) {
  switch(s) {
    case STATE_INIT:
    return "(1) STATE_INIT";
    case STATE_STANDBY:
    return "(2) STATE_STANDBY";
    case STATE_RECORDING:
    return "(3) STATE_RECORDING";
    case STATE_VOICE_RECOGNITION:
    return "(4) STATE_VOICE_RECOGNITION";
    case STATE_EXECUTE_COMMAND:
    return "(5) STATE_EXECUTE_COMMAND";
    case STATE_UNRECOGNIZED_COMMAND:
    return "(6) STATE_UNRECOGNIZED_COMMAND";
    case STATE_RESET:
    return "(7) STATE_RESET";
    case STATE_TRANSLATING:
    return "(8) STATE_TRANSLATING";
    default:
    return "???";
  }
}


bool testTransition(state startState,
                     state endState,
                     state_inputs testStateInputs, 
                     state_vars startStateVars,
                     state_vars endStateVars,
                     bool verbos) {
  // get start_state vars
  // need to move switch statement outside of loop() int updateFSM function
  state resultState = updateFSM(startState);
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
    sprintf(sToPrint, "Inputs: mils %ld | numButtons %d | lastButton %s", testStateInputs.mils, testStateInputs.numButtons, o2str(testStateInputs.lastButton));
    Serial.println(sToPrint);
    sprintf(sToPrint, "          %2s | %2s | %5s | %3s | %3s | %5s | %9s | %11s | %9s", "x", "y", "o", "lxb", "uxb", "level", "timeStep", "savedClock", "countdown");
    Serial.println(sToPrint);
    sprintf(sToPrint, "expected: %2d | %2d | %5s | %3d | %3d | %5d | %9d | %11d | %9d", endStateVars.x, endStateVars.y, o2str(endStateVars.o), endStateVars.lxb, endStateVars.uxb, endStateVars.level, endStateVars.timeStep, endStateVars.savedClock, endStateVars.countdown);
    Serial.println(sToPrint);
    sprintf(sToPrint, "actual:   %2d | %2d | %5s | %3d | %3d | %5d | %9d | %11d | %9d", x, y, o2str(o), lxb, uxb, level, timeStep, savedClock, countdown);
    Serial.println(sToPrint);
    return false;
  }
}

const state testStatesIn[23] = {(state) 1, (state) 2, (state) 3};

const state testStatesOut[23] = {(state) 2, (state)3 (state) 4};

const state_inputs testInputs[23] = {};

const state_vars testVarsIn[23] = {{"", NULL}, {"", NULL} {"on", NULL }, {"off", NULL}, {"calender", NULL}};

const state_vars testVarsOut[23] = {{"", NULL}, {"", new IntentChunkedUploader("")}, {"on", NULL},  {"off", NULL}, {"calendar", NULL}};

const int numTests = 23;

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