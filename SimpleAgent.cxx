/*
 * Author: Adam White, Matthew Schlegel
 * Purpose: for use of Rienforcement learning course Indiana University Spring 2016
 *
 * agent does *no* learning, selects actions randomly from the set of legal actions, ignores observation/state
 *
 */

#include "utils.h"
#include <stdio.h>
#include <string.h>
#include <time.h>


#include "rlglue/Agent_common.h" /* Required for RL-Glue */

/*** IMPORTANT: 
 * Any private variables to the environment must be declared static
 * Otherwise they are default public global variables that can be 
 * accessed in other files 
 ***/

static gsl_vector* local_action;
static action_t* this_action;
static gsl_vector* last_observation;

static int numActions = 1;
static int numStates = 1;

static int lastAction;
static int lastState;

static double alpha = 0.5;
static double lambda = 0.95;
static int planningSteps = 5;
//0.04 was right
static double theta = 0.04;

static double PAlpha[8] = {0.0025,0.05,0.1,0.2,0.4,0.5,0.8};

struct model{
	double reward;
	int nextState;
};

struct exper{
	int state;
	int action;
};

static exper experience[300];
static int expLen;

struct back{
	int state[200];
	int action[200];
	int len;
};

static back backtrack[60];

struct Que{
	int state;
	int action;
	double priority;
};

static model M[60][4];
static double Q[60][4];

static Que PQue[100];
static int Psize = 0;

void addQ(int state,int action,double p){
	int ins = 0;
	for(int i=0;i<Psize;i++){
		if(p > PQue[i].priority){
			ins++;
		}else{
			break;
		}
	}
	for(int i=Psize;i>=ins;i--){
		PQue[i+1] = PQue[i];
	}
	PQue[ins].state = state;
	PQue[ins].action = action;
	PQue[ins].priority=p;
	Psize++;
}

void agent_init()
{
  //Allocate Memory
  for(int i=0;i<60;i++){
	  for(int j=0;j<4;j++){
		  M[i][j].nextState=0;
		  M[i][j].reward=0;
		  Q[i][j] = 0;
		  backtrack[i].len=0;
	  }
  }

  expLen = 0;
  Psize = 0;


  local_action = gsl_vector_calloc(1);
  this_action = local_action;
  last_observation = gsl_vector_calloc(1);
}

const int findAction(int state){
	int max = 0;
	int equal = 0;

	for(int i=1;i<4;i++){
		if(Q[state][max] < Q[state][i]){
			max = i;
		}else if(Q[state][max] == Q[state][i]){
			if(rand()%2 == 0){
				max = i;
			}
		}
	}

	if(rand()%10 < 9){
		return max;
	}else{
		return rand()%4;
	}
}


const action_t *agent_start(const observation_t *this_observation) {

  expLen = 0;
  
  for(int i=0;i<60;i++){
	  backtrack[i].len=0;
    }

  int stp1 = (int)gsl_vector_get(this_observation,0); /* how you convert observation to a int, if state is tabular */
  int atp1=findAction(stp1);
  
  Psize = 0;
  lastAction = atp1;
  lastState = stp1;

  gsl_vector_set(local_action,0,atp1);

  gsl_vector_memcpy(last_observation,this_observation);/*save observation, might be useful on the next step */
  
  return this_action;
}

