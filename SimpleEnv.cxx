/*
 * Author: Adam White
 * Purpose: for use of Rienforcement learning course Indiana University Spring 2016
 *
 * env transitions *ignore* actions, state transitions, rewards, and terminations are all random
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include "rlglue/Environment_common.h" /* Required for RL-Glue interface*/
#include "utils.h"
#include <ctime>

/*** IMPORTANT: 
 * Any private variables to the environment must be declared static
 * Otherwise they are default public global variables that can be 
 * accessed in other files 
 ***/

static gsl_vector* local_observation;
static reward_observation_terminal_t this_reward_observation;
static const int nStatesSimpleEnv = 20;

static double start;
static double goal;

static int walls[7] = {14,20,29,34,38,43,52};
//0-up,1-left,2-down,3-right
static int moves[4] = {9,-1,-9,1};

void env_init() 
{
	local_observation = gsl_vector_calloc(1);
	
	goal = 53;
	start = 27;

	srand(time(0));


	this_reward_observation.observation=local_observation;
	this_reward_observation.reward=0;
	this_reward_observation.terminal=0;
	
	return;
}

const observation_t* env_start()
{
  gsl_vector_set(local_observation,0,start);
  return this_reward_observation.observation;
}

const int findState(int action){
	int lastState = gsl_vector_get(local_observation,0);
	int expected = gsl_vector_get(local_observation,0) + moves[action];

	if(action == 0 && expected > 53){
		expected = expected - 9;
	}
	if(action == 1 && lastState%9 == 0){
		expected = expected + 1;
	}
	if(action == 2 && expected < 0){
		expected = expected + 9;
	}
	if(action == 3 && lastState%9 == 8){
		expected = expected - 1;
	}

	for(int i=0;i<7;i++){
		if(expected==walls[i]){
			expected = expected - moves[action];
		}
	}


	return expected;
}

const reward_observation_terminal_t *env_step(const action_t *this_action)
{
  int episode_over=0;
  double the_reward;
  
  int atp1 = gsl_vector_get(this_action,0); /* how to extact action */
  int stp1 = findState(atp1); /* state transitions are uniform random */

  if(stp1 == goal){
	  the_reward=1;
	  episode_over = 1;
  }else{
	  the_reward=0;
  }

  gsl_vector_set(local_observation, 0,stp1);
  this_reward_observation.reward = the_reward;
  this_reward_observation.terminal = episode_over;
  
  return &this_reward_observation;
}

void env_cleanup()
{
  gsl_vector_free(local_observation);
}

const char* env_message(const char* inMessage) 
{
  if(strcmp(inMessage,"what is your name?")==0)
  return "my name is skeleton_environment!";
  
  /* else */
  return "I don't know how to respond to your message";
}
