#pragma once
#include <Arduino.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/i2s.h"
#define SAMPLE_RATE (44100)

class I2SRecord
{
public:
  bool InitInput(i2s_bits_per_sample_t BPS,
                 int bckPin,
                 int wsPin,
                 int dataInPin);

  size_t Read(char* data, int numData);

  size_t Write(char* data, int numData);

  void End();
};