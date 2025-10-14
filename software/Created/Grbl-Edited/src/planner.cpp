#include "planner.h"

//planner ringbuffer
planner_t planner;
float currentPosition[N_AXIS] = {0,0,0};



/**
 * Plans a line and puts it in the planner ring buffer.
 * @param targetPow float targetPos[N_AXIS] -> target position for each axis on the machine.
 * @param feedrate float feedrate -> the feedrate (nominalspeed) for each block.
 */
void planBufferLine(float targetPos[N_AXIS], float feedrate) {
  planner_block_t *block = &planner.block[planner.head];

  memcpy(block->p0, currentPosition, sizeof(currentPosition));
  memcpy(block->p1, targetPos, sizeof(currentPosition));

  for(int i = 0; i < N_AXIS; i++) {
    block->p1[i] = targetPos[i];
  }
  block->nominal_speed = feedrate;
  
  float dx = targetPos[0] - currentPos[0];
  float dy = targetPos[1] - currentPos[1];
  float dz = targetPos[2] - currentPos[2];
  block->distance = sqrtf(dx*dx + dy*dy + dz*dz);
  block->accel = SYSTEM_MAX_ACCEL;
  vecSub3(block->dir, block->p0, block->p1);
  vecNormalize3(block->dir);



  planner.head = (planner.head + 1) % PLANNER_BUFFER_SIZE;
  if (planner.head == planner.tail) return; //buffer overflow.  


  plannerRecalculate(0.001f);

}

/**
 * Recalculates the junction speed for every planned block in the 'planner' ringbuffer to ensure the machine is running on maximum speed for each segment without losing accuracy.
 * @param J float J -> dividation that can happen in the corner smaller = more accurate.
 */
void plannerRecalculate(float J) {

  if(planner.head == planner.tail) {
    planner.block[planner.head].exit_speed = 0.0f;
    return;
  }


  // set all speeds to nominal
  int idx = planner.tail;
  while(idx != planner.head) {
    planner.block[idx].entry_speed = planner.block[idx].nominal_speed;
    planner.block[idx].entry_speed = planner.block[idx].nominal_speed;
    idx = (idx + 1) % PLANNER_BUFFER_SIZE;
  } 

  // forward checking
  idx = planner.tail;
  while(idx != planner.head) {
    int prev = (idx - 1 + PLANNER_BUFFER_SIZE) % PLANNER_BUFFER_SIZE;
    if(prev != idx) {
      float maxJunctionSpeed = junctionSpeedFromDeviation(planner.block[prev].dir, planner.block[idx].dir, J, fminf(planner.block[prev].accel, planner.block[idx].accel));

      if(planner.block[idx].entry_speed > maxJunctionSpeed) {
        planner.block[idx].entry_speed = maxJunctionSpeed;
      }
      if(planner.block[prev].exit_speed > maxJunctionSpeed) {
        planner.block[prev].exit_speed = maxJunctionSpeed;
      } 
      idx = (idx + 1) % PLANNER_BUFFER_SIZE;                
    }
  }

  // backwards checking
  bool changes = true;
  int iter = 0;
  while (changes && iter < 4) {
    changes = false;
    int i = (planner.head - 1 + PLANNER_BUFFER_SIZE) % PLANNER_BUFFER_SIZE;

    while (true) {
      int prev = (i - 1 + PLANNER_BUFFER_SIZE) % PLANNER_BUFFER_SIZE;
      if(i == planner.tail) {
        break;
      }

      planner_block_t *B = &planner.block[i];
      planner_block_t *P = &planner.block[prev];

      float max_entry_from_prev = sqrtf(fmaxf(0.0f, P->entry_speed * P->entry_speed + 2.0f * P->accel * P->distance));

      if(B->entry_speed > max_entry_from_prev) {
        B->entry_speed = max_entry_from_prev;
        changes = true;
      }
      i = prev;
    }
    iter++;
  }

  // set all exit and entry the same when they follow eachother & exit block speed 0
  idx = planner.tail;
  while(idx != planner.head) {
    int next = (idx + 1) % PLANNER_BUFFER_SIZE;
    if(next != planner.head) {
      planner.block[idx].exit_speed = planner.block[next].entry_speed;
    } else {
      planner.block[idx].exit_speed = 0.0f;
    }
    idx = (idx + 1) % PLANNER_BUFFER_SIZE;
  }
}




