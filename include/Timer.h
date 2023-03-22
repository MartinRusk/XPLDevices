#ifndef SoftTimer_h
#define SoftTimer_h
#include <Arduino.h>

/// @brief Priovide a simple software driven timer for general purpose use
class Timer
{
  public: 
    /// @brief Setup timer
    /// @param cycle Cycle time for elapsing timer in ms. 0 means no cycle, just for measurement.
    Timer(float cycle = 0); // ms

    /// @brief Set or reset cycle time
    /// @param cycle Cycle time in ms
    void setCycle(float cycle); 

    /// @brief Check if cyclic timer elapsed and reset if so
    /// @return true: timer elapsed and restarted, false: still running
    bool elapsed();

    /// @brief Get measured time since and reset timer
    /// @return Elapsed time in ms
    float getTime(); // ms

    /// @brief Return cycle counter and reset to zero
    /// @return Number of calls to elapsed() since last call of count()
    long count();
  private:
    unsigned long _cycleTime;
    unsigned long _lastUpdateTime;
    long _count;
};

#endif
