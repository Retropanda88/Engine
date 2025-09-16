#ifndef MIXER_H_
#define MIXER_H_

#include <types.h>

struct Sample
{
	int len;
	u8 *pcmData;
	int position;
};

const int MAX_SAMPLES = 16;

class Cmixer
{
  public:
	Cmixer();
	~Cmixer();
	bool loadSample(const char *file);
	int playChannel(int channel, struct Sample *sample, int loop);
	int isPlaying(struct Sample *sample);
	void stopChannel(int id);

  private:
	struct Sample samples[MAX_SAMPLES];

};

#endif