const action_t *agent_step(double reward, const observation_t *this_observation) {
  
  int stp1 = (int)gsl_vector_get(this_observation,0);
  int atp1=findAction(stp1);
  int S = lastState;
  double p;

  M[lastState][lastAction].nextState=stp1;
  M[lastState][lastAction].reward=reward;

  experience[expLen].state=lastState;
  experience[expLen].action=lastAction;
  expLen++;

  for(int i=0;i<expLen;i++){
	  int l = backtrack[stp1].len;
	  int flag=0;
	  for(int j=0;j<l;j++){
		if(experience[i].state == backtrack[stp1].state[j] || experience[i].state == stp1){
			flag = 1;
		}
	  }
	  if(flag == 0){
		  backtrack[stp1].state[l]=experience[i].state;
		  backtrack[stp1].len += 1;
	  }
  }

  double maxQ = Q[stp1][0];
  
  for(int i=1;i<4;i++){
	  if(Q[stp1][i] > maxQ){
		  maxQ = Q[stp1][i];
	  }
  }

  p = fabs(reward + lambda*maxQ - Q[lastState][lastAction]);

  if(p>theta){
	  addQ(lastState, lastAction, p);
  }

  int n = planningSteps;
  while(n > 0 && Psize>0){
	  int S = PQue[Psize].state;
	  int A = PQue[Psize].action;
	  Psize--;

	  int ns = M[S][A].nextState;
	  double r = M[S][A].reward;

	  maxQ = Q[ns][0];
	  for(int i=1;i<4;i++){
		  if(Q[ns][i] > maxQ){
	  		  maxQ = Q[ns][i];
	  	  }
	  }
	  Q[S][A] = Q[S][A] + alpha*(r + lambda*maxQ - Q[S][A]);

	  int l = backtrack[S].len;
	  for(int i=0;i<l;i++){
			  int predS = backtrack[S].state[i];
			  int predA = backtrack[S].action[i];

			  r = M[predS][predA].reward;

			  p = fabs(r + lambda*maxQ - Q[predS][predA]);

			  if(p>theta){
			  	  addQ(predS, predA, p);
			  }
		  }
	  n--;
}


  //sarsa
//	  Q[lastState][lastAction] = Q[lastState][lastAction] + 0.1*(reward + Q[stp1][atp1] - Q[lastState][lastAction]);

  lastAction = atp1;
  lastState = stp1;


  gsl_vector_set(local_action,0,atp1);
  gsl_vector_memcpy(last_observation,this_observation);
  
  return this_action;
}

void agent_end(double reward) {

	int S = lastState;
	double p;

	M[lastState][lastAction].nextState=-1;
	M[lastState][lastAction].reward=reward;

	p = fabs(reward - Q[lastState][lastAction]);

	if(p>theta){
	  addQ(lastState, lastAction, p);
	}

	  int n = planningSteps;
	  while(n > 0 && Psize>0){
		  int S = PQue[Psize].state;
		  int A = PQue[Psize].action;
		  Psize--;

		  int ns = M[S][A].nextState;
		  double r = M[S][A].reward;

		  double maxQ;

		  if(ns==-1){
			  maxQ=0;
		  }else{
			  maxQ = Q[ns][0];
			  for(int i=1;i<4;i++){
				  if(Q[ns][i] > maxQ){
					  maxQ = Q[ns][i];
				  }
			  }
		  }
		  Q[S][A] = Q[S][A] + alpha*(r + lambda*maxQ - Q[S][A]);

		  int l = backtrack[S].len;
		  for(int i=0;i<l;i++){
				  int predS = backtrack[S].state[i];
				  int predA = backtrack[S].action[i];

				  r = M[predS][predA].reward;

				  p = fabs(r + lambda*maxQ - Q[predS][predA]);

				  if(p>theta){
					  addQ(predS, predA, p);
				  }
			  }

		  n--;
	 }

}

void agent_cleanup() {
  /* clean up mememory */
  gsl_vector_free(local_action);
  gsl_vector_free(last_observation);
}

const char* agent_message(const char* inMessage) {
  /* might be useful to get information from the agent */
	  if(strcmp(inMessage,"0")==0){
		  alpha=PAlpha[0];
	  }
	  if(strcmp(inMessage,"1")==0){
		  alpha=PAlpha[1];
	  }
	  if(strcmp(inMessage,"2")==0){
		  alpha=PAlpha[2];
	  }
	  if(strcmp(inMessage,"3")==0){
		  alpha=PAlpha[3];
	  }
	  if(strcmp(inMessage,"4")==0){
		  alpha=PAlpha[4];
	  }
	  if(strcmp(inMessage,"5")==0){
		  alpha=PAlpha[5];
	  }
	  if(strcmp(inMessage,"6")==0){
		  alpha=PAlpha[6];
	  }
  if(strcmp(inMessage,"what is your name?")==0)
  return "my name is skeleton_agent!";
  
  /* else */
  return "I don't know how to respond to your message";
}
