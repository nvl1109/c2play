/*
*
* Copyright (C) 2016 OtherCrashOverride@users.noreply.github.com.
* All rights reserved.
*
* This program is free software; you can redistribute it and/or modify
* it under the terms of the GNU General Public License version 2, as 
* published by the Free Software Foundation.
*
* This program is distributed in the hope that it will be useful, but WITHOUT
* ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
* FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License for
* more details.
*
*/

#include <stdio.h>

#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <vector>
#include <alsa/asoundlib.h>
#include <string> 
#include <queue>
#include <pthread.h>
#include <signal.h>
#include <unistd.h>
#include <getopt.h>

extern "C"
{
#include <libavformat/avformat.h>
#include <libavcodec/avcodec.h>
}

//#include "InputDevice.h"
#include "MediaPlayer.h"

#ifdef X11
#include "X11Window.h"
#else
#include "FbdevAmlWindow.h"
#endif

#include <dirent.h>
#include <sys/stat.h>
#include <string>
//#include <linux/input.h>
//#include <linux/uinput.h>

#include <linux/kd.h>
#include <linux/fb.h>

//#include "Osd.h"
#include "Compositor.h"

#include "gpio/gpioc2.h"

volatile bool isRunning = true;

// Signal handler for Ctrl-C
void SignalHandler(int s)
{
	isRunning = false;
}

struct option longopts[] = {
	{ "time",			required_argument,  NULL,          't' },
	{ "chapter",		required_argument,  NULL,          'c' },
	{ "video",			required_argument,  NULL,          'v' },
	{ "audio",			required_argument,  NULL,          'a' },
	{ "subtitle",		required_argument,  NULL,          's' },
	{ "avdict",			required_argument,  NULL,          'A' },
	{ "loop",			no_argument,        NULL,          'l' },
	{ "gpio",			no_argument,        NULL,          'g' },
	{ 0, 0, 0, 0 }
};

int main(int argc, char** argv)
{
	if (argc < 1)
	{
		exit(0);
	}

	// Trap signal to clean up
	signal(SIGINT, SignalHandler);


	// options
	int c;
	double optionStartPosition = 0;
	int optionChapter = -1;
	int optionVideoIndex = 0;
	int optionAudioIndex = 0;
	int optionSubtitleIndex = -1;	//disabled by default
	std::string avOptions;
	bool optionLoop = false;
	bool gpioTrigger = false;

	while ((c = getopt_long(argc, argv, "t:c:", longopts, NULL)) != -1)
	{
		switch (c)
		{
			case 'A':
				avOptions = optarg;
				break;

			case 't':
			{
				if (strchr(optarg, ':'))
				{
					unsigned int h;
					unsigned int m;
					double s;
					if (sscanf(optarg, "%u:%u:%lf", &h, &m, &s) == 3)
					{
						optionStartPosition = h * 3600 + m * 60 + s;
					}
					else
					{
						printf("invalid time specification.\n");
						throw Exception();
					}
				}
				else
				{
					optionStartPosition = atof(optarg);
				}

				printf("startPosition=%f\n", optionStartPosition);
			}
			break;

			case 'c':
				optionChapter = atoi(optarg);
				printf("optionChapter=%d\n", optionChapter);
				break;

			case 'v':
				optionVideoIndex = atoi(optarg);
				printf("optionVideoIndex=%d\n", optionVideoIndex);
				break;

			case 'a':
				optionAudioIndex = atoi(optarg);
				printf("optionAudioIndex=%d\n", optionAudioIndex);
				break;

			case 's':
				optionSubtitleIndex = atoi(optarg);
				printf("optionSubtitleIndex=%d\n", optionSubtitleIndex);
				break;

			case 'l':
				optionLoop = true;
				printf("optionLoop=true\n");
				break;
				
			case 'g':
				gpioTrigger = true;
				printf("gpioTrigger=true\n");
				break;
		}
	}

	const char* url = nullptr;
	if (optind < argc)
	{
		while (optind < argc)
		{
			url = argv[optind++];
			break;
		}
	}

	if (url == nullptr)
	{
		exit(0);
	}

	// Initialize GPIO input(s)
	unsigned int gpioCounter = 0;
	GPIOC2 *gpio[4];
	gpio[0] = new GPIOC2(11, INPUT); //GPIO0 (pin11)
	gpio[1] = new GPIOC2(12, INPUT); //GPIO1 (pin12)
	gpio[2] = new GPIOC2(13, INPUT); //GPIO2 (pin13)
	gpio[3] = new GPIOC2(15, INPUT); //GPIO3 (pin15)

	// Initialize libav
	av_log_set_level(AV_LOG_VERBOSE);
	av_register_all();
	avformat_network_init();

	WindowSPTR window;

#ifdef X11

	window = std::make_shared<X11AmlWindow>();

#else

	window = std::make_shared<FbdevAmlWindow>();

#endif
	 
	window->ProcessMessages();

	RenderContextSPTR renderContext = std::make_shared<RenderContext>(window->EglDisplay(),
		window->Surface(),
		window->Context());

	CompositorSPTR compositor = std::make_shared<Compositor>(renderContext, 1920, 1080);

	MediaPlayerSPTR mediaPlayer = std::make_shared<MediaPlayer>(url,
		avOptions,
		compositor,
		optionVideoIndex,
		optionAudioIndex,
		optionSubtitleIndex);

	if (optionChapter > -1)
	{
		if (optionChapter <= (int)mediaPlayer->Chapters()->size())
		{
			optionStartPosition = mediaPlayer->Chapters()->at(optionChapter - 1).TimeStamp;
			printf("MAIN: Chapter found (%f).\n", optionStartPosition);
		}
	}

	mediaPlayer->Seek(optionStartPosition);
	mediaPlayer->SetState(MediaState::Play);

	isRunning = true;

	unsigned int exitCode = 0;

	while (isRunning)
	{

		isRunning = window->ProcessMessages();

		// Handling GPIO inputs
		if (gpioTrigger && gpio[gpioCounter]->read_value() == 0) {
			
			exitCode = gpioCounter + 1;
			isRunning = false;

		} else {
			
			if (++gpioCounter == 4) {
				gpioCounter = 0;
			}	

			if (mediaPlayer->IsEndOfStream())
			{
				if (optionLoop) {
					mediaPlayer->Seek(0);
					mediaPlayer->SetState(MediaState::Play);
				} else {
					isRunning = false;
				}
			} else {
				usleep(10000); // 10msec do not decrease
			}
		}
	}

	// Release GPIO resources
	for (int r = 0; r < 4; r++) { delete gpio[r]; }

	// Provide GPIO number to the caller script
	return exitCode;
}
