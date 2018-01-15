#include <stdio.h>
#include <math.h>
#include <string.h>
#include "rlglue/RL_glue.h" /* Required for RL-Glue  */


void saveResults(double* data, int dataSize, const char* filename);

int main(int argc, char *argv[]) {
  int k,i;
  int numEpisodes = 200;
  int maxStepsInEpisodes = 100;
  int numRuns = 30;
  double result[numEpisodes];
  memset(result, 0, sizeof(result));

  double res[50];
  double counts[7];
  memset(res, 0, sizeof(res));
  memset(counts, 0, sizeof(counts));
  for(int j=0;j<30;j++){
	  RL_init();
	  for(int i=0;i<50;i++){
		  RL_episode(0);
		  res[i] += RL_num_steps();
  	  }
	  RL_cleanup();
  }
  double sum = 0;
  for(int i=0;i<50;i++){
	  res[i] = res[i]/30;
	  sum +=res[i];
  }

  saveResults(res, 50, "overall.dat");

//  second question
    memset(res, 0, sizeof(res));
    for(int j=0;j<30;j++){
      RL_init();
	  RL_agent_message("0");
  	  for(int i=0;i<50;i++){
  		  RL_episode(2000);
  		  res[i] += RL_num_steps();
    	  }
  	  RL_cleanup();
    }
    sum = 0;
    for(int i=1;i<50;i++){
  	  res[i] = res[i]/30;
  	  sum +=res[i];
    }
    counts[0] = sum;

    //one more
    memset(res, 0, sizeof(res));
    for(int j=0;j<30;j++){
	  RL_init();
	  RL_agent_message("1");
	  for(int i=0;i<50;i++){
		  RL_episode(2000);
		  res[i] += RL_num_steps();
	  }
	  RL_cleanup();
    }
    sum = 0;
    for(int i=1;i<50;i++){
	  res[i] = res[i]/30;
	  sum +=res[i];
    }
    counts[1] = sum;

	//one more
    memset(res, 0, sizeof(res));
	for(int j=0;j<30;j++){
		RL_init();
		RL_agent_message("2");
	for(int i=0;i<50;i++){
		  RL_episode(2000);
		  res[i] += RL_num_steps();
		  }
	  RL_cleanup();
	}
	sum = 0;
	for(int i=1;i<50;i++){
	  res[i] = res[i]/30;
	  sum +=res[i];
	}
	counts[2] = sum;


	//one more
	memset(res, 0, sizeof(res));
    for(int j=0;j<30;j++){
	  RL_init();
	  RL_agent_message("3");
	  for(int i=0;i<50;i++){
		  RL_episode(2000);
		  res[i] += RL_num_steps();
	  }
	  RL_cleanup();
    }
    sum = 0;
    for(int i=1;i<50;i++){
	  res[i] = res[i]/30;
	  sum +=res[i];
	}
	counts[3] = sum;

	//one more
	memset(res, 0, sizeof(res));
	for(int j=0;j<30;j++){
	  RL_init();
	  RL_agent_message("4");
	  for(int i=0;i<50;i++){
		  RL_episode(2000);
		  res[i] += RL_num_steps();
	  }
	  RL_cleanup();
	}
	sum = 0;
	for(int i=1;i<50;i++){
	  res[i] = res[i]/30;
	  sum +=res[i];
	}
	counts[4] = sum;


	//one more
	memset(res, 0, sizeof(res));

	for(int j=0;j<30;j++){
	  RL_init();
	  RL_agent_message("5");
	  for(int i=0;i<50;i++){
		  RL_episode(2000);
		  res[i] += RL_num_steps();
	  }
	  RL_cleanup();
	}
	sum = 0;
	for(int i=1;i<50;i++){
	  res[i] = res[i]/30;
	  sum +=res[i];
	}
	counts[5] = sum;

      //last one
    memset(res, 0, sizeof(res));
	for(int j=0;j<30;j++){
		RL_init();
		RL_agent_message("6");
		for(int i=0;i<50;i++){
		  RL_episode(2000);
		  res[i] += RL_num_steps();
		  }
      	  RL_cleanup();
	}
	sum = 0;
	for(int i=1;i<50;i++){
	  res[i] = res[i]/30;
	  sum +=res[i];
	}
	counts[6] = sum;

	saveResults(counts, 7, "sums.dat");


  return 0;
}

void saveResults(double* data, int dataSize, const char* filename) {
  FILE *dataFile;
  int i;
  dataFile = fopen(filename, "w");
  for(i = 0; i < dataSize; i++){
    fprintf(dataFile, "%lf\n", data[i]);
  }
  fclose(dataFile);
}