/**
 * calculates the maximum junctionspeed from deviation with unit vectors.
 * 
 * @param u const float u[3] -> unit vector prev block.
 * @param v const float v[3] -> unit vector current block.
 * @param J float J -> dividation that can happen in the corner smaller = more accurate.
 * @param a float a -> maxmimal system acceleration.
 * 
 * @return float - junction speed.
 */
float junctionSpeedFromDeviation(const float u[3], const float v[3], float J, float a) {
  float cos_theta = vecDot3(u, v);

  if(cos_theta > EPS_COS_STRAIGHT) {
    return __FLT_MAX__;
  }


  float numerator = a * J * (1.0f + cos_theta);
  float denom = (1.0f - cos_theta);
  if(denom <= 0.0f) {
    return 0.0f;
  }

  float vj_sq = numerator / denom;
  if(vj_sq <= 0.0f) {
    return 0.0f;
  }

  return sqrtf(vj_sq);
}




/**
 * Vector function for function: junctionSpeedFromDeviation. 
 * 
 * @param a const float a[3] -> vector of floats 
 * @param b const float b[3] -> vector of floats
 * @return angle between vectors
 */
static inline float vecDot3(const float a[3], const float b[3]) {
  return a[0]*b[0] + a[1]*b[1] + a[2]*b[2];
}


/**
 * Vector function for function: planBufferLine. Gives the displacement.
 *  
 * @param r float r[3] -> refrence to return vector
 * @param a float a[3] -> vector of floats
 * @param b float b[3] -> vector of floats
 */
static inline void vecSub3(float r[3], const float a[3], const float b[3]) {
  r[0] = a[0]-b[0]; r[1] = a[1]-b[1]; r[2] = a[2]-b[2];
}

/**
 * Vector function for function: vecNormalize3. It calculates the length of the vector
 * 
 * @param a const float a[3] -> vector of floats to get length of.
 * @return float - lenght of vector
 */
static inline float vecNorm3(const float a[3]) {
  return sqrtf(a[0]*a[0] + a[1]*a[1] + a[2]*a[2]);
}

/**
 * Vector function for function: planBufferLine. It calculates the unit vector.
 * 
 * @param r float r[3] -> refrence to vector to change.
 */
static inline void vecNormalize3(float r[3]) {
  float n = vecNorm3(r);
  if (n > 0.0f) { r[0]/=n; r[1]/=n; r[2]/=n; }
}


#ifdef ENTRYBLOCKSPEED
/**
 * @note NOT NEEDED
 * computes the max entry and exit speed between blocks.
 * @param i index in buffer - int
 * @param J mm junction diviation - float
 */
void computeBlockEntrySpeed(int i, float J) {
  if(i == planner.tail) return;
  int prev = (i - 1 + PLANNER_BUFFER_SIZE) % PLANNER_BUFFER_SIZE;
  int next = i;

  planner_block_t *prev_block = &planner.block[prev];
  planner_block_t *next_block = &planner.block[next];

  if(prev_block->distance <= 0.0f || next_block->distance <= 0.0f) {
    next_block->entry_speed = 0.0f;
    return;
  }

  float accel = fminf(prev_block->accel, next_block->accel);

  float maxJunctionSpeed = junction_speed_from_deviation(prev_block->dir, next_block->dir, J, accel);

  float allowed = fminf(fminf(prev_block->nominal_speed, next_block->nominal_speed), maxJunctionSpeed);

  float prev_exit_max = sqrtf(fmaxf(0.0f, prev_block->nominal_speed * next_block->nominal_speed + 2.0f * prev_block->accel * prev_block->distance));

  if(allowed > prev_exit_max) allowed = prev_exit_max;

  next_block->entry_speed = allowed;
  prev_block->exit_speed = allowed;
}
#endif