#ifndef TIMER_HPP_
#define TIMER_HPP_

/**
 * File     :  timer.hpp
 * --------------
 *
 * Author   : Brandon L. Barker
 *   Inspiration from https://gist.github.com/mcleary/b0bf4fa88830ff7c882d
 * Purpose  : Simple timer class
 **/

#include <chrono>

class Timer {
 public:
  void start( ) {
    chrono_start = std::chrono::steady_clock::now( );
    running  = true;
  }
  void stop( ) {
    chrono_end  = std::chrono::steady_clock::now( );
    running = false;
  }

  double elapsedNanoseconds( ) {
    std::chrono::time_point<std::chrono::steady_clock> endTime;

    // if we haven't stopped the timer, use current time
    if ( running ) {
      endTime = std::chrono::steady_clock::now( );
    } else {
      endTime = chrono_end;
    }

    return std::chrono::duration_cast<std::chrono::nanoseconds>( endTime -
                                                                  chrono_start )
        .count( );
  }


  /* support nanoseconds up to hours (days+ requires C++20) */
  double elapsedMicroseconds( ) { return elapsedNanoseconds( ) / 1.0e3; }
  double elapsedMilliseconds( ) { return elapsedNanoseconds( ) / 1.0e6; }
  double elapsedSeconds( ) { return elapsedNanoseconds( ) / 1.0e9; }
  double elapsedMinutes( ) { return elapsedSeconds( ) / 60.0; }
  double elapsedHours( ) { return elapsedMinutes( ) / 60.0; }

 private:
  std::chrono::time_point<std::chrono::steady_clock> chrono_start;
  std::chrono::time_point<std::chrono::steady_clock> chrono_end;
  bool running = false;
};

#endif // TIMER_HPP_
