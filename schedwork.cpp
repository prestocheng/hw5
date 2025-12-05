#ifndef RECCHECK
#include <set>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
// add or remove necessary headers as you please

#endif

#include "schedwork.h"

using namespace std;

// a constant that can be used to indicate an INVALID 
// worker ID if that is useful to your implementation.
// Feel free to not use or delete.
static const Worker_T INVALID_ID = (unsigned int)-1;


// Add prototypes for any helper functions here

bool scheduleWorker(
  size_t day,
  const AvailabilityMatrix& avail,
  const size_t dailyNeed,
   const size_t maxShifts,
   DailySchedule& sched,
   size_t pos,
   size_t numScheduled,
   vector<size_t>& daysScheduled
);


// Add your implementation of schedule() and other helper functions here

bool schedule(
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched
)
{
    if(avail.size() == 0U){
        return false;
    }
    sched.clear();
    
    vector<size_t> daysScheduled;
    for (size_t i = 0; i < avail[0].size(); i++) {
      daysScheduled.push_back(0);
    }

    for (size_t j = 0; j < avail.size(); j++) {
      vector<Worker_T> newDay;
      sched.push_back(newDay);
    }
    return scheduleWorker(0, avail, dailyNeed, maxShifts, sched, 0, 0, daysScheduled);
}



bool scheduleWorker(
    size_t day,
    const AvailabilityMatrix& avail,
    const size_t dailyNeed,
    const size_t maxShifts,
    DailySchedule& sched,
    size_t pos,
    size_t numScheduled,
    vector<size_t>& daysScheduled
)
{

    // Base case: reached end of schedule
    if (day == avail.size()) {
      return true;
    }


    size_t numWorkers = avail[0].size();

    // Schedule workers based on availability
    for (size_t i = pos; i < numWorkers; i++) {
      if (avail[day][i] == true && daysScheduled[i] < maxShifts) {
        sched[day].push_back(i);
        daysScheduled[i] += 1;
        numScheduled += 1;

        // Case 1: Find next worker for same day
        if (numScheduled < dailyNeed) {
          if (scheduleWorker(day, avail, dailyNeed, maxShifts, sched, i+1, numScheduled, daysScheduled)) {
            return true;
          }
          else { // Backtrack if solution doesn't work
            numScheduled -= 1;
            daysScheduled[i] -= 1;
            sched[day].pop_back();
          }
        }

        // Case 2: Enough workers, move onto next day
        else if (numScheduled == dailyNeed) {
          // Pass in pos and numscheduled as 0, new day
          if (scheduleWorker(day + 1, avail, dailyNeed, maxShifts, sched, 0, 0, daysScheduled)) {
            return true;
          }
          else { // Backtrack if solution doesn't work
            numScheduled -= 1;
            daysScheduled[i] -= 1;
            sched[day].pop_back();
          }
        }
      }
    }

    return false;
}

